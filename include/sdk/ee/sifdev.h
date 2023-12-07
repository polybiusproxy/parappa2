/* SCE CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library Release 2.5.4
 */
/*
 *                      Emotion Engine Library
 *                          Version 0.2.0
 *                           Shift-JIS
 *
 *         Copyright (C) 2001 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                            sifdev.h
 *                         develop library
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.1.0         
 *       0.2.0          01/23/2001      akiyuki     addition of const and
 *                                                   sceSifLoadModuleBuffer
 */

#ifndef _SIFDEV_H_DEFS
#define _SIFDEV_H_DEFS

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

/* Flag for sceOpen() */
#define SCE_RDONLY      0x0001
#define SCE_WRONLY      0x0002
#define SCE_RDWR        0x0003
#define SCE_NBLOCK      0x0010  /* Non-Blocking I/O */
#define SCE_APPEND      0x0100  /* append (writes guaranteed at the end) */
#define SCE_CREAT       0x0200  /* open with file create */
#define SCE_TRUNC       0x0400  /* open with truncation */
#define SCE_EXCL        0x0800  /* exclusive create */
#define SCE_NOBUF       0x4000  /* no device buffer and console interrupt */
#define SCE_NOWAIT      0x8000  /* asyncronous i/o */

/* SCE local usage */
#define SCE_NOWBDC      0x20000000  /* not write back d cashe */

/* Seek Code */
#ifndef SCE_SEEK_SET
#define SCE_SEEK_SET        (0)
#endif
#ifndef SCE_SEEK_CUR
#define SCE_SEEK_CUR        (1)
#endif
#ifndef SCE_SEEK_END
#define SCE_SEEK_END        (2)
#endif

/* Ioctl Code */
#define SCE_FS_EXECUTING	0x1

/* dev9 */
#define DDIOC_MODEL		(('D'<<8)|1)
#define DDIOC_OFF		(('D'<<8)|2)

/* hdd		*/
#define HIOCADDSUB              (('h'<<8)|1)
#define HIOCDELSUB              (('h'<<8)|2)
#define HIOCNSUB                (('h'<<8)|3)
#define HIOCFLUSH               (('h'<<8)|4)

#define HDIOC_MAXSECTOR         (('H'<<8)|1)
#define HDIOC_TOTALSECTOR       (('H'<<8)|2)
#define HDIOC_IDLE              (('H'<<8)|3)
#define HDIOC_FLUSH             (('H'<<8)|4)
#define HDIOC_SWAPTMP           (('H'<<8)|5)
#define HDIOC_DEV9OFF           (('H'<<8)|6)
#define HDIOC_STATUS            (('H'<<8)|7)
#define HDIOC_FORMATVER         (('H'<<8)|8)
#define HDIOC_SMARTSTAT		(('H'<<8)|9)
#define HDIOC_FREESECTOR	(('H'<<8)|10)
#define HDIOC_IDLEIMM		(('H'<<8)|11)

/* mount() flag 	*/
#define SCE_MT_RDWR		0x00
#define SCE_MT_RDONLY		0x01
#define SCE_MT_ROBUST		0x02
#define SCE_MT_ERRCHECK		0x04

/* pfs			 */
#define PIOCALLOC               (('p'<<8)|1)
#define PIOCFREE                (('p'<<8)|2)
#define PIOCATTRADD             (('p'<<8)|3)
#define PIOCATTRDEL             (('p'<<8)|4)
#define PIOCATTRLOOKUP          (('p'<<8)|5)
#define PIOCATTRREAD            (('p'<<8)|6)

#define PDIOC_ZONESZ            (('P'<<8)|1)
#define PDIOC_ZONEFREE          (('P'<<8)|2)
#define PDIOC_CLOSEALL          (('P'<<8)|3)
#define PDIOC_GETFSCKSTAT       (('P'<<8)|4)
#define PDIOC_CLRFSCKSTAT       (('P'<<8)|5)

/* CD/DVD Ioctl           */
#define CIOCSTREAMPAUSE         (('c'<<8)|13)
#define CIOCSTREAMRESUME        (('c'<<8)|14)
#define CIOCSTREAMSTAT          (('c'<<8)|15)

/* CD/DVD Devctl          */
#define CDIOC_READCLOCK         (('C'<<8)|12)
#define CDIOC_GETDISKTYP        (('C'<<8)|31)
#define CDIOC_GETERROR          (('C'<<8)|32)
#define CDIOC_TRAYREQ           (('C'<<8)|33)
#define CDIOC_STATUS            (('C'<<8)|34)
#define CDIOC_POWEROFF          (('C'<<8)|35)
#define CDIOC_MMODE             (('C'<<8)|36)
#define CDIOC_DISKRDY           (('C'<<8)|37)
#define CDIOC_STREAMINIT        (('C'<<8)|39)
#define CDIOC_BREAK	        (('C'<<8)|40)

