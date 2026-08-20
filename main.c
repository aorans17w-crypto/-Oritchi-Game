#include <pspkernel.h>
#include <pspctrl.h>

#include "graphics.h"
#include "scene.h"

PSP_MODULE_INFO("Oritchi Game", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

int main(void)
{
    SceCtrlData pad;

    graphicsInit();
    sceneInit();

    while (1)
    {
        sceCtrlReadBufferPositive(&pad, 1);

        if (pad.Buttons & PSP_CTRL_START)
        {
            break;
        }

        sceneUpdate(pad.Buttons);

        graphicsStartFrame();

        sceneDraw();

        graphicsEndFrame();

        sceKernelDelayThread(16000);
    }

    graphicsShutdown();

    sceKernelExitGame();

    return 0;
}
