/*
 *
 * Copyright (C) 2007 Texas Instruments Inc
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
/* display_vpbe.h */

#ifndef DISPLAY_VPBE_H
#define DISPLAY_VPBE_H


/*  Enum for Boolean variables  */
typedef enum {
	SET_0 = 0,
	SET_1 = 1
} CB_CR_ORDER, ATTRIBUTE, ROM_RAM_CLUT;

#ifdef __KERNEL__
#include <asm/arch/hardware.h>
#include <asm/io.h>

/*  Enum for Boolean variables  */
#define	VPBE_NON_INTERLACED  0
#define VPBE_INTERLACED  1
#define VPBE_MAX_NAME   10

/* This structure will store size parameters as per the mode selected by
   the user */
struct video_config_param {
	char    name[VPBE_MAX_NAME];
	u32     activepixels;
	u32     activelines;
	u16     fps;
	u8     	mode; /* interlaced or not interlaced */
	u32 	basepx; /* Display x,y start position */ 
	u32 	basepy;
	u16 	hint; /* VENC HINT val */
	u16 	hstart;  /* VENC HSTART val */
	u16 	hvalid;  /* VENC HVALID val */
	u16 	vint; /* VENC VINT val */
	u16 	vstart; /* VENC VSTART */
	u16 	vvalid; /* VENC VVALID */
	u16 	vstata; /* VENC VSTATA  only for Interlaced display */
};
typedef enum {
	VPBE_DISABLE = 0,
	VPBE_ENABLE = 1
} ATTENUATION, TRANSPARENCY, EXPANSION, BLINKING;


/*  Structure for window expansion  */
typedef struct vpbe_win_expansion {
	EXPANSION horizontal;
	EXPANSION vertical;	/* 1: Enable 0:disable */
} vpbe_win_expansion_t;

/* enumerated data types */
/* Enumerated data type to give id to each hardware plane */
enum vpbe_plane_id {
	VPBE_PLANE_VIDEO0= 0,/* Video0 */
	VPBE_PLANE_VIDEO1	/* Video1 */
};

/*  Structure for transparency and the blending factor for the bitmap window  */
typedef struct vpbe_bitmap_blend_params {
	unsigned int colorkey;	/* color key to be blend */
	unsigned int enable_colorkeying;	/* enable color keying */
	unsigned int bf;	/* valid range from 0 to 7 only. */
} vpbe_bitmap_blend_params_t;

/*  Structure for Video window configurable parameters  */
typedef struct vpbe_video_config_params {
    CB_CR_ORDER cb_cr_order;    /*Cb/Cr order in input data for a pixel. */
    /*    0: cb cr  1:  cr cb */
    vpbe_win_expansion_t exp_info;  /* HZ/VT Expansion enable disable */
} vpbe_video_config_params_t;

/*Union of structures giving the CLUT index for the 1, 2, 4 bit bitmap values.*/
typedef union vpbe_clut_idx {
    struct _for_4bit_bimap {
        unsigned char bitmap_val_0;
        unsigned char bitmap_val_1;
        unsigned char bitmap_val_2;
        unsigned char bitmap_val_3;
        unsigned char bitmap_val_4;
        unsigned char bitmap_val_5;
        unsigned char bitmap_val_6;
        unsigned char bitmap_val_7;
        unsigned char bitmap_val_8;
        unsigned char bitmap_val_9;
        unsigned char bitmap_val_10;
        unsigned char bitmap_val_11;
        unsigned char bitmap_val_12;
        unsigned char bitmap_val_13;
        unsigned char bitmap_val_14;
        unsigned char bitmap_val_15;
    } for_4bit_bimap;
    struct _for_2bit_bimap {
        unsigned char bitmap_val_0;
        unsigned char dummy0[4];
        unsigned char bitmap_val_1;
        unsigned char dummy1[4];
        unsigned char bitmap_val_2;
        unsigned char dummy2[4];
        unsigned char bitmap_val_3;
    } for_2bit_bimap;
    struct _for_1bit_bimap {
        unsigned char bitmap_val_0;
        unsigned char dummy0[14];
        unsigned char bitmap_val_1;
    } for_1bit_bimap;
} vpbe_clut_idx_t;

/*  Structure for bitmap window configurable parameters */
typedef struct vpbe_bitmap_config_params {
    /* Only for bitmap width = 1,2,4 bits */
    vpbe_clut_idx_t clut_idx;
    /* Attenuation value for YUV o/p for bitmap window */
    unsigned char attenuation_enable;
    /* 0: ROM DM270, 1:ROM DM320, 2:RAM CLUT */
    unsigned char clut_select;
} vpbe_bitmap_config_params_t;


