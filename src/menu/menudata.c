#include "menu/menudata.h"

#include "os/system.h"

#include "main/etc.h"
#include "main/subt.h"
#include "main/cdctrl.h"
#include "main/scrctrl.h"

#include "iop_mdl/tapctrl_rpc.h"

// unsure where to put at the moment
typedef struct
{
    u_short bnkNo;
    SNDTAP *pTap;
} VOICE_TBL;

typedef struct
{
    int fno[4][4];
} MN_CRWCL1_TBL;

/* data */
/* static */ extern SNDTAP         sndtap_menu[];
/* static */ extern VOICE_TBL      VoiceSet[];
/* static */ extern int           *ocl1_rndTbl[];
/* static */ extern MN_CRWCL1_TBL *ocl1_stageTbl[];
/* static */ extern RT2TRANS_STR   rt2trans_str[];

extern MESS mess_mc[];
extern MESS mess_menu[];

/* sdata - static */
int _BankChan1Req;
int _BankChan1Stat;

INCLUDE_ASM("menu/menudata", MenuDataGetIconSysHed);

void MenuDataSndInit(void)
{
    _BankChan1Req = 0;
    _BankChan1Stat = 0;

    TapCt(0x8011, 0x135010, 0);
    TapCt(0x90, PR_CONCAT(0x3fff, 0x3fff), 0);
}

void MenuDataSndReq(int chanId, int req)
{
    SNDTAP *sndtap_pp = &sndtap_menu[req];

    TapCt(0xf0, chanId, sndtap_pp->volume);
    TapCt(0xd0, chanId, sndtap_pp->prg + sndtap_pp->key * 256);
}

void MenuDataSndStop(int chanId)
{
    TapCt(0xe0, chanId, 0);
}

void MenuDataSndQuit(void)
{
    int i;

    for (i = 0; i < 3; i++)
        TapCt(0x50 | i, 0, 0);
}

void MenuDataSndSetVol(int chanId, int req, int vol0)
{
    SNDTAP *sndtap_pp = &sndtap_menu[req];

    TapCt(0x130, chanId, sndtap_pp->volume * vol0 >> 8); /* Should be a division rather than a shift? */
}

MENU_SPU_ENUM MenuDataSndTrans(int bdId, int hdId, MENU_SPU_ENUM trId)
{
    TapCt(0x8030 | trId, (int)GetIntAdrsCurrent(bdId), (int)GetIntSizeCurrent(bdId));
    TapCt(0x8040 | trId, (int)GetIntAdrsCurrent(hdId), (int)GetIntSizeCurrent(hdId));

    return trId;
}

int MenuDataSndTransCheck(void)
{
    return TapCt(0x8070, 0, 0);
}

void MenuDataSndReqChan(int chanId, int req, MENU_SPU_ENUM trId)
{
    SNDTAP *sndtap_pp = &sndtap_menu[req];

    TapCt(0xf0 | trId, chanId, sndtap_pp->volume);
    TapCt(0xd0 | trId, chanId, sndtap_pp->prg + sndtap_pp->key * 256);
}

void MenuDataSpuVolume(int vol)
{
    if (vol << 7 <= 0x3FFF)
        vol = vol << 7;
    else
        vol = 0x3FFF;

    TapCt(0x90, vol << 16 | vol, 0);
}

void MenuDataDiskVolume(u_int vol)
{
    CdctrlWP2SetVolume(vol);
}

void MenuDataDiskSndReq(MENU_DISKSND_ENUM sndId)
{
    CdctrlWP2Set(&file_str_extra_file[sndId]);
}

int MenuDataDiskSndReady(void)
{
    return CdctrlWP2CheckBuffer();
}

void MenuDataDiskSndPlay()
{
    CdctrlWP2Play();
}

void MenuDataDiskSndEnd()
{
    CdctrlWp2FileEnd();
}

void menuDiskSndDebug(void *x)
{
    int sndId = MDISK_00;

    while (TRUE)
    {
    loop:
        do
        {
            MtcWait(1);
        } while (!(pad[1].one & SCE_PADRdown));

        MenuDataDiskSndReq(sndId);

        do
        {
            MtcWait(1);
        } while (MenuDataDiskSndReady());

        MenuDataDiskSndPlay();
        MenuDataDiskVolume(128);

        do
        {
            MtcWait(1);
        } while (!(pad[1].one & SCE_PADRdown));

        MenuDataDiskSndEnd();
        sndId++;

        if (sndId < MDISK_MAX)
            goto loop;

        sndId = MDISK_00;
    }
}

