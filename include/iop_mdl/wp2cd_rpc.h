#ifndef WP2CD_RPC_H
#define WP2CD_RPC_H

#include "common.h"

#include <eekernel.h>
#include <eetypes.h>
#include <sifrpc.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// WP2 commands
#define WP2_NONE                (0)

#define WP2_QUIT                0x0001 /*        No args        */
#define WP2_CLOSE               0x0003 /*        No args        */
#define WP2_PRELOAD             0x0004 /*        No args        */
#define WP2_STOP                0x0006 /*        No args        */
#define WP2_SETVOLDIRECT        0x0009 /* Arg -> Direct volume  */
#define WP2_SETMASTERVOL        0x000a /* Arg -> Master volume  */
#define WP2_SDINIT              0x000d /* Arg -> Status         */
#define WP2_INIT                0x000e /* Arg -> Mode           */
#define WP2_SETCHANNEL          0x000f /* Arg -> Channels       */
#define WP2_PRELOADBACK         0x0015 /*        No args        */
#define WP2_BGMSETTRPOINT       0x0016 /* Arg -> Transfer pos   */
#define WP2_BGMINIT             0x8000 /* Arg -> Block size     */
#define WP2_START               0x8005 /* Arg -> Seek position  */
#define WP2_SEEK                0x8007 /*        No args        */
#define WP2_GETMODE             0x800b /*        No args        */
#define WP2_SETMODE             0x800c /* Arg -> Mode           */
#define WP2_GETTIME             0x8010 /*        No args        */
#define WP2_GETTIMESAMPLE       0x8011 /*        No args        */
#define WP2_OPENFLOC            0x8013 /* Arg -> File name      */
#define WP2_SEEKFLOC            0x8014 /* Arg -> File name      */
#define WP2_READBUF             0x8017 /*        No args        */

/* WP2 modes */
/* Set */
#define WP2_MODE_REPEAT_OFF      0x0000
#define WP2_MODE_REPEAT_DEFAULT  0x0001
#define WP2_MODE_REPEAT_FORCED   0x0002

#define WP2_MODE_STEREO          0x0000
#define WP2_MODE_MONO            0x0010

/* Get */
#define WP2_MODE_IDLE            0x0000
#define WP2_MODE_RUNNING         0x1000
#define WP2_MODE_PAUSE           0x2000
#define WP2_MODE_FADE            0x4000
#define WP2_MODE_TERMINATE       0x8000

/* WaveP2 module ID */
#define WP2_DEV (0x8800)

int WP2Init(void);
int WP2Ctrl(int command, int data0);

#endif