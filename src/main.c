#include <kernel.h>
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

int main(int argc, char** argv)
{
    int ID, RET;
	char hddload_argc[34];

    SifInitRpc(0);
    while (!SifIopReset("", 0)) {};
    while (!SifIopSync()) {};

    SifInitIopHeap();
    SifLoadFileInit();
    fioInit();
    construct_HDDLOAD_argvc(hddload_argc);
    ID = LOAD_IRX(ps2dev9_irx, 0, NULL, &RET);
    ID = LOAD_IRX(ps2atad_irx, 0, NULL, &RET);
    ID = LOAD_IRX(hddload_irx, sizeof(hddload_argc), hddload_argc, &RET);
    IsHddSupportEnabled = (ID >= 0 && RET != 1);
    DetermineHDDLoadStatus();

    if (GetHddSupportEnabledStatus() && GetHddUpdateStatus())
    {
        ExecPS2(0x00100000, NULL, 0, NULL);
    }
    SleepThread();
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