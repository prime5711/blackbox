#ifdef _ONLYREAD_
unsigned int ubldesc [] = {
0xA1ACED44, //Magic number 
0x00000020, // Entry point for the UBL
0x00008000, // No. of pages for the UBL code
0x0, // Starting block of UBL in NAND
0x0 // Starting page of UBL in NAND
};

#else
unsigned int ubldesc [] = {
0xA1ACED00, //Magic number 
0x00000100, // Entry point for the UBL
0x00000007, // No. of pages for the UBL code
0x1, // Starting block of UBL in NAND
0x1 // Starting page of UBL in NAND
};
#endif