/*  Structure for OSD window blinking options */
typedef struct vpbe_blink_option {
    BLINKING blinking;  /* 1: Enable blinking 0: Disable */
    unsigned int interval;  /* Valid only if blinking is 1 */
} vpbe_blink_option_t;


/*  Unioun for video/OSD configuration parameters  */
typedef union vpbe_conf_params {

	struct vpbe_video_params {
		CB_CR_ORDER cb_cr_order;
		/* HZ/VT Expansion enable disable */
		vpbe_win_expansion_t exp_info;
	} video_params;

	struct vpbe_bitmap_params {
		/* Attenuation value for YUV o/p */
		ATTENUATION attenuation_enable;
		/* 0: ROM DM270, 1: ROM DM320, 2:RAM CLUT */
		unsigned char clut_select;
		/* Only for bitmap width = 1,2,4 bits */
		vpbe_clut_idx_t clut_idx;
		/* 0: OSD window is bitmap window */
		/* 1: OSD window is attribute window */
		ATTRIBUTE enable_attribute;
		/* To hold bps value. 
		   Used to switch back from attribute to bitmap. */
		unsigned int stored_bits_per_pixel;
		/* Blending information */
		vpbe_bitmap_blend_params_t blend_info;
		/* OSD Blinking information */
		vpbe_blink_option_t blink_info;
	} bitmap_params;

} vpbe_conf_params_t;

typedef struct vpbe_video_params vpbe_video_params_t;
typedef struct vpbe_bitmap_params vpbe_bitmap_params_t;

struct fb_bitfield {
	u32 offset;			/* beginning of bitfield	*/
	u32 length;			/* length of bitfield		*/
	u32 msb_right;		/* != 0 : Most significant bit is */ 
					/* right */ 
};

struct vpbe_windowinfo {
	u32 x_pos;			/* x offset from base x pixel */
	u32 y_pos;			/* y offset from base y pixel */
	u32 width;			/* width of window in pixel */
	u32 height;			/* width of window in pixel */
	u32 bits_per_pixel;		/* guess what			*/
	u32 grayscale;		/* != 0 Graylevels instead of colors */
	struct fb_bitfield red;		/* bitfield in fb mem if true color, */
	struct fb_bitfield green;	/* else only length is significant */
	struct fb_bitfield blue;
	struct fb_bitfield transp;	/* transparency	*/
};


struct vpbe_display_stdinfo {
	char    name[30];
	u32     activepixels;
	u32     activelines;
	u16     fps;
	u8     frame_format;
};
struct vpbe_interface;

int vpbe_display_get_mode_info(struct vpbe_display_stdinfo *std_info);

#define ISNULL(p)       ((NULL) == (p))

void vpbe_set_vid0_sdram_address(u32, u32);
void vpbe_set_vid1_sdram_address(u32, u32);
void vpbe_set_vid0_win_size(u32, u32, u32, u32, u32);
void vpbe_set_vid1_win_size(u32, u32, u32, u32, u32);
void vpbe_set_vid0_interlaced(unsigned int);
void vpbe_set_vid1_interlaced(unsigned int);
void vpbe_set_vid0_win_enable(unsigned int);
void vpbe_set_vid1_win_enable(unsigned int);
int  vpbe_get_display_field_id(void);
void vpbe_set_vid1_win_mode(short bits_per_pixel);
void vpbe_set_vid0_win_mode(short bits_per_pixel);
int  vpbe_display_get_std_info(struct vpbe_display_stdinfo *std_info);
void vpbe_osd_set_vid_params(CB_CR_ORDER cb_cr_order);
void vpbe_osd_set_vid_expansion(vpbe_win_expansion_t * required);
void vpbe_set_bg_color(u8 clut, u8 color_offset);
void vpbe_osd_set_zoom_vid0(int h_factor, int v_factor);
void vpbe_osd_set_zoom_vid1(int h_factor, int v_factor);
int  vpbe_config_display_output(struct vpbe_display_stdinfo *stdinfo);
void vpbe_enable_vpbe_output(int on);
void vpbe_get_vid0_window_defaults(struct vpbe_windowinfo *info);
void vpbe_get_vid1_window_defaults(struct vpbe_windowinfo *info);
void vpbe_get_hardware_address_range(dma_addr_t *mmio_base, unsigned long *mmio_size);
void vpbe_set_display_default(void);
int  vpbe_check_for_dma_error(void);
void vpbe_reset_dma_error(void);
void vpbe_set_interrupt_register(void);

#endif

#endif				/* End of #ifndef DISPLAY_VPBE_H */
