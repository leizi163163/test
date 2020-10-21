
#ifndef __STATE_H
#define __STATE_H


#define ERR_ARG         -1
#define ERR_GETHANDLE   -2
#define ERR_OPENFILE    -3
#define ERR_OPERATE     -4
#define ERR_UNSUSPPORT  -5
#define ERR_UNDEF       -6

#ifndef FAIL
#define FAIL	-1
#endif

//#ifndef SUCCESS
//#define SUCCESS		0
//#endif

#ifndef NULL
#define NULL    (void*)0
#endif

//#ifndef ERROR
//#define ERROR    -1
//#endif

#ifndef OK
#define OK       0
#endif


    
//typedef char            	INT8S;
typedef short           	INT16S;
typedef int             	INT32S;
typedef unsigned char   	INT8U;
typedef unsigned short  	INT16U;
typedef unsigned int    	INT32U;
typedef char            	S8;
typedef short           	S16;
typedef int             	S32;
//typedef unsigned char   	U8;
//typedef unsigned short  	U16;
//typedef unsigned int    	U32;
typedef float           	F32;
typedef double          	F64;
typedef char            	INT8;
typedef short           	INT16;
typedef int             	INT32;
typedef unsigned char   	UINT8_T;
typedef unsigned short  	UINT16_T;
//typedef unsigned int    	UINT32;
typedef unsigned int    	UINT32_T;

typedef unsigned long long  UINT64;

#endif


