#include <pspgu.h>
#include <pspctrl.h>

#include "scene.h"

/*
 * Oritchi Game
 * Main Menu - First Prototype
 *
 * PSP resolution: 480x272
 */

/* حالات اللعبة */
#define STATE_MENU 0
#define STATE_GAME 1

static int gameState = STATE_MENU;
static int menuSelection = 0;

/*
 * ألوان
 */
#define COLOR_WHITE   0xFFFFFFFF
#define COLOR_BLACK   0xFF000000
#define COLOR_RED     0xFFB02030
#define COLOR_DARKRED 0xFF401018
#define COLOR_GRAY    0xFF808080

/*
 * Vertex بسيط للرسم ثنائي الأبعاد
 */
typedef struct
{
    float x;
    float y;
    float z;
    unsigned int color;
} Vertex;


/*
 * رسم مستطيل
 */
static void drawRectangle(
    float x,
    float y,
    float width,
    float height,
    unsigned int color)
{
    Vertex vertices[2];

    vertices[0].x = x;
    vertices[0].y = y;
    vertices[0].z = 0.0f;
    vertices[0].color = color;

    vertices[1].x = x + width;
    vertices[1].y = y + height;
    vertices[1].z = 0.0f;
    vertices[1].color = color;

    sceGuDisable(GU_TEXTURE_2D);
    sceGuShadeModel(GU_FLAT);

    /*
     * رسم مستطيل باستخدام Sprite
     */
    sceGuDrawArray(
        GU_SPRITES,
        GU_COLOR_8888 |
        GU_VERTEX_32BITF |
        GU_TRANSFORM_2D,
        2,
        NULL,
        vertices
    );
}


/*
 * رسم مؤشر القائمة
 */
static void drawMenuCursor(float y)
{
    drawRectangle(
        105.0f,
        y,
        10.0f,
        20.0f,
        COLOR_RED
    );
}


/*
 * تهيئة المشهد
 */
void sceneInit(void)
{
    gameState = STATE_MENU;
    menuSelection = 0;
}


/*
 * تحديث اللعبة
 */
void sceneUpdate(unsigned int buttons)
{
    /*
     * القائمة الرئيسية
     */
    if (gameState == STATE_MENU)
    {
        if (buttons & PSP_CTRL_UP)
        {
            if (menuSelection > 0)
                menuSelection--;
        }

        if (buttons & PSP_CTRL_DOWN)
        {
            if (menuSelection < 2)
                menuSelection++;
        }

        /*
         * X = اختيار
         */
        if (buttons & PSP_CTRL_CROSS)
        {
            if (menuSelection == 0)
            {
                /*
                 * START GAME
                 */
                gameState = STATE_GAME;
            }
            else if (menuSelection == 1)
            {
                /*
                 * OPTIONS
                 *
                 * سيتم تطويرها لاحقًا.
                 */
            }
            else if (menuSelection == 2)
            {
                /*
                 * EXIT
                 *
                 * العودة للقائمة حاليًا.
                 */
            }
        }
    }
}


/*
 * رسم النص بشكل مؤقت
 *
 * ملاحظة:
 * لن نستخدم Font خارجي الآن.
 * سنضيف نظام الخط الحقيقي لاحقًا.
 */


/*
 * رسم شاشة القائمة
 */
static void drawMenu(void)
{
    /*
     * خلفية
     */
    drawRectangle(
        0.0f,
        0.0f,
        480.0f,
        272.0f,
        COLOR_BLACK
    );

    /*
     * شريط علوي
     */
    drawRectangle(
        0.0f,
        0.0f,
        480.0f,
        8.0f,
        COLOR_RED
    );

    /*
     * شريط سفلي
     */
    drawRectangle(
        0.0f,
        264.0f,
        480.0f,
        8.0f,
        COLOR_DARKRED
    );

    /*
     * عنوان مؤقت
     *
     * سنضع الخط الحقيقي هنا في الخطوة القادمة.
     */

    /*
     * عناصر القائمة
     */

    drawRectangle(
        135.0f,
        105.0f,
        210.0f,
        28.0f,
        COLOR_DARKRED
    );

    drawRectangle(
        135.0f,
        145.0f,
        210.0f,
        28.0f,
        COLOR_DARKRED
    );

    drawRectangle(
        135.0f,
        185.0f,
        210.0f,
        28.0f,
        COLOR_DARKRED
    );

    /*
     * المؤشر
     */
    if (menuSelection == 0)
        drawMenuCursor(109.0f);

    if (menuSelection == 1)
        drawMenuCursor(149.0f);

    if (menuSelection == 2)
        drawMenuCursor(189.0f);
}


/*
 * رسم أول مشهد لعب
 */
static void drawGame(void)
{
    /*
     * خلفية المشهد
     */
    drawRectangle(
        0.0f,
        0.0f,
        480.0f,
        272.0f,
        0xFF202030
    );

    /*
     * أرضية
     */
    drawRectangle(
        0.0f,
        200.0f,
        480.0f,
        72.0f,
        0xFF303020
    );

    /*
     * اللاعب - مؤقت
     */
    drawRectangle(
        225.0f,
        155.0f,
        30.0f,
        45.0f,
        COLOR_WHITE
    );
}


/*
 * رسم المشهد
 */
void sceneDraw(void)
{
    if (gameState == STATE_MENU)
    {
        drawMenu();
    }
    else
    {
        drawGame();
    }
}


/*
 * هل انتهى المشهد؟
 */
int sceneIsFinished(void)
{
    return 0;
}
