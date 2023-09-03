#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>

#define printf pspDebugScreenPrintf

PSP_MODULE_INFO("DeadlyStagesDemo", 0, 1, 0);


int exitCallback(int arg1, int arg2, void *common) 
{
    sceKernelExitGame();
    return 0;
}

int callbackThread(SceSize args, void *argp) 
{
    int cbid = sceKernelCreateCallback("Exit Callback", exitCallback, NULL);
    sceKernelRegisterExitCallback(cbid);
    sceKernelSleepThreadCB();
    return 0;
}


int setupCallbacks(void) 
{
    int thid = sceKernelCreateThread("update_thread", callbackThread, 0x11, 0xFA0, 0, 0);
    sceKernelStartThread(thid, 0, 0);
    return thid;
}

int main()
{
    setupCallbacks();
    pspDebugScreenInit();
    printf("DeadlyStagesDemo");
    sceKernelSleepThread();
}