#define CDIOC_SPINNOM           (('C'<<8)|128)
#define CDIOC_TRYCNT            (('C'<<8)|130)
#define CDIOC_STANDBY           (('C'<<8)|132)
#define CDIOC_STOP              (('C'<<8)|133)
#define CDIOC_PAUSE             (('C'<<8)|134)
#define CDIOC_GETTOC            (('C'<<8)|135)
#define CDIOC_READDVDDUALINFO	(('C'<<8)|137)
#define CDIOC_INIT              (('C'<<8)|138)



#define SCE_PAD_ADDRESS		0x1

/* Error codes */
#define	SCE_ENXIO	6	/* No such device or address */
#define	SCE_EBADF	9	/* Bad file number */
#define	SCE_ENODEV	19	/* No such device */
#define	SCE_EINVAL	22	/* Invalid argument */
#define	SCE_EMFILE	24	/* Too many open files */

#define	SCE_EBINDMISS		0x10000
#define	SCE_ECALLMISS		0x10001
#define	SCE_ETYPEMISS		0x10002
#define	SCE_ELOADMISS		0x10003
#define	SCE_EVERSIONMISS	0x10004

struct sce_stat {
        unsigned int    st_mode;        /* ファイルの種類(file/dir) */
                                        /* とモード(R/W/X) */
        unsigned int    st_attr;        /* デバイス依存の属性 */
        unsigned int    st_size;        /* ファイルサイズ 下位 32 bit */
        unsigned char   st_ctime[8];    /* 作成時間 */
        unsigned char   st_atime[8];    /* 最終参照時間 */
        unsigned char   st_mtime[8];    /* 最終変更時間 */
        unsigned int    st_hisize;      /* ファイルサイズ 上位 32bit */
        unsigned int    st_private[6];  /* その他 */
};

struct sce_dirent {
        struct sce_stat d_stat; /* ファイルのステータス */
        char d_name[256];       /* ファイル名(フルパスではない) */
        void    *d_private;     /* その他 */
};

#define SCE_CST_MODE    0x0001
#define SCE_CST_ATTR    0x0002
#define SCE_CST_SIZE    0x0004
#define SCE_CST_CT      0x0008
#define SCE_CST_AT      0x0010
#define SCE_CST_MT      0x0020
#define SCE_CST_PRVT    0x0040

extern int  sceOpen(const char *filename, int flag, ...);
extern int  sceClose(int fd);
extern int  sceRead(int fd, void *buf, int nbyte);
extern int  sceWrite(int fd, const void *buf, int nbyte);
extern int  sceLseek(int fd, int offset, int where);
extern int  sceIoctl(int fd, int req, void *);
extern int  sceFsReset(void);

extern int  sceDopen ( const char *dirname);
extern int  sceDclose ( int fd);
extern int  sceDread(int fd, struct sce_dirent *buf);
extern int  sceRemove ( const char *filename);
extern int  sceMkdir ( const char *dirname, int flag);
extern int  sceRmdir ( const char *dirname);
extern int  sceGetstat ( const char *name, struct sce_stat *buf);
extern int  sceChstat ( const char *name, struct sce_stat *buf, unsigned int cbit);
extern int  sceFormat( const char *path, const char *blkdevname, void *arg, int arglen);
extern int  sceChdir( const char *name);
extern int  sceSync( const char *path, int flag);	
extern int  sceMount( const char *fsdevname, const char *blkdevname, int flag, void *arg, int arglen);
extern int  sceUmount( const char *name);
extern long sceLseek64(int fd, long offset, int whence);
extern int  sceDevctl (const char *devname, int cmd, const void *arg,
		 unsigned int arglen, void *bufp, unsigned int buflen);
extern int  sceSymlink(const char *existing, const char *newname);
extern int  sceReadlink(const char *path, char *buf, unsigned int bufsize);
extern int  sceRename( const char *oldname, const char *newname);
extern int  sceIoctl2(int fd, int request, const void *argp,
		 unsigned int arglen, void *bufp, unsigned int buflen);
extern int  sceFsInit(void); 
extern int *scePowerOffHandler(void (*func)(void *),void *addr);

/*
    Memory Card status
*/
#define SCE_STM_R	0x0001
#define SCE_STM_W	0x0002
#define SCE_STM_X	0x0004
#define SCE_STM_C	0x0008
#define SCE_STM_F	0x0010
#define SCE_STM_D	0x0020

