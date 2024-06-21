/* SCE CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library Release 2.5.4
 */
/* 
 *                      Emotion Engine Library
 *                          Version 0.10
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                       libmc - libmc.h
 *                     header file of libmc
 *
 *      Version       Date           Design      Log
 *  --------------------------------------------------------------------
 *      0.00          1999- 7-28     T.Honda     the first version
 *      0.01          2001-11-01     iwano       update macro define/etc
 */

#ifndef _LIBMC_H_
#define _LIBMC_H_

#ifndef SCE_STM_R
#include <sifdev.h>
#endif

#define sceMcFuncNoCardInfo	(1)
#define sceMcFuncNoOpen		(2)
#define sceMcFuncNoClose	(3)
#define sceMcFuncNoSeek		(4)
#define sceMcFuncNoRead		(5)
#define sceMcFuncNoWrite	(6)
#define sceMcFuncNoFlush	(10)
#define sceMcFuncNoMkdir	(11)
#define sceMcFuncNoChDir	(12)
#define sceMcFuncNoGetDir	(13)
#define sceMcFuncNoFileInfo	(14)
#define sceMcFuncNoDelete	(15)
#define sceMcFuncNoFormat	(16)
#define sceMcFuncNoUnformat	(17)
#define sceMcFuncNoEntSpace	(18)
#define sceMcFuncNoRename	(19)
#define sceMcFuncChgPrior	(20)
#define sceMcFuncSlotMax	(21)


#define sceMcFileInfoCreate	0x01
#define sceMcFileInfoModify	0x02
#define sceMcFileInfoAttr	0x04

#define sceMcFileAttrReadable		SCE_STM_R
#define sceMcFileAttrWriteable		SCE_STM_W
#define sceMcFileAttrExecutable		SCE_STM_X
#define sceMcFileAttrDupProhibit	SCE_STM_C
#define sceMcFileAttrSubdir		SCE_STM_D
#define sceMcFileAttrClosed		0x0080
#define sceMcFileAttrPDAExec		0x0800
#define sceMcFileAttrPS1		0x1000

#define sceMcResSucceed		(0)
#define sceMcResChangedCard	(-1)
#define sceMcResNoFormat	(-2)
#define sceMcResFullDevice	(-3)
#define sceMcResNoEntry		(-4)
#define sceMcResDeniedPermit	(-5)
#define sceMcResNotEmpty	(-6)
#define sceMcResUpLimitHandle	(-7)
#define sceMcResFailReplace	(-8)

#define sceMcIniSucceed		(0)
#define sceMcIniErrKernel	(-101)
#define sceMcIniOldMcserv	(-120)
#define sceMcIniOldMcman	(-121)

#define sceMcErrUnbind		(-100)
#define sceMcErrSemapho		(-200)
#define sceMcErrNullStr		(-210)

#define sceMcTypeNoCard		(0)
#define sceMcTypePS1		(1)
#define sceMcTypePS2		(2)
#define sceMcTypePDA		(3)

#define sceMcExecRun		(0)
#define sceMcExecIdle		(-1)
#define sceMcExecFinish		(1)


typedef int	_iconVu0IVECTOR[4];
typedef float	_iconVu0FVECTOR[4];

typedef struct
{
	unsigned char Head[4];
	unsigned short Reserv1;
	unsigned short OffsLF;
	unsigned Reserv2;
	unsigned TransRate;
	_iconVu0IVECTOR BgColor[4];
	_iconVu0FVECTOR LightDir[3];
	_iconVu0FVECTOR LightColor[3];
	_iconVu0FVECTOR Ambient;
	unsigned char TitleName[68];
	unsigned char FnameView[64];
	unsigned char FnameCopy[64];
	unsigned char FnameDel[64];
	unsigned char Reserve3[512];
} sceMcIconSys;


typedef struct
{
	unsigned char Resv2,Sec,Min,Hour;
	unsigned char Day,Month;
	unsigned short Year;
} sceMcStDateTime;


typedef struct
{
	sceMcStDateTime _Create;
	sceMcStDateTime _Modify;
	unsigned FileSizeByte;
	unsigned short AttrFile;
	unsigned short Reserve1;
	unsigned Reserve2;
	unsigned PdaAplNo;
	unsigned char EntryName[32];
} sceMcTblGetDir __attribute__((aligned (64)));


#if defined(__LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

int sceMcInit(void);
int sceMcEnd(void);
int sceMcGetInfo(int, int, int *, int *, int *);
int sceMcOpen(int, int, const char *, int);
int sceMcClose(int);
int sceMcSeek(int, int, int);
int sceMcRead(int, void *, int);
int sceMcWrite(int, const void *, int);
int sceMcFlush(int);
int sceMcMkdir(int, int, const char *);
int sceMcChdir(int, int, const char *, char *);
int sceMcGetDir(int, int, const char *, unsigned, int, sceMcTblGetDir *);
int sceMcSetFileInfo(int, int, const char *, const char *, unsigned int);
int sceMcDelete(int, int, const char *);
int sceMcFormat(int, int);
int sceMcUnformat(int, int);
int sceMcGetEntSpace(int, int, const char *);
int sceMcRename(int, int, const char *, const char *);
int sceMcChangeThreadPriority(int);
int sceMcGetSlotMax(int);
int sceMcSync(int, int *, int *);

#if defined(__LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif
#endif /* _LIBMC_H_ */