int MenuVoiceBankSet(int bnkNo)
{
    _BankChan1Req = bnkNo;
    _BankChan1Stat = 0;

    return 0;
}

#if 1
INCLUDE_ASM("menu/menudata", MenuVoicePlayVol);
#else
void MenuVoicePlayVol(int chanId, int vsetIdx, int vol0)
{
    SNDTAP       *sndtap_pp;
    MENU_SPU_ENUM trId;
    int           bnkNo;

    if (VoiceSet[vsetIdx].bnkNo == 0)
    {
        bnkNo = 0;
    }
    else if (_BankChan1Req != VoiceSet[vsetIdx].bnkNo)
    {
        return;
    }
    else
    {
        bnkNo = 1;
        if (_BankChan1Stat != 0 && MenuVoiceBankSet(_BankChan1Req) != 0)
        {
            return;
        }
    }

    sndtap_pp = VoiceSet[vsetIdx].pTap;

    TapCt(bnkNo | 0xf0, chanId, (sndtap_pp->volume * vol0) >> 8);
    TapCt(bnkNo | 0xd0, chanId, sndtap_pp->prg + sndtap_pp->key * 0x100);
}
#endif

void MenuVoicePlay(int chanId, int vsetIdx)
{
    MenuVoicePlayVol(chanId, vsetIdx, 0x100);
}

void MenuVoiceStop(int chanId)
{
    TapCt(0xe0, chanId, 0);
}

void MenuVoiceSetVol(int chanId, int vsetIdx, int vol0)
{
    SNDTAP *sndtap_pp;

    sndtap_pp = VoiceSet[vsetIdx].pTap;

    TapCt(0x130, chanId, (sndtap_pp->volume * vol0) >> 8);
}

void MenuMsgInit(void)
{
    SubtMenuCtrlInit(GetIntAdrsCurrent(INTNUM_SUBT_CODE));
    MENUSubtSetKanji(GetIntAdrsCurrent(INTNUM_SUBT_CODE));
}

void MenuMsgPrintSub(int id, int xp, int yp, int flg)
{
    SubtMenuCtrlPrint(mess_menu[id][flg], xp, yp, flg);
}

void MenuMsgPrintMc(int id, int xp, int yp, int flg)
{
    SubtMenuCtrlPrint(mess_mc[id][flg], xp, yp, flg);
}

char *MenuMsgGetMessageMc(int id, int flg)
{
    return mess_mc[id][flg];
}

char *MenuMsgGetMessageSub(int id, int flg)
{
    return mess_menu[id][flg];
}

int MenuRoundTim2Trans(TAP_ROUND_ENUM round)
{
    RT2TRANS_STR *rt2trans_str_pp;
    int           i;

    if (round < TRND_R4)
        hat_change_enum = HCNG_AUTO;

    if (hat_change_enum != HCNG_AUTO)
        round = hat_change_enum;

    if (round >= TRND_MAX)
        return 1;

    rt2trans_str_pp = &rt2trans_str[round];

    for (i = 0; i < rt2trans_str_pp->num; i++)
        Tim2Trans(GetIntAdrsCurrent(rt2trans_str_pp->data_pp[i]));

    return 0;
}

int MenuStageCl1Trans(int nStage, int nRound)
{
    if (nStage > 7U)
        return 1;

    if (nRound > 3U)
        return 0;

    Tim2Trans(GetIntAdrsCurrent(ocl1_rndTbl[nRound][nStage]));

    return 0;
}

int MenuCoolCl1Trans(int nStage, int nPos, int nRound)
{
    if (nStage > 7U)
        return 1;

    if (nRound > 3U)
        return 1;

    if (nPos > 3U)
        return 1;

    Tim2Trans(GetIntAdrsCurrent(ocl1_stageTbl[nStage]->fno[nPos][nRound]));

    return 0;
}

/* EUC-JP strings... scary */
INCLUDE_RODATA("menu/menudata", D_00393C00);

INCLUDE_RODATA("menu/menudata", D_00393C20);

INCLUDE_RODATA("menu/menudata", D_00393C38);

INCLUDE_RODATA("menu/menudata", D_00393C50);

INCLUDE_RODATA("menu/menudata", D_00393C70);

INCLUDE_RODATA("menu/menudata", D_00393C98);

INCLUDE_RODATA("menu/menudata", D_00393CD8);

INCLUDE_RODATA("menu/menudata", D_00393D10);

INCLUDE_RODATA("menu/menudata", D_00393D28);

INCLUDE_RODATA("menu/menudata", D_00393D38);

INCLUDE_RODATA("menu/menudata", D_00393D58);

