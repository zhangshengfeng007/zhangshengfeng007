#ifndef _AES_H
#define _AES_H


#define BPOLY 0x1b 		//!< Lower 8 bits of (x^8+x^4+x^3+x+1), ie. (x^4+x^3+x+1).
#define BLOCKSIZE 16 	//!< Block size in number of bytes.

#define KEY_COUNT 1

#if KEY_COUNT == 1
  #define KEYBITS 128 //!< Use AES128.
#elif KEY_COUNT == 2
  #define KEYBITS 192 //!< Use AES196.
#elif KEY_COUNT == 3
  #define KEYBITS 256 //!< Use AES256.
#else
  #error Use 1, 2 or 3 keys!
#endif


#if KEYBITS == 128
  #define ROUNDS 10 		//!< Number of rounds.
  #define KEYLENGTH 16 	//!< Key length in number of bytes.
#elif KEYBITS == 192
  #define ROUNDS 12 		//!< Number of rounds.
  #define KEYLENGTH 24 	//!< // Key length in number of bytes.
#elif KEYBITS == 256
  #define ROUNDS 14 		//!< Number of rounds.
  #define KEYLENGTH 32 	//!< Key length in number of bytes.
#else
  #error Key must be 128, 192 or 256 bits!
#endif


#define EXPANDED_KEY_SIZE (BLOCKSIZE * (ROUNDS+1)) //!< 176, 208 or 240 bytes.

void CalcPowLog(unsigned char *powTbl, unsigned char *logTbl);
void CalcSBox( unsigned char * sBox );
void CalcSBoxInv( unsigned char * sBox, unsigned char * sBoxInv );
void CycleLeft( unsigned char * row );
void InvMixColumn( unsigned char * column );
void SubBytes( unsigned char * bytes, unsigned char count );
void InvSubBytesAndXOR( unsigned char * bytes, unsigned char * key, unsigned char count );
void InvShiftRows( unsigned char * state );
void InvMixColumns( unsigned char * state );
void XORBytes( unsigned char * bytes1, unsigned char * bytes2, unsigned char count );
void CopyBytes( unsigned char * to, unsigned char * from, unsigned char count );
void KeyExpansion( unsigned char * expandedKey );
void InvCipher( unsigned char * block, unsigned char * expandedKey );
void aesDecInit(void);
void aesDecrypt( unsigned char * buffer, unsigned char * chainBlock );
unsigned char Multiply( unsigned char num, unsigned char factor );
unsigned char DotProduct( unsigned char * vector1, unsigned char * vector2 );
void MixColumn( unsigned char * column );
void MixColumns( unsigned char * state );
void ShiftRows( unsigned char * state );
void Cipher( unsigned char * block, unsigned char * expandedKey );
void aesEncInit(void);
void aesEncrypt( unsigned char * buffer, unsigned char * chainBlock );

extern const unsigned char  AES_KEY[32];	


extern unsigned char AES_ENCODE(unsigned char *key,unsigned char *databuf,unsigned int datalen);//数据长度要为16的倍数,密钥为32为
extern unsigned char AES_DECODE(unsigned char *key,unsigned char *databuf,unsigned int datalen);//数据长度要为16的倍数,密钥为32为


#endif

