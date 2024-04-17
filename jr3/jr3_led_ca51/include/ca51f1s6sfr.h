#ifndef __CA51F1S6_H__
#define __CA51F1S6_H__

sfr P0    	 	= 0x80;
sfr SP       	= 0x81;
sfr DP0L     	= 0x82;
sfr DP0H     	= 0x83;
sfr DP1L     	= 0x84;
sfr DP1H     	= 0x85;
sfr PWCON    	= 0x86;
sfr PCON     	= 0x87;

sfr TCON     	= 0x88;
sfr TMOD     	= 0x89;
sfr TL0      	= 0x8a;
sfr TL1      	= 0x8b;
sfr TH0     	= 0x8c;
sfr TH1      	= 0x8d;
sfr IDLST    	= 0x8e;
sfr STPST    	= 0x8f;

sfr P1       	= 0x90;
sfr PWMEN 		= 0x91;
sfr PWMIF 		= 0x92;
sfr PWMCON 		= 0x94;
sfr PWMCKD		= 0x95;
sfr PWMDIVL 	= 0x96;
sfr PWMDIVH 	= 0x97;

sfr S1CON   	= 0x98;
sfr S1BUF    	= 0x99;
sfr PWMDUTH 	= 0x9a;
sfr PWMDUTL 	= 0x9b;
sfr INDEX 		= 0x9f;

sfr P2    		= 0xa0;
sfr WDFLG    	= 0xa1;
sfr WDVTHL   	= 0xa2;
sfr WDVTHH   	= 0xa3;
sfr T2CON 		= 0xa4;
sfr T2CRH 		= 0xa5;
sfr T2CRL 		= 0xa6;
sfr WDCON 		= 0xa7;

sfr IE       	= 0xa8;
sfr SPCON 		= 0xa9;
sfr SPDAT 		= 0xaa;
sfr SPSTA 		= 0xab;

sfr P3       	= 0xb0;

sfr IP       	= 0xb8;
sfr S2CON  		= 0xb9;
sfr S2BUF  		= 0xba;
sfr S2RELL  	= 0xbb;
sfr S2RELH  	= 0xbc;
sfr UDCKS2  	= 0xbd;

sfr I2CCON   	= 0xc0;
sfr I2CADR   	= 0xc1;
sfr I2CADM   	= 0xc2;
sfr I2CCCR    	= 0xc3;
sfr I2CDAT   	= 0xc4;
sfr I2CSTA   	= 0xc5;
sfr I2CFLG   	= 0xc6;
sfr SDASEL    	= 0xc7;

sfr CKCON    	= 0xc8;
sfr CKDIV    	= 0xc9;
sfr IHCFG    	= 0xca;
sfr TKCCFG  	= 0xcb;

sfr PSW      	= 0xd0;
sfr EP0CON   	= 0xd1;
sfr EP1CON   	= 0xd2;
sfr EP2CON   	= 0xd3;
sfr EPIF     	= 0xd4;
sfr TMCON    	= 0xd5;
sfr TMSNU    	= 0xd6;
sfr SCLSEL   	= 0xd7;

sfr UDCKS1    	= 0xd8;

sfr ACC      	= 0xe0;

sfr LVDCON   	= 0xe8;
sfr IDCODE  	= 0xef;


sfr B        	= 0xf0;
sfr CBYTE  		= 0xf1;
sfr TKPULLTRIM  = 0xf2;
sfr SPMAX    	= 0xf3;
sfr TKIE   		= 0xf4;

sfr TKST    	= 0xf8;
sfr TKCFG1    	= 0xf9;
sfr TKCFG2    	= 0xfa;
sfr TKCFG3     	= 0xfb;
sfr TKDL   		= 0xfc;
sfr TKDH   		= 0xfd;
sfr TKPULL   	= 0xfe;
sfr TKCFG4   	= 0xff;

/*  BIT Register  */
/*	PSW	*/
sbit CY   		= 0xD7;
sbit AC   		= 0xD6;
sbit F0   		= 0xD5;
sbit RS1  		= 0xD4;
sbit RS0  		= 0xD3;
sbit OV   		= 0xD2;
sbit DPS   		= 0xD1;
sbit P    		= 0xD0;
		  		
/*	TCON	*/
sbit TF1  		= 0x8F;
sbit TR1  		= 0x8E;
sbit TF0  		= 0x8D;
sbit TR0  		= 0x8C;
sbit IE1  		= 0x8B;
sbit IT1  		= 0x8A;
sbit IE0  		= 0x89;
sbit IT0  		= 0x88;

/*	IE	*/
sbit EA   		= 0xAF;
sbit INT4EN  	= 0xAE;
sbit INT3EN  	= 0xAD;
sbit INT2EN	   	= 0xAC;
sbit ET1  		= 0xAB;
sbit EX1  		= 0xAA;
sbit ET0  		= 0xA9;
sbit EX0  		= 0xA8;

/*	IP	*/ 
sbit PX4  		= 0xBE;
sbit PX3  		= 0xBD;
sbit PX2   		= 0xBC;
sbit PT1  		= 0xBB;
sbit PX1  		= 0xBA;
sbit PT0  		= 0xB9;
sbit PX0  		= 0xB8;


/*	P0	*/
sbit P00		= P0^0;
sbit P01		= P0^1;
sbit P02		= P0^2;
sbit P03		= P0^3;
sbit P04		= P0^4;
sbit P05		= P0^5;
sbit P06		= P0^6;
sbit P07		= P0^7;

/*	P1	*/     	
sbit P10		= P1^0;
sbit P11		= P1^1;
sbit P12		= P1^2;
sbit P13		= P1^3;
sbit P14		= P1^4;
sbit P15		= P1^5;
sbit P16		= P1^6;
sbit P17		= P1^7;

/*	P2	*/        	
sbit P20		= P2^0;
sbit P21		= P2^1;
sbit P22		= P2^2;
sbit P23		= P2^3;
sbit P24		= P2^4;
sbit P25		= P2^5;
sbit P26		= P2^6;
sbit P27		= P2^7;

/*	P3	*/
sbit P30		= P3^0;
sbit P31		= P3^1;
	
#endif  

