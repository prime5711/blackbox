bin\hex470 -a  -romwidth 32  -datawidth 8  -memwidth 32  -order MS  -map UBL_OBJ\ublDM350.hexmap UBL_OBJ\ublDM350.out

.\bin\ascii2bin P

copy ublDM350.bin ublDM355-nand.bin

del ublDM350.a0


