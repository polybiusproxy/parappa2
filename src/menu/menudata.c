#include "common.h"

#include "os/system.h"

#include "main/etc.h"
#include "main/subt.h"
#include "main/cdctrl.h"
#include "main/scrctrl.h"

#include "iop_mdl/tapctrl_rpc.h"

// temp
int hat_change_enum;

/* data */
/* static */ extern SNDTAP sndtap_menu[];
extern MESS mess_mc[];
extern MESS mess_menu[];

/* sdata - static */
int _BankChan1Req;
int _BankChan1Stat;

INCLUDE_ASM(const s32, "menu/menudata", MenuDataGetIconSysHed);

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
    {
        TapCt(0x50 | i, 0, 0);
    }
}

void MenuDataSndSetVol(int chanId, int req, int vol0)
{
    SNDTAP *sndtap_pp = &sndtap_menu[req];

    TapCt(0x130, chanId, sndtap_pp->volume * vol0 >> 8); /* Should be a division rather than a shift? */
}

INCLUDE_ASM(const s32, "menu/menudata", MenuDataSndTrans);

int MenuDataSndTransCheck(void)
{
    return TapCt(0x8070, 0, 0);
}

INCLUDE_ASM(const s32, "menu/menudata", MenuDataSndReqChan);

INCLUDE_ASM(const s32, "menu/menudata", MenuDataSpuVolume);

INCLUDE_ASM(const s32, "menu/menudata", MenuDataDiskVolume);

INCLUDE_ASM(const s32, "menu/menudata", MenuDataDiskSndReq);

INCLUDE_ASM(const s32, "menu/menudata", MenuDataDiskSndReady);

INCLUDE_ASM(const s32, "menu/menudata", MenuDataDiskSndPlay);

INCLUDE_ASM(const s32, "menu/menudata", MenuDataDiskSndEnd);

INCLUDE_ASM(const s32, "menu/menudata", menuDiskSndDebug);

INCLUDE_ASM(const s32, "menu/menudata", MenuVoiceBankSet);

INCLUDE_ASM(const s32, "menu/menudata", MenuVoicePlayVol);

INCLUDE_ASM(const s32, "menu/menudata", MenuVoicePlay);

INCLUDE_ASM(const s32, "menu/menudata", MenuVoiceStop);

INCLUDE_ASM(const s32, "menu/menudata", MenuVoiceSetVol);

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

char* MenuMsgGetMessageMc(int id, int flg)
{
    return mess_mc[id][flg];
}

char* MenuMsgGetMessageSub(int id, int flg)
{
    return mess_menu[id][flg];
}

INCLUDE_ASM(const s32, "menu/menudata", MenuRoundTim2Trans);

INCLUDE_ASM(const s32, "menu/menudata", MenuStageCl1Trans);

INCLUDE_ASM(const s32, "menu/menudata", MenuCoolCl1Trans);

/* EUC-JP strings... scary */
INCLUDE_RODATA(const s32, "menu/menudata", D_00393C00);

INCLUDE_RODATA(const s32, "menu/menudata", D_00393C20);

INCLUDE_RODATA(const s32, "menu/menudata", D_00393C38);

INCLUDE_RODATA(const s32, "menu/menudata", D_00393C50);

INCLUDE_RODATA(const s32, "menu/menudata", D_00393C70);

INCLUDE_RODATA(const s32, "menu/menudata", D_00393C98);

INCLUDE_RODATA(const s32, "menu/menudata", D_00393CD8);

INCLUDE_RODATA(const s32, "menu/menudata", D_00393D10);

INCLUDE_RODATA(const s32, "menu/menudata", D_00393D28);

INCLUDE_RODATA(const s32, "menu/menudata", D_00393D38);

INCLUDE_RODATA(const s32, "menu/menudata", D_00393D58);

INCLUDE_RODATA(const s32, "menu/menudata", D_00393D78);

INCLUDE_RODATA(const s32, "menu/menudata", D_00393DD0);

INCLUDE_RODATA(const s32, "menu/menudata", D_00393E18);

INCLUDE_RODATA(const s32, "menu/menudata", D_00393E40);

INCLUDE_RODATA(const s32, "menu/menudata", D_00393E60);

INCLUDE_RODATA(const s32, "menu/menudata", D_00393EB8);

INCLUDE_RODATA(const s32, "menu/menudata", D_00393F10);

INCLUDE_RODATA(const s32, "menu/menudata", D_00393F90);

INCLUDE_RODATA(const s32, "menu/menudata", D_00393FE8);

INCLUDE_RODATA(const s32, "menu/menudata", D_00394070);

INCLUDE_RODATA(const s32, "menu/menudata", D_003940D8);

INCLUDE_RODATA(const s32, "menu/menudata", D_00394190);

INCLUDE_RODATA(const s32, "menu/menudata", D_00394218);

INCLUDE_RODATA(const s32, "menu/menudata", D_003942D0);

INCLUDE_RODATA(const s32, "menu/menudata", D_00394358);

INCLUDE_RODATA(const s32, "menu/menudata", D_00394438);

INCLUDE_RODATA(const s32, "menu/menudata", D_00394500);

INCLUDE_RODATA(const s32, "menu/menudata", D_003945E0);

INCLUDE_RODATA(const s32, "menu/menudata", D_003946A8);

INCLUDE_RODATA(const s32, "menu/menudata", D_003946C0);

INCLUDE_RODATA(const s32, "menu/menudata", D_003946D0);

INCLUDE_RODATA(const s32, "menu/menudata", D_00394718);

INCLUDE_RODATA(const s32, "menu/menudata", D_00394748);

INCLUDE_RODATA(const s32, "menu/menudata", D_00394768);

INCLUDE_RODATA(const s32, "menu/menudata", D_00394788);

INCLUDE_RODATA(const s32, "menu/menudata", D_003947E0);

INCLUDE_RODATA(const s32, "menu/menudata", D_00394828);

INCLUDE_RODATA(const s32, "menu/menudata", D_00394850);

INCLUDE_RODATA(const s32, "menu/menudata", D_00394890);

INCLUDE_RODATA(const s32, "menu/menudata", D_003948B8);

INCLUDE_RODATA(const s32, "menu/menudata", D_003948F8);

INCLUDE_RODATA(const s32, "menu/menudata", D_00394918);

INCLUDE_RODATA(const s32, "menu/menudata", D_00394938);

INCLUDE_RODATA(const s32, "menu/menudata", D_00394988);

INCLUDE_RODATA(const s32, "menu/menudata", D_003949C8);

INCLUDE_RODATA(const s32, "menu/menudata", D_00394A38);

INCLUDE_RODATA(const s32, "menu/menudata", D_00394AB0);

INCLUDE_RODATA(const s32, "menu/menudata", D_00394B70);

INCLUDE_RODATA(const s32, "menu/menudata", D_00394C08);

INCLUDE_RODATA(const s32, "menu/menudata", D_00394D30);

INCLUDE_RODATA(const s32, "menu/menudata", D_00394DD8);

INCLUDE_RODATA(const s32, "menu/menudata", D_00394E10);

INCLUDE_RODATA(const s32, "menu/menudata", D_00394E50);

INCLUDE_RODATA(const s32, "menu/menudata", D_00394E60);

INCLUDE_RODATA(const s32, "menu/menudata", D_00394EA8);

INCLUDE_RODATA(const s32, "menu/menudata", D_00394EE8);

INCLUDE_RODATA(const s32, "menu/menudata", D_00394F18);

INCLUDE_RODATA(const s32, "menu/menudata", D_00394F48);

INCLUDE_RODATA(const s32, "menu/menudata", D_00394F88);

INCLUDE_RODATA(const s32, "menu/menudata", D_00394FB0);

INCLUDE_RODATA(const s32, "menu/menudata", D_00394FF0);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395018);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395050);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395080);

INCLUDE_RODATA(const s32, "menu/menudata", D_003950B8);

INCLUDE_RODATA(const s32, "menu/menudata", D_003950E8);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395110);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395140);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395170);

INCLUDE_RODATA(const s32, "menu/menudata", D_003951A0);

INCLUDE_RODATA(const s32, "menu/menudata", D_003951C0);

INCLUDE_RODATA(const s32, "menu/menudata", D_003951E0);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395218);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395248);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395268);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395288);

INCLUDE_RODATA(const s32, "menu/menudata", D_003952C8);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395308);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395338);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395358);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395390);

INCLUDE_RODATA(const s32, "menu/menudata", D_003953B8);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395400);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395438);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395488);

INCLUDE_RODATA(const s32, "menu/menudata", D_003954C0);

INCLUDE_RODATA(const s32, "menu/menudata", D_003954D0);

INCLUDE_RODATA(const s32, "menu/menudata", D_003954E0);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395508);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395528);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395540);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395550);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395578);

INCLUDE_RODATA(const s32, "menu/menudata", D_003955A0);

INCLUDE_RODATA(const s32, "menu/menudata", D_003955E8);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395618);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395660);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395690);

INCLUDE_RODATA(const s32, "menu/menudata", D_003956D0);

INCLUDE_RODATA(const s32, "menu/menudata", D_003956F8);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395720);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395740);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395780);

INCLUDE_RODATA(const s32, "menu/menudata", D_003957A8);

INCLUDE_RODATA(const s32, "menu/menudata", D_003957E0);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395808);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395850);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395880);

INCLUDE_RODATA(const s32, "menu/menudata", D_003958C0);

INCLUDE_RODATA(const s32, "menu/menudata", D_003958E8);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395928);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395958);

INCLUDE_RODATA(const s32, "menu/menudata", D_003959A0);

INCLUDE_RODATA(const s32, "menu/menudata", D_003959D0);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395A08);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395A30);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395A60);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395A88);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395AC0);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395AE8);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395B10);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395B30);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395B68);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395B90);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395BC8);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395BF0);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395C20);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395C48);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395C70);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395C90);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395CB8);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395CD8);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395D00);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395D20);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395D48);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395D68);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395D90);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395DB0);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395DD8);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395DF8);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395E20);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395E40);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395E68);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395E88);

INCLUDE_RODATA(const s32, "menu/menudata", D_00395ED0);
