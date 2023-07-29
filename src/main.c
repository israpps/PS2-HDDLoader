#include <kernel.h>
#include <ps2sdkapi.h>
#include <debug.h>
#include <sbv_patches.h>
#include <loadfile.h>
#include <iopcontrol.h>
#include "HDDSupport.h"

#define IMPORT_BIN2C(_n)       \
    extern unsigned char _n[]; \
    extern unsigned int size_##_n

IMPORT_BIN2C(ps2atad_irx);
IMPORT_BIN2C(ps2dev9_irx);
IMPORT_BIN2C(hddload_irx);

#define LOAD_IRX(__IRX__, argc, argv, RETURN_VALUE_PTR) \
    SifExecModuleBuffer(__IRX__, size_##__IRX__, argc, argv, RETURN_VALUE_PTR)

#define COL_BLUE   0xFF0000
#define COL_GREEN  0x00FF00
#define COL_RED    0x0000FF
#define COL_YELLOW 0x00FFFF
#define COL_WHITE  0xFFFFFF

#define DEBUG
#ifdef DEBUG
#include <stdio.h>
#include <sio.h>
#define DPRINTF(x...) printf(x);
#define DBGCOL(X...) DEBUG_BGCOLOR(X)
int cookie_wrt(void* cookie, const char* buf, int z)
{
    return sio_write(buf, z);
}

#define DPRINTF_INIT() \
	stdout = fwopen(NULL, cookie_wrt); \
    setvbuf(stdout, NULL, _IONBF, 0); \
    sio_init(38400, 0, 0, 0, 0)
#else
#define DPRINTF(x...)
#define DPRINTF_INIT(x...)
#define DBGCOL(x...)
#endif


int main(int argc, char** argv)
{
    int ID, RET;
	char hddload_argc[34];
    SifInitRpc(0);
    sbv_patch_disable_prefix_check();
    sbv_patch_enable_lmb();
    DPRINTF_INIT();
    //while (!SifIopReset("", 0)) {};
    //while (!SifIopSync()) {};
    DPRINTF("EE SIO::::%s\n", __FUNCTION__);
    //SifInitIopHeap();
    SifLoadFileInit();
    fioInit();
    construct_HDDLOAD_argvc(hddload_argc, sizeof(hddload_argc));
    DPRINTF("NEW HDDLOAD ARGUMMENTS: ");
    for (ID = 0; ID < sizeof(hddload_argc); ID++)
    {
        if (hddload_argc[ID] == 0) 
        {
            DPRINTF(" ");
        } else {
            DPRINTF("%c", hddload_argc[ID]);
        }
    }
    DPRINTF("}\n");
    DBGCOL(COL_BLUE);
    ID = LOAD_IRX(ps2dev9_irx, 0, NULL, &RET);
    DPRINTF("[DEV9]: ID=%d RET=%d\n", ID, RET);
    DBGCOL(COL_GREEN);
    ID = LOAD_IRX(ps2atad_irx, 0, NULL, &RET);
    DPRINTF("[ATAD]: ID=%d RET=%d\n", ID, RET);
    DBGCOL(COL_YELLOW);
    ID = LOAD_IRX(hddload_irx, sizeof(hddload_argc), hddload_argc, &RET);
    DPRINTF("[HDDLOAD]: ID=%d RET=%d\n", ID, RET);
    IsHddSupportEnabled = (ID >= 0 && RET != 1);
    DetermineHDDLoadStatus();

    DBGCOL(0);
    DPRINTF("WAITING FOR HDDLOAD TO GET READY\n");
    for (ID = 0; !(GetHddSupportEnabledStatus() && GetHddUpdateStatus()) || ID < 9; ID++)
    {
        sleep(1);
        DPRINTF(".");
    }
    DPRINTF("\nWaited for ~%d seconds\n", ID);
    if (GetHddSupportEnabledStatus() && GetHddUpdateStatus())
    {
        DPRINTF("OK!\n\t-- EXECUTING MBR.KELF at 0x00100000\n");
        fflush(stdout);
        sleep(2);
        ExecPS2((void*)0x00100000, NULL, 0, NULL);
    } else {
        DPRINTF("SHIT: HddSupportEnabledStatus:%d | HddUpdateStatus:%d\n", GetHddSupportEnabledStatus(), GetHddUpdateStatus());
    }
    DBGCOL(COL_WHITE);
    //SleepThread();
    return 0;
}


#if defined(DUMMY_TIMEZONE)
   void _libcglue_timezone_update() {}
#endif

#if defined(DUMMY_LIBC_INIT)
   void _libcglue_init() {}
   void _libcglue_deinit() {}
   void _libcglue_args_parse() {}
#endif

#if defined(KERNEL_NOPATCH)
    DISABLE_PATCHED_FUNCTIONS();
#endif

PS2_DISABLE_AUTOSTART_PTHREAD();
//DISABLE_EXTRA_TIMERS_FUNCTIONS();