INCLUDE_RODATA("menu/menudata", D_00393D78);

INCLUDE_RODATA("menu/menudata", D_00393DD0);

INCLUDE_RODATA("menu/menudata", D_00393E18);

INCLUDE_RODATA("menu/menudata", D_00393E40);

INCLUDE_RODATA("menu/menudata", D_00393E60);

INCLUDE_RODATA("menu/menudata", D_00393EB8);

INCLUDE_RODATA("menu/menudata", D_00393F10);

INCLUDE_RODATA("menu/menudata", D_00393F90);

INCLUDE_RODATA("menu/menudata", D_00393FE8);

INCLUDE_RODATA("menu/menudata", D_00394070);

INCLUDE_RODATA("menu/menudata", D_003940D8);

INCLUDE_RODATA("menu/menudata", D_00394190);

INCLUDE_RODATA("menu/menudata", D_00394218);

INCLUDE_RODATA("menu/menudata", D_003942D0);

INCLUDE_RODATA("menu/menudata", D_00394358);

INCLUDE_RODATA("menu/menudata", D_00394438);

INCLUDE_RODATA("menu/menudata", D_00394500);

INCLUDE_RODATA("menu/menudata", D_003945E0);

INCLUDE_RODATA("menu/menudata", D_003946A8);

INCLUDE_RODATA("menu/menudata", D_003946C0);

INCLUDE_RODATA("menu/menudata", D_003946D0);

INCLUDE_RODATA("menu/menudata", D_00394718);

INCLUDE_RODATA("menu/menudata", D_00394748);

INCLUDE_RODATA("menu/menudata", D_00394768);

INCLUDE_RODATA("menu/menudata", D_00394788);

INCLUDE_RODATA("menu/menudata", D_003947E0);

INCLUDE_RODATA("menu/menudata", D_00394828);

INCLUDE_RODATA("menu/menudata", D_00394850);

INCLUDE_RODATA("menu/menudata", D_00394890);

INCLUDE_RODATA("menu/menudata", D_003948B8);

INCLUDE_RODATA("menu/menudata", D_003948F8);

INCLUDE_RODATA("menu/menudata", D_00394918);

INCLUDE_RODATA("menu/menudata", D_00394938);

INCLUDE_RODATA("menu/menudata", D_00394988);

INCLUDE_RODATA("menu/menudata", D_003949C8);

INCLUDE_RODATA("menu/menudata", D_00394A38);

INCLUDE_RODATA("menu/menudata", D_00394AB0);

INCLUDE_RODATA("menu/menudata", D_00394B70);

INCLUDE_RODATA("menu/menudata", D_00394C08);

INCLUDE_RODATA("menu/menudata", D_00394D30);

INCLUDE_RODATA("menu/menudata", D_00394DD8);

INCLUDE_RODATA("menu/menudata", D_00394E10);

INCLUDE_RODATA("menu/menudata", D_00394E50);

INCLUDE_RODATA("menu/menudata", D_00394E60);

INCLUDE_RODATA("menu/menudata", D_00394EA8);

INCLUDE_RODATA("menu/menudata", D_00394EE8);

INCLUDE_RODATA("menu/menudata", D_00394F18);

INCLUDE_RODATA("menu/menudata", D_00394F48);

INCLUDE_RODATA("menu/menudata", D_00394F88);

INCLUDE_RODATA("menu/menudata", D_00394FB0);

INCLUDE_RODATA("menu/menudata", D_00394FF0);

INCLUDE_RODATA("menu/menudata", D_00395018);

INCLUDE_RODATA("menu/menudata", D_00395050);

INCLUDE_RODATA("menu/menudata", D_00395080);

INCLUDE_RODATA("menu/menudata", D_003950B8);

INCLUDE_RODATA("menu/menudata", D_003950E8);

INCLUDE_RODATA("menu/menudata", D_00395110);

INCLUDE_RODATA("menu/menudata", D_00395140);

INCLUDE_RODATA("menu/menudata", D_00395170);

INCLUDE_RODATA("menu/menudata", D_003951A0);

INCLUDE_RODATA("menu/menudata", D_003951C0);

INCLUDE_RODATA("menu/menudata", D_003951E0);

INCLUDE_RODATA("menu/menudata", D_00395218);

INCLUDE_RODATA("menu/menudata", D_00395248);

INCLUDE_RODATA("menu/menudata", D_00395268);

INCLUDE_RODATA("menu/menudata", D_00395288);

INCLUDE_RODATA("menu/menudata", D_003952C8);

