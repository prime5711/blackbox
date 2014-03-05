/* 
 * S35390A Driver for IPNC 
 */ 
 
#include <linux/config.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/ctype.h>
#include <linux/delay.h>
#include <linux/rtc.h> 
#include <linux/i2c.h> 
#include <linux/bcd.h> 
#include <linux/err.h>
#include <asm/rtc.h>


#define PINMUX3		 __REG(0x01c4000c)
#define SELECT_GIO14    PINMUX3 |= 0x100000        
#define SELECT_GIO15    PINMUX3 |= 0x80000


#define S35390A_I2C_ADDR       0x30 
 
#define S35390A_CMD_STATUS1    0 
#define S35390A_CMD_STATUS2    1 
#define S35390A_CMD_TIME1      2 
 
#define S35390A_BYTE_YEAR      0 
#define S35390A_BYTE_MONTH     1 
#define S35390A_BYTE_DAY       2 
#define S35390A_BYTE_WDAY      3 
#define S35390A_BYTE_HOURS     4 
#define S35390A_BYTE_MINS      5 
#define S35390A_BYTE_SECS      6 
 
#define S35390A_FLAG_POC       0x01 
#define S35390A_FLAG_BLD       0x02 
#define S35390A_FLAG_24H       0x40 
#define S35390A_FLAG_RESET     0x80 
#define S35390A_FLAG_TEST      0x01 

#define RULE_24HOUR  (1)    

static void s35390a_rtc_read_time(struct rtc_time *tm);
static struct i2c_client s35390a_i2c_client;
static struct i2c_driver s35390a_i2c_driver;
static int _i2c_attach_client(struct i2c_client *, struct i2c_driver *,
			      struct i2c_adapter *, int);

struct device *s35390a_i2c_dev;

struct s35390a { 
       struct i2c_client *client; 
       struct rtc_device *rtc; 
       int twentyfourhour; 
}; 

struct s35390a s35390aRtc;


/*
 * ======== _i2c_attach_client  ========
 */
/* This function is used to attach i2c client */
static int _i2c_attach_client(struct i2c_client *client,
			      struct i2c_driver *driver,
			      struct i2c_adapter *adap, int addr)
{
	int err = 0;

	if (client->adapter) {
		err = -EBUSY;	/* our client is already attached */
	} else {
		client->addr = addr;
		client->flags = I2C_CLIENT_ALLOW_USE;
		client->driver = driver;
		client->adapter = adap;


		err = i2c_attach_client(client);
		if (err) {
			client->adapter = NULL;
		}
	}
	return err;
}


static int s35390a_set_reg(struct s35390a *s35390a, int reg, char *buf, int len) 
{ 
       
       struct i2c_client *client = &s35390a_i2c_client; 
       struct i2c_msg msg[] = { 
               { client->addr | reg, 0, len, buf }, 
       }; 
       
       udelay (1000);

       /* Only write to the writable bits in the status1 register */ 
       if (reg == S35390A_CMD_STATUS1) 
               buf[0] &= 0xf; 

        udelay (1000);
 
       if ((i2c_transfer(client->adapter, msg, 1)) != 1) 
               return -EIO; 
 
       return 0; 
} 
 
static int s35390a_get_reg(struct s35390a *s35390a, int reg, char *buf, int len) 
{      
        
       struct i2c_client *client = &s35390a_i2c_client;
       struct i2c_msg msg[] = { 
               { client->addr | reg, I2C_M_RD, len, buf }, 
       };
       
       udelay (1000);
       
       if ((i2c_transfer(client->adapter, msg, 1)) != 1) 
               return -EIO;
       
       udelay (1000); 
 
       return 0; 
} 


int s35390a_disable_test_mode(struct s35390a *s35390a) 
{ 
       char buf[1]; 
 
       if (s35390a_get_reg(s35390a, S35390A_CMD_STATUS2, buf, sizeof(buf)) < 0) 
               return -EIO; 
 
       if (!(buf[0] & S35390A_FLAG_TEST)) 
               return 0; 
 
       buf[0] &= ~S35390A_FLAG_TEST; 
       return s35390a_set_reg(s35390a, S35390A_CMD_STATUS2, buf, sizeof(buf)); 
} 
 
static char s35390a_hr2reg(struct s35390a *s35390a, int hour) 
{ 
       if (s35390a->twentyfourhour) 
               return BIN2BCD(hour); 
 
       if (hour < 12) 
               return BIN2BCD(hour); 
 
       return 0x40 | BIN2BCD(hour - 12); 
} 
 
static int s35390a_reg2hr(struct s35390a *s35390a, char reg) 
{ 
       unsigned hour; 
 
       if (s35390a->twentyfourhour) 
               return BCD2BIN(reg & 0x3f); 
 
       hour = BCD2BIN(reg & 0x3f); 
       if (reg & 0x40) 
               hour += 12; 
 
       return hour; 
} 
 
static inline char reverse(char x) 
{ 
       x = ((x >> 1) & 0x55) | ((x << 1) & 0xaa); 
       x = ((x >> 2) & 0x33) | ((x << 2) & 0xcc); 
       return (x >> 4) | (x << 4); 
} 
 
static int s35390a_set_datetime(struct rtc_time *tm) 
{ 
       struct s35390a  *s35390a = &s35390aRtc; 
       int i, err; 
       char buf[7];
       struct timespec tv; 

       dev_dbg(s35390a, "%s: tm is secs=%d, mins=%d, hours=%d mday=%d, " 
               "mon=%d, year=%d, wday=%d\n", __FUNCTION__, tm->tm_sec, 
               tm->tm_min, tm->tm_hour, tm->tm_mday, tm->tm_mon, tm->tm_year, 
               tm->tm_wday);

       buf[S35390A_BYTE_YEAR] = BIN2BCD(tm->tm_year - 100); 
       buf[S35390A_BYTE_MONTH] = BIN2BCD(tm->tm_mon + 1); 
       buf[S35390A_BYTE_DAY] = BIN2BCD(tm->tm_mday); 
       buf[S35390A_BYTE_WDAY] = BIN2BCD(tm->tm_wday); 
       buf[S35390A_BYTE_HOURS] = s35390a_hr2reg(s35390a, tm->tm_hour); 
       buf[S35390A_BYTE_MINS] = BIN2BCD(tm->tm_min); 
       buf[S35390A_BYTE_SECS] = BIN2BCD(tm->tm_sec); 
 
       /* This chip expects the bits of each byte to be in reverse order */ 
       for (i = 0; i < 7; ++i) 
               buf[i] = reverse(buf[i]); 
 
       err = s35390a_set_reg(s35390a, S35390A_CMD_TIME1, buf, sizeof(buf)); 
       if( err < 0 )
            return err;
            
       tv.tv_nsec = 0;
       tv.tv_sec = mktime (tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
				tm->tm_hour, tm->tm_min, tm->tm_sec+2);
       do_settimeofday (&tv);

	   return 0;
} 
 
static int s35390a_get_datetime(struct rtc_time *tm) 
{ 
       struct s35390a *s35390a = &s35390aRtc;
       
       char buf[7]; 
       int i, err; 
       

       err = s35390a_get_reg(s35390a, S35390A_CMD_TIME1, buf, sizeof(buf)); 
        if (err < 0)
            return err; 


       /* This chip returns the bits of each byte in reverse order */ 
       for (i = 0; i < 7; ++i) 
               buf[i] = reverse(buf[i]);
       


       tm->tm_sec = BCD2BIN( (buf[S35390A_BYTE_SECS]) ); 
       tm->tm_min = BCD2BIN( (buf[S35390A_BYTE_MINS]) ); 
       tm->tm_hour = s35390a_reg2hr(s35390a, buf[S35390A_BYTE_HOURS]); 
       tm->tm_wday = BCD2BIN( (buf[S35390A_BYTE_WDAY]) ); 
       tm->tm_mday = BCD2BIN( (buf[S35390A_BYTE_DAY]) ); 
       tm->tm_mon = BCD2BIN( (buf[S35390A_BYTE_MONTH]) ) - 1; 
       tm->tm_year = BCD2BIN( (buf[S35390A_BYTE_YEAR]) ) + 100; 
		



		return 0;
} 

static void s35390a_rtc_read_time(struct rtc_time *tm) 
{ 

        s35390a_get_datetime(tm); 
} 
 
static int s35390a_rtc_set_time(struct rtc_time *tm) 
{ 
       return s35390a_set_datetime(tm); 
} 
 
 

 
static struct rtc_ops s35390a_rtc_ops = { 
       .owner          = THIS_MODULE,
       .read_time      = s35390a_rtc_read_time, 
       .set_time       = s35390a_rtc_set_time, 
}; 
 
static struct i2c_driver s35390a_driver;  



static int s35390a_probe(struct i2c_adapter *adap)
{
    struct s35390a *s35390a = &s35390aRtc;
    struct timespec tv;
    struct rtc_time tm;
    char buf[1];
    int status;


    s35390a -> client           = &s35390a_i2c_client;
        
	s35390a_i2c_dev = &(adap->dev);
	
	
	status = _i2c_attach_client(&s35390a_i2c_client, &s35390a_i2c_driver,
	               adap, S35390A_I2C_ADDR);


    s35390a_disable_test_mode(s35390a); 

    s35390a_get_reg(s35390a, S35390A_CMD_STATUS1, buf, sizeof(buf)); 

    if (buf[0] & S35390A_FLAG_24H) 
        s35390a->twentyfourhour = 1; 
    else 
        s35390a->twentyfourhour = 0;
	
	
	register_rtc(&s35390a_rtc_ops); 
    
    s35390a_rtc_read_time(&tm);
    
    tv.tv_nsec = 0;
	tv.tv_sec = mktime (tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
			    tm.tm_hour, tm.tm_min, tm.tm_sec);
	do_settimeofday(&tv);        
	               
    
    return status;

}


 
static int s35390a_remove(struct i2c_client *client) 
{ 

       return 0; 
} 



static int __init s35390a_rtc_init(void) 
{ 
	int err = 0;
    struct i2c_driver *driver = &s35390a_i2c_driver;


    SELECT_GIO14;
    SELECT_GIO15;

    driver->owner = THIS_MODULE;
    strlcpy(driver->name, "DM355 IPNC RTC I2C driver",
            sizeof(driver->name));
    driver->id = I2C_DRIVERID_EXP0;
    driver->flags = I2C_DF_NOTIFY;
    driver->attach_adapter = s35390a_probe;
    driver->detach_client = s35390a_remove;
    
    
	err = i2c_add_driver(driver);

	if (err) {
		printk(KERN_ERR "Failed to register S35390A I2C client.\n");
		return -1;
	}
	
    return err; 
} 
 
 
static void __exit s35390a_rtc_exit(void) 
{ 
       i2c_del_driver(&s35390a_driver); 
} 
 
MODULE_AUTHOR("APPROPHO"); 
MODULE_DESCRIPTION("S35390A RTC driver"); 
MODULE_LICENSE("GPL"); 
 
module_init(s35390a_rtc_init); 
module_exit(s35390a_rtc_exit); 
