#include <pspgu.h>
#include <pspgum.h>
#include <pspctrl.h>

#include "scene.h"

/*
 * ORITCHI GAME
 * Main Menu - PSP
 *
 * لا نستخدم صورًا أو ملفات خارجية في هذه المرحلة.
 * الخط مرسوم مباشرة باستخدام أشكال بسيطة حتى
 * يعمل المشروع بدون الحاجة إلى تحميل ملفات إضافية.
 */

static int menuSelection = 0;

/* ---------------------------------------------------------
   معلومات الخط البسيط 5x7
   --------------------------------------------------------- */

static const unsigned char font_O[7] = {
    0x0E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E
};

static const unsigned char font_R[7] = {
    0x1E, 0x11, 0x11, 0x1E, 0x14, 0x12, 0x11
};

static const unsigned char font_I[7] = {
    0x1F, 0x04, 0x04, 0x04, 0x04, 0x04, 0x1F
};

static const unsigned char font_T[7] = {
    0x1F, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04
};

static const unsigned char font_C[7] = {
    0x0F, 0x10, 0x10, 0x10, 0x10, 0x10, 0x0F
};

static const unsigned char font_H[7] = {
    0x11, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11
};

static const unsigned char font_G[7] = {
    0x0E, 0x11, 0x10, 0x17, 0x11, 0x11, 0x0E
};

static const unsigned char font_A[7] = {
    0x0E, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11
};

static const unsigned char font_M[7] = {
    0x11, 0x1B, 0x15, 0x15, 0x11, 0x11, 0x11
};

static const unsigned char font_E[7] = {
    0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x1F
};

static const unsigned char font_S[7] = {
    0x0F, 0x10, 0x10, 0x0E, 0x01, 0x01, 0x1E
};

static const unsigned char font_P[7] = {
    0x1E, 0x11, 0x11, 0x1E, 0x10, 0x10, 0x10
};

static const unsigned char font_N[7] = {
    0x11, 0x19, 0x15, 0x13, 0x11, 0x11, 0x11
};

static const unsigned char font_X[7] = {
    0x11, 0x11, 0x0A, 0x04, 0x0A, 0x11, 0x11
};

static const unsigned char font_SPACE[7] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/* ---------------------------------------------------------
   إرجاع شكل الحرف
   --------------------------------------------------------- */

static const unsigned char* getCharacter(char c)
{
    switch (c)
    {
        case 'O': return font_O;
        case 'R': return font_R;
        case 'I': return font_I;
        case 'T': return font_T;
        case 'C': return font_C;
        case 'H': return font_H;
        case 'G': return font_G;
        case 'A': return font_A;
        case 'M': return font_M;
        case 'E': return font_E;
        case 'S': return font_S;
        case 'P': return font_P;
        case 'N': return font_N;
        case 'X': return font_X;
        case ' ': return font_SPACE;
        default:  return font_SPACE;
    }
}

/* ---------------------------------------------------------
   رسم حرف واحد
   --------------------------------------------------------- */

