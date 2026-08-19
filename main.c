#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspgu.h>
#include <pspgum.h>

PSP_MODULE_INFO("Oritchi Game", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

#define BUF_WIDTH 512
#define SCR_WIDTH 480
#define SCR_HEIGHT 272

static unsigned int __attribute__((aligned(16))) list[262144];

typedef struct
{
    float x;
    float y;
    float z;
} Vertex;

static Vertex floorVertices[4] =
{
    {-5.0f, 0.0f, -5.0f},
    { 5.0f, 0.0f, -5.0f},
    {-5.0f, 0.0f,  5.0f},
    { 5.0f, 0.0f,  5.0f}
};

static Vertex playerVertices[4] =
{
    {-0.4f, 0.0f, 0.0f},
    { 0.4f, 0.0f, 0.0f},
    {-0.4f, 1.0f, 0.0f},
    { 0.4f, 1.0f, 0.0f}
};

void drawMenu(int selected)
{
    pspDebugScreenClear();

    pspDebugScreenSetXY(15, 4);
    pspDebugScreenPrintf("ORITCHI GAME");

    pspDebugScreenSetXY(15, 9);

    if (selected == 0)
    {
        pspDebugScreenPrintf("> NEW GAME");
    }
    else
    {
        pspDebugScreenPrintf("  NEW GAME");
    }

    pspDebugScreenSetXY(15, 11);

    if (selected == 1)
    {
        pspDebugScreenPrintf("> EXIT");
    }
    else
    {
        pspDebugScreenPrintf("  EXIT");
    }

    pspDebugScreenSetXY(15, 17);
    pspDebugScreenPrintf("UP / DOWN : SELECT");

    pspDebugScreenSetXY(15, 18);
    pspDebugScreenPrintf("X : CONFIRM");
}

void drawGame(float playerX, float playerZ)
{
    sceGuStart(GU_DIRECT, list);

    sceGuClearColor(0xFF202020);
    sceGuClearDepth(0);

    sceGuClear(
        GU_COLOR_BUFFER_BIT |
        GU_DEPTH_BUFFER_BIT
    );

    sceGumMatrixMode(GU_PROJECTION);
    sceGumLoadIdentity();

    sceGumPerspective(
        60.0f,
        16.0f / 9.0f,
        0.5f,
        100.0f
    );

    sceGumMatrixMode(GU_VIEW);
    sceGumLoadIdentity();

    ScePspFVector3 camera =
    {
        0.0f,
        3.0f,
        -6.0f
    };

    ScePspFVector3 target =
    {
        0.0f,
        0.0f,
        0.0f
    };

    ScePspFVector3 up =
    {
        0.0f,
        1.0f,
        0.0f
    };

    sceGumLookAt(
        &camera,
        &target,
        &up
    );

    sceGumMatrixMode(GU_MODEL);
    sceGumLoadIdentity();

    sceGuColor(0xFF406040);
    sceGuDisable(GU_TEXTURE_2D);

    sceGumDrawArray(
        GU_TRIANGLE_STRIP,
        GU_VERTEX_32BITF | GU_TRANSFORM_3D,
        4,
        0,
        floorVertices
    );

    sceGumPushMatrix();

    ScePspFVector3 player =
    {
        playerX,
        0.0f,
        playerZ
    };

    sceGumTranslate(&player);

    sceGuColor(0xFFCCAA66);

    sceGumDrawArray(
        GU_TRIANGLE_STRIP,
        GU_VERTEX_32BITF | GU_TRANSFORM_3D,
        4,
        0,
        playerVertices
    );

    sceGumPopMatrix();

    sceGuFinish();
    sceGuSync(0, 0);

    sceDisplayWaitVblankStart();
    sceGuSwapBuffers();
}

int main(void)
{
    SceCtrlData pad;

    int selected = 0;
    int gameStarted = 0;

    float playerX = 0.0f;
    float playerZ = 0.0f;

    pspDebugScreenInit();

    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_DIGITAL);

    sceGuInit();

    sceGuStart(GU_DIRECT, list);

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

    while (1)
    {
        sceCtrlReadBufferPositive(&pad, 1);

        if (!gameStarted)
        {
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
                    gameStarted = 1;
                    playerX = 0.0f;
                    playerZ = 0.0f;
                }
                else
                {
                    break;
                }
            }

            drawMenu(selected);
        }
        else
        {
            if (pad.Buttons & PSP_CTRL_LEFT)
            {
                playerX += 0.03f;
            }

            if (pad.Buttons & PSP_CTRL_RIGHT)
            {
                playerX -= 0.03f;
            }

            if (pad.Buttons & PSP_CTRL_UP)
            {
                playerZ += 0.03f;
            }

            if (pad.Buttons & PSP_CTRL_DOWN)
            {
                playerZ -= 0.03f;
            }

            if (pad.Buttons & PSP_CTRL_START)
            {
                gameStarted = 0;
                selected = 0;
            }

            drawGame(playerX, playerZ);
        }

        sceKernelDelayThread(16000);
    }

    sceGuTerm();

    sceKernelExitGame();

    return 0;
}
