#ifndef __CA51F1S6_XDATA_H__
#define __CA51F1S6_XDATA_H__

#ifdef _MAIN_C_
    #define XRAM_U8 		volatile unsigned char xdata
    #define XRAM_AT_(x)     _at_ x
#else
    #define XRAM_U8 		extern volatile unsigned char xdata
    #define XRAM_AT_(x)
#endif


XRAM_U8     P00F        XRAM_AT_(0x8000);
XRAM_U8     P01F        XRAM_AT_(0x8001);
XRAM_U8     P02F        XRAM_AT_(0x8002);
XRAM_U8     P03F        XRAM_AT_(0x8003);
XRAM_U8     P04F        XRAM_AT_(0x8004);
XRAM_U8     P05F        XRAM_AT_(0x8005);
XRAM_U8     P06F        XRAM_AT_(0x8006);
XRAM_U8     P07F        XRAM_AT_(0x8007);

XRAM_U8     P10F        XRAM_AT_(0x8008);
XRAM_U8     P11F        XRAM_AT_(0x8009);
XRAM_U8     P12F        XRAM_AT_(0x800a);
XRAM_U8     P13F        XRAM_AT_(0x800b);
XRAM_U8     P14F        XRAM_AT_(0x800c);
XRAM_U8     P15F        XRAM_AT_(0x800d);
XRAM_U8     P16F        XRAM_AT_(0x800e);
XRAM_U8     P17F        XRAM_AT_(0x800f);

XRAM_U8     P20F        XRAM_AT_(0x8010);
XRAM_U8 	P21F        XRAM_AT_(0x8011);
XRAM_U8 	P22F        XRAM_AT_(0x8012);
XRAM_U8 	P23F        XRAM_AT_(0x8013);
XRAM_U8 	P24F        XRAM_AT_(0x8014);
XRAM_U8 	P25F        XRAM_AT_(0x8015);
XRAM_U8 	P26F        XRAM_AT_(0x8016);
XRAM_U8 	P27F        XRAM_AT_(0x8017);

XRAM_U8 	P30F        XRAM_AT_(0x8018);
XRAM_U8 	P31F        XRAM_AT_(0x8019);

XRAM_U8 	ADCON       XRAM_AT_(0x8060);
XRAM_U8 	ADCFGL      XRAM_AT_(0x8061);
XRAM_U8 	ADCDL       XRAM_AT_(0x8062);
XRAM_U8 	ADCDH       XRAM_AT_(0x8063);
XRAM_U8 	ADCALL      XRAM_AT_(0x8064);
XRAM_U8 	ADCALH      XRAM_AT_(0x8065);

XRAM_U8 	S1RELL      XRAM_AT_(0x8068);
XRAM_U8 	S1RELH      XRAM_AT_(0x8069);

XRAM_U8 	P00C        XRAM_AT_(0x8120);
XRAM_U8 	P01C        XRAM_AT_(0x8121);
XRAM_U8 	P02C        XRAM_AT_(0x8122);
XRAM_U8 	P03C        XRAM_AT_(0x8123);
XRAM_U8 	P04C        XRAM_AT_(0x8124);
XRAM_U8 	P05C        XRAM_AT_(0x8125);
XRAM_U8 	P06C        XRAM_AT_(0x8126);
XRAM_U8 	P07C        XRAM_AT_(0x8127);

XRAM_U8 	P10C        XRAM_AT_(0x8128);
XRAM_U8 	P11C        XRAM_AT_(0x8129);
XRAM_U8 	P12C        XRAM_AT_(0x812a);
XRAM_U8 	P13C        XRAM_AT_(0x812b);
XRAM_U8 	P14C        XRAM_AT_(0x812c);
XRAM_U8 	P15C        XRAM_AT_(0x812d);
XRAM_U8 	P16C        XRAM_AT_(0x812e);
XRAM_U8 	P17C        XRAM_AT_(0x812f);

XRAM_U8 	P20C        XRAM_AT_(0x8130);
XRAM_U8 	P21C        XRAM_AT_(0x8131);
XRAM_U8 	P22C        XRAM_AT_(0x8132);
XRAM_U8 	P23C        XRAM_AT_(0x8133);
XRAM_U8 	P24C        XRAM_AT_(0x8134);
XRAM_U8 	P25C        XRAM_AT_(0x8135);
XRAM_U8 	P26C        XRAM_AT_(0x8136);
XRAM_U8 	P27C        XRAM_AT_(0x8137);

XRAM_U8 	P30C        XRAM_AT_(0x8138);
XRAM_U8 	P31C        XRAM_AT_(0x8139);

XRAM_U8 	MECON       XRAM_AT_(0xfc00);
XRAM_U8 	FSCMD       XRAM_AT_(0xfc01);
XRAM_U8 	FSDAT       XRAM_AT_(0xfc02);
XRAM_U8 	LOCK        XRAM_AT_(0xfc03);
XRAM_U8 	PADRD       XRAM_AT_(0xfc04);
XRAM_U8 	PTSL        XRAM_AT_(0xfc05);
XRAM_U8 	PTSH        XRAM_AT_(0xfc06);


#endif  
