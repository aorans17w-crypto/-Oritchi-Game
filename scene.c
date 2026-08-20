#include <pspgu.h>
#include <pspgum.h>
#include <pspctrl.h>

#include "scene.h"

static float playerX = 0.0f;
static float playerY = 0.0f;

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

    /* حدود الشاشة */
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
    /* سنضع الرسم الحقيقي للشخصية والبيئة هنا لاحقًا */
}

int sceneIsFinished(void)
{
    return 0;
}
