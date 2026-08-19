#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>

PSP_MODULE_INFO("Oritchi Game", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

int main(void)
{
    SceCtrlData pad;

    pspDebugScreenInit();
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_DIGITAL);

    int selected = 0;

    while (1)
    {
        pspDebugScreenClear();

        pspDebugScreenSetXY(8, 5);
        pspDebugScreenPrintf("ORITCHI GAME");

        pspDebugScreenSetXY(8, 9);

        if (selected == 0)
            pspDebugScreenPrintf("> START GAME");
        else
            pspDebugScreenPrintf("  START GAME");

        pspDebugScreenSetXY(8, 11);

        if (selected == 1)
            pspDebugScreenPrintf("> EXIT");
        else
            pspDebugScreenPrintf("  EXIT");

        pspDebugScreenSetXY(8, 16);
        pspDebugScreenPrintf("UP / DOWN : SELECT");
        pspDebugScreenSetXY(8, 17);
        pspDebugScreenPrintf("X : CONFIRM");

        sceCtrlReadBufferPositive(&pad, 1);

        if (pad.Buttons & PSP_CTRL_UP)
        {
            selected = 0;
        }

        if (pad.Buttons & PSP_CTRL_DOWN)
        {
            selected = 1;
        }

        if (pad.Buttons & PSP_CTRL_CROSS)
        {
            if (selected == 0)
            {
                pspDebugScreenClear();

                pspDebugScreenSetXY(7, 8);
                pspDebugScreenPrintf("ORITCHI GAME");

                pspDebugScreenSetXY(7, 10);
                pspDebugScreenPrintf("GAME STARTED!");

                pspDebugScreenSetXY(7, 13);
                pspDebugScreenPrintf("Press TRIANGLE to return.");

                while (1)
                {
                    sceCtrlReadBufferPositive(&pad, 1);

                    if (pad.Buttons & PSP_CTRL_TRIANGLE)
                        break;

                    sceKernelDelayThread(16000);
                }
            }
            else
            {
               
