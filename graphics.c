#include <pspdisplay.h>
#include <pspgu.h>
#include <pspgum.h>

#include "graphics.h"

#define BUF_WIDTH 512
#define SCR_WIDTH 480
#define SCR_HEIGHT 272

static unsigned int __attribute__((aligned(16))) displayList[262144];

void graphicsInit(void)
{
    sceGuInit();

    sceGuStart(GU_DIRECT, displayList);

    sceGuDrawBuffer(
        GU_PSM_8888,
        (void *)0,
        BUF_WIDTH
    );

    sceGuDispBuffer(
        SCR_WIDTH,
        SCR_HEIGHT,
        (void *)0x88000,
        BUF_WIDTH
    );

    sceGuDepthBuffer(
        (void *)0x110000,
        BUF_WIDTH
    );

    sceGuOffset(
        2048 - (SCR_WIDTH / 2),
        2048 - (SCR_HEIGHT / 2)
    );

    sceGuViewport(
        2048,
        2048,
        SCR_WIDTH,
        SCR_HEIGHT
    );

    sceGuDepthRange(65535, 0);

    sceGuScissor(
        0,
        0,
        SCR_WIDTH,
        SCR_HEIGHT
    );

    sceGuEnable(GU_SCISSOR_TEST);
    sceGuEnable(GU_DEPTH_TEST);

    sceGuDepthFunc(GU_GEQUAL);
    sceGuFrontFace(GU_CCW);
    sceGuShadeModel(GU_SMOOTH);

    sceGuFinish();
    sceGuSync(0, 0);

    sceDisplayWaitVblankStart();

    sceGuDisplay(GU_TRUE);
}

void graphicsStartFrame(void)
{
    sceGuStart(GU_DIRECT, displayList);

    sceGuClearColor(0xFF101018);
    sceGuClearDepth(0);

    sceGuClear(
        GU_COLOR_BUFFER_BIT |
        GU_DEPTH_BUFFER_BIT
    );
}

void graphicsEndFrame(void)
{
    sceGuFinish();
    sceGuSync(0, 0);

    sceDisplayWaitVblankStart();

    sceGuSwapBuffers();
}

void graphicsShutdown(void)
{
    sceGuTerm();
}
