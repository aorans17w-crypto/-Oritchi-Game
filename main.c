#include <pspkernel.h>
#include <pspdebug.h>

PSP_MODULE_INFO("Oritchi Game", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

int main(void)
{
    pspDebugScreenInit();

    pspDebugScreenPrintf("ORITCHI GAME\n");
    pspDebugScreenPrintf("PSP BUILD TEST SUCCESS!\n");

    sceKernelSleepThread();

    return 0;
}
