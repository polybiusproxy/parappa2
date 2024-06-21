#include "menu/mntm2hed.h"

/* data */
extern MENU_TM2_HED menu_tm2_hed[]; /* size: 105 */

MENU_TM2_HED* TsGetTM2Hed(int no)
{
    return &menu_tm2_hed[no];
}