INCLUDE_RODATA("menu/menudata", D_00395308);

INCLUDE_RODATA("menu/menudata", D_00395338);

INCLUDE_RODATA("menu/menudata", D_00395358);

INCLUDE_RODATA("menu/menudata", D_00395390);

INCLUDE_RODATA("menu/menudata", D_003953B8);

INCLUDE_RODATA("menu/menudata", D_00395400);

INCLUDE_RODATA("menu/menudata", D_00395438);

INCLUDE_RODATA("menu/menudata", D_00395488);

INCLUDE_RODATA("menu/menudata", D_003954C0);

INCLUDE_RODATA("menu/menudata", D_003954D0);

INCLUDE_RODATA("menu/menudata", D_003954E0);

INCLUDE_RODATA("menu/menudata", D_00395508);

INCLUDE_RODATA("menu/menudata", D_00395528);

INCLUDE_RODATA("menu/menudata", D_00395540);

INCLUDE_RODATA("menu/menudata", D_00395550);

INCLUDE_RODATA("menu/menudata", D_00395578);

INCLUDE_RODATA("menu/menudata", D_003955A0);

INCLUDE_RODATA("menu/menudata", D_003955E8);

INCLUDE_RODATA("menu/menudata", D_00395618);

INCLUDE_RODATA("menu/menudata", D_00395660);

INCLUDE_RODATA("menu/menudata", D_00395690);

INCLUDE_RODATA("menu/menudata", D_003956D0);

INCLUDE_RODATA("menu/menudata", D_003956F8);

INCLUDE_RODATA("menu/menudata", D_00395720);

INCLUDE_RODATA("menu/menudata", D_00395740);

INCLUDE_RODATA("menu/menudata", D_00395780);

INCLUDE_RODATA("menu/menudata", D_003957A8);

INCLUDE_RODATA("menu/menudata", D_003957E0);

INCLUDE_RODATA("menu/menudata", D_00395808);

INCLUDE_RODATA("menu/menudata", D_00395850);

INCLUDE_RODATA("menu/menudata", D_00395880);

INCLUDE_RODATA("menu/menudata", D_003958C0);

INCLUDE_RODATA("menu/menudata", D_003958E8);

INCLUDE_RODATA("menu/menudata", D_00395928);

INCLUDE_RODATA("menu/menudata", D_00395958);

INCLUDE_RODATA("menu/menudata", D_003959A0);

INCLUDE_RODATA("menu/menudata", D_003959D0);

INCLUDE_RODATA("menu/menudata", D_00395A08);

INCLUDE_RODATA("menu/menudata", D_00395A30);

INCLUDE_RODATA("menu/menudata", D_00395A60);

INCLUDE_RODATA("menu/menudata", D_00395A88);

INCLUDE_RODATA("menu/menudata", D_00395AC0);

INCLUDE_RODATA("menu/menudata", D_00395AE8);

INCLUDE_RODATA("menu/menudata", D_00395B10);

INCLUDE_RODATA("menu/menudata", D_00395B30);

INCLUDE_RODATA("menu/menudata", D_00395B68);

INCLUDE_RODATA("menu/menudata", D_00395B90);

INCLUDE_RODATA("menu/menudata", D_00395BC8);

INCLUDE_RODATA("menu/menudata", D_00395BF0);

INCLUDE_RODATA("menu/menudata", D_00395C20);

INCLUDE_RODATA("menu/menudata", D_00395C48);

INCLUDE_RODATA("menu/menudata", D_00395C70);

INCLUDE_RODATA("menu/menudata", D_00395C90);

INCLUDE_RODATA("menu/menudata", D_00395CB8);

INCLUDE_RODATA("menu/menudata", D_00395CD8);

INCLUDE_RODATA("menu/menudata", D_00395D00);

INCLUDE_RODATA("menu/menudata", D_00395D20);

INCLUDE_RODATA("menu/menudata", D_00395D48);

INCLUDE_RODATA("menu/menudata", D_00395D68);

INCLUDE_RODATA("menu/menudata", D_00395D90);

INCLUDE_RODATA("menu/menudata", D_00395DB0);

INCLUDE_RODATA("menu/menudata", D_00395DD8);

INCLUDE_RODATA("menu/menudata", D_00395DF8);

INCLUDE_RODATA("menu/menudata", D_00395E20);

INCLUDE_RODATA("menu/menudata", D_00395E40);

INCLUDE_RODATA("menu/menudata", D_00395E68);

INCLUDE_RODATA("menu/menudata", D_00395E88);

INCLUDE_RODATA("menu/menudata", D_00395ED0);