/*
    HDD Pfs status macro  
*/
/* Filetypes and Protection bits for pfs */
#define SCE_STM_FMT		(0xf  <<  12)
#define SCE_STM_FLNK		(0x4  <<  12)	/* symbolic link */
#define SCE_STM_FREG		(0x2  <<  12)	/* regular file  */
#define SCE_STM_FDIR		(0x1  <<  12)	/* directory 	 */
#define SCE_STM_ISLNK(m)	(((m) & SCE_STM_FMT) == SCE_STM_FLNK)
#define SCE_STM_ISREG(m)	(((m) & SCE_STM_FMT) == SCE_STM_FREG)
#define SCE_STM_ISDIR(m)	(((m) & SCE_STM_FMT) == SCE_STM_FDIR)

#define SCE_STM_SUID		04000		/* set uid bit   */
#define SCE_STM_SGID		02000		/* set gid bit   */
#define SCE_STM_SVTX		01000		/* sticky bit    */

#define SCE_STM_RWXU		00700
#define SCE_STM_RUSR		00400
#define SCE_STM_WUSR		00200
#define SCE_STM_XUSR		00100

#define SCE_STM_RWXG		00070
#define SCE_STM_RGRP		00040
#define SCE_STM_WGRP		00020
#define SCE_STM_XGRP		00010

#define SCE_STM_RWXO		00007
#define SCE_STM_ROTH		00004
#define SCE_STM_WOTH		00002
#define SCE_STM_XOTH		00001

#define SCE_STM_ALLUGO	\
	(SCE_STM_SUID|SCE_STM_SGID|SCE_STM_SVTX|SCE_STM_RWXUGO)
#define SCE_STM_RWXUGO		(SCE_STM_RWXU|SCE_STM_RWXG|SCE_STM_RWXO)
#define SCE_STM_RUGO		(SCE_STM_RUSR|SCE_STM_RGRP|SCE_STM_ROTH)
#define SCE_STM_WUGO		(SCE_STM_WUSR|SCE_STM_WGRP|SCE_STM_WOTH)
#define SCE_STM_XUGO		(SCE_STM_XUSR|SCE_STM_XGRP|SCE_STM_XOTH)


extern int sceSifInitIopHeap(void);
extern void *sceSifAllocIopHeap(unsigned int);
extern int sceSifFreeIopHeap(void *);
extern int sceSifLoadIopHeap(const char *, void *);
extern void *sceSifAllocSysMemory(int, unsigned int, void *);
extern int sceSifFreeSysMemory(void *);

extern unsigned int sceSifQueryMemSize(void);
extern unsigned int sceSifQueryMaxFreeMemSize(void);
extern unsigned int sceSifQueryTotalFreeMemSize(void);
extern void *sceSifQueryBlockTopAddress(void *);
extern unsigned int sceSifQueryBlockSize(void *);


/* ee load file routine */
typedef struct {
	unsigned int epc;
	unsigned int gp;
	unsigned int sp;
	unsigned int dummy;
} sceExecData;

#define SCE_SIF_TYPECHAR	0
#define SCE_SIF_TYPESHORT	1
#define SCE_SIF_TYPEINT		2

#define RESIDENT_END            (0)
#define REMOVABLE_RESIDENT_END  (2)
#define NO_RESIDENT_END         (1)
#define FAREWELL_END            (1)

extern int sceSifLoadModule(const char *filename, int args, const char *argp);
extern int sceSifLoadModuleBuffer(const void *addr, int args, const char *argp);
extern int sceSifLoadStartModule(const char *filename, int args,
	const char *argp, int *result);
extern int sceSifLoadStartModuleBuffer(const void *addr, int args,
	const char *argp, int *result);
extern int sceSifLoadElf(const char *name, sceExecData *data);
extern int sceSifLoadElfPart(const char *name, const char *secname,
	sceExecData *data);
extern int sceSifStopModule(int modid, int args,
			     const char *argp, int *result);
extern int sceSifUnloadModule(int modid);
extern int sceSifSearchModuleByName(const char *modulename);
extern int sceSifSearchModuleByAddress(const void *addr);
    
extern int sceSifLoadFileReset(void);

extern int sceSifRebootIop(const char *img);
extern int sceSifSyncIop(void);

#define IOP_IMAGE_FILE "IOPRP254.IMG"
#define IOP_IMAGE_file "ioprp254.img"

/* extern int errno; */


#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif /* _SIFDEV_H_DEFS */

/* End of File */