static void drawCharacter(
    char character,
    float x,
    float y,
    float scale,
    unsigned int color)
{
    const unsigned char *glyph = getCharacter(character);

    typedef struct
    {
        float x;
        float y;
        float z;
        unsigned int color;
    } Vertex;

    Vertex vertices[35];

    int count = 0;

    int row;
    int column;

    for (row = 0; row < 7; row++)
    {
        for (column = 0; column < 5; column++)
        {
            if (glyph[row] & (1 << (4 - column)))
            {
                vertices[count].x = x + column * scale;
                vertices[count].y = y + row * scale;
                vertices[count].z = 0.0f;
                vertices[count].color = color;

                count++;
            }
        }
    }

    if (count == 0)
        return;

    sceGuDisable(GU_TEXTURE_2D);
    sceGuShadeModel(GU_FLAT);

    /*
     * كل نقطة تمثل مربعًا صغيرًا.
     * نستخدم GU_SPRITES، وكل زوج من الرؤوس يمثل مربعًا.
     */

    Vertex spriteVertices[70];

    int spriteCount = 0;

    for (row = 0; row < 7; row++)
    {
        for (column = 0; column < 5; column++)
        {
            if (glyph[row] & (1 << (4 - column)))
            {
                spriteVertices[spriteCount].x =
                    x + column * scale;

                spriteVertices[spriteCount].y =
                    y + row * scale;

                spriteVertices[spriteCount].z = 0.0f;
                spriteVertices[spriteCount].color = color;

                spriteCount++;

                spriteVertices[spriteCount].x =
                    x + column * scale + scale;

                spriteVertices[spriteCount].y =
                    y + row * scale + scale;

                spriteVertices[spriteCount].z = 0.0f;
                spriteVertices[spriteCount].color = color;

                spriteCount++;
            }
        }
    }

    sceGuDrawArray(
        GU_SPRITES,
        GU_COLOR_8888 |
        GU_VERTEX_32BITF |
        GU_TRANSFORM_2D,
        spriteCount,
        NULL,
        spriteVertices
    );
}

/* ---------------------------------------------------------
   رسم كلمة
   --------------------------------------------------------- */

static void drawText(
    const char *text,
    float x,
    float y,
    float scale,
    unsigned int color)
{
    float currentX = x;

    while (*text)
    {
        drawCharacter(
            *text,
            currentX,
            y,
            scale,
            color
        );

        currentX += scale * 6.0f;

        text++;
    }
}

/* ---------------------------------------------------------
   تهيئة المشهد
   --------------------------------------------------------- */

void sceneInit(void)
{
    menuSelection = 0;
}

/* ---------------------------------------------------------
   التحكم بالقائمة
   --------------------------------------------------------- */

void sceneUpdate(unsigned int buttons)
{
    static unsigned int oldButtons = 0;

    unsigned int pressed =
        buttons & ~oldButtons;

    if (pressed & PSP_CTRL_UP)
    {
        menuSelection--;

        if (menuSelection < 0)
            menuSelection = 2;
    }

    if (pressed & PSP_CTRL_DOWN)
    {
        menuSelection++;

        if (menuSelection > 2)
            menuSelection = 0;
    }

    /*
     * X حاليًا لا يبدأ اللعبة بعد.
     * سنربطه بمشهد الهروب في الخطوة التالية.
     */

    oldButtons = buttons;
}

/* ---------------------------------------------------------
   رسم القائمة الرئيسية
   --------------------------------------------------------- */

void sceneDraw(void)
{
    /*
     * العنوان
     */

    drawText(
        "ORITCHI",
        -105.0f,
        -105.0f,
        5.0f,
        0xFFFFFFFF
    );

    drawText(
        "GAME",
        -60.0f,
        -60.0f,
        5.0f,
        0xFFFFFFFF
    );

    /*
     * خيارات القائمة
     */

    unsigned int normalColor = 0xFFBBBBBB;
    unsigned int selectedColor = 0xFFFFFFFF;

    drawText(
        "START GAME",
        -105.0f,
        5.0f,
        3.0f,
        menuSelection == 0
            ? selectedColor
            : normalColor
    );

    drawText(
        "OPTIONS",
        -105.0f,
        35.0f,
        3.0f,
        menuSelection == 1
            ? selectedColor
            : normalColor
    );

    drawText(
        "EXIT",
        -105.0f,
        65.0f,
        3.0f,
        menuSelection == 2
            ? selectedColor
            : normalColor
    );

    /*
     * تعليمات التحكم
     */

    drawText(
        "PRESS X",
        -75.0f,
        105.0f,
        2.5f,
        0xFF888888
    );
}

/* ---------------------------------------------------------
   هل انتهى المشهد؟
   --------------------------------------------------------- */

int sceneIsFinished(void)
{
    return 0;
}
