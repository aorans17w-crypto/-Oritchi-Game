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

int main(void)
{
    SceCtrlData pad;

    pspDebugScreenInit();

    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

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

    float playerX = 0.0f;
    float playerZ = 0.0f;

    while (1)
    {
        sceCtrlReadBufferPositive(&pad, 1);

        if (pad.Buttons & PSP_CTRL_START)
        {
            break;
        }

        if (pad.Buttons & PSP_CTRL_LEFT)
            playerX -= 0.03f;

        if (pad.Buttons & PSP_CTRL_RIGHT)
            playerX += 0.03f;

        if (pad.Buttons & PSP_CTRL_UP)
            playerZ += 0.03f;

        if (pad.Buttons & PSP_CTRL_DOWN)
            playerZ -= 0.03f;

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

        ScePspFVector3 camera = {
            0.0f,
            3.0f,
            -6.0f
        };

        ScePspFVector3 target = {
            0.0f,
            0.0f,
            0.0f
        };

        ScePspFVector3 up = {
            0.0f,
            1.0f,
            0.0f
        };

        sceGumLookAt(
            &camera,
            &target,
            &up
        );

        /*
         * أرضية
         */

        sceGumMatrixMode(GU_MODEL);
        sceGumLoadIdentity();

        sceGumTranslate(
            &(ScePspFVector3){0.0f, -1.0f, 0.0f}
        );

        sceGuColor(0xFF406040);

        sceGuDisable(GU_TEXTURE_2D);

        /*
         * مربع أرضي
         */

        typedef struct {
            float x;
            float y;
            float z;
        } Vertex;

        Vertex floorVertices[4] = {
            {-5.0f, 0.0f, -5.0f},
            { 5.0f, 0.0f, -5.0f},
            {-5.0f, 0.0f,  5.0f},
            { 5.0f, 0.0f,  5.0f}
        };

        sceGumDrawArray(
            GU_TRIANGLE_STRIP,
            GU_VERTEX_32BITF |
            GU_TRANSFORM_3D,
            4,
            0,
            floorVertices
        );

        /*
         * اللاعب التجريبي
         */

        sceGumPushMatrix();

        ScePspFVector3 player = {
            playerX,
            0.0f,
            playerZ
        };

        sceGumTranslate(&player);

        sceGuColor(0xFFCCAA66);

        Vertex playerVertices[4] = {
            {-0.4f, 0.0f,  0.0f},
            { 0.4f, 0.0f,  0.0f},
            {-0.4f, 1.0f,  0.0f},
            { 0.4f, 1.0f,  0.0f}
        };

        sceGumDrawArray(
            GU_TRIANGLE_STRIP,
            GU_VERTEX_32BITF |
            GU_TRANSFORM_3D,
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

    sceGuTerm();

    sceKernelExitGame();

    return 0;
}
