#include <pspgu.h>
#include <pspgum.h>
#include <pspctrl.h>

#include "scene.h"

/* موقع اللاعب */
static float playerX = 0.0f;
static float playerY = 0.0f;

/* تعريف رأس مثلث */
typedef struct
{
    float x;
    float y;
    float z;
    unsigned int color;
} Vertex;

/* رسم مثلث */
static void drawTriangle(
    float x1, float y1,
    float x2, float y2,
    float x3, float y3,
    unsigned int color)
{
    Vertex vertices[3];

    vertices[0].x = x1;
    vertices[0].y = y1;
    vertices[0].z = 0.0f;
    vertices[0].color = color;

    vertices[1].x = x2;
    vertices[1].y = y2;
    vertices[1].z = 0.0f;
    vertices[1].color = color;

    vertices[2].x = x3;
    vertices[2].y = y3;
    vertices[2].z = 0.0f;
    vertices[2].color = color;

    sceGuDisable(GU_TEXTURE_2D);
    sceGuShadeModel(GU_FLAT);

    sceGuDrawArray(
        GU_TRIANGLES,
        GU_COLOR_8888 |
        GU_VERTEX_32BITF |
        GU_TRANSFORM_2D,
        3,
        NULL,
        vertices
    );
}

void sceneInit(void)
{
    playerX = 0.0f;
    playerY = 0.0f;
}

void sceneUpdate(unsigned int buttons)
{
    const float speed = 2.0f;

    if (buttons & PSP_CTRL_LEFT)
        playerX -= speed;

    if (buttons & PSP_CTRL_RIGHT)
        playerX += speed;

    if (buttons & PSP_CTRL_UP)
        playerY -= speed;

    if (buttons & PSP_CTRL_DOWN)
        playerY += speed;

    /* حدود الحركة */
    if (playerX < -220.0f)
        playerX = -220.0f;

    if (playerX > 220.0f)
        playerX = 220.0f;

    if (playerY < -120.0f)
        playerY = -120.0f;

    if (playerY > 120.0f)
        playerY = 120.0f;
}

void sceneDraw(void)
{
    /*
     * شخصية مؤقتة:
     * مثلث يمثل أوريتشي.
     */

    drawTriangle(
        playerX,
        playerY - 25.0f,

        playerX - 18.0f,
        playerY + 20.0f,

        playerX + 18.0f,
        playerY + 20.0f,

        0xFFFFFFFF
    );
}

int sceneIsFinished(void)
{
    return 0;
}
