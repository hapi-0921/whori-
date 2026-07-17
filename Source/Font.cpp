#include "Font.h"
#include <string>

Font::Font(const char* filename)
{
    sprite = new Sprite(filename);
}

Font::~Font()
{
    delete sprite;
}
void Font::Draw(
    const RenderContext& rc,
    const char* text,
    float x,
    float y,
    float scale)
{
    float drawX = x;
    float drawY = y;

    while (*text)
    {
        // 改行対応
        if (*text == '\n')
        {
            drawX = x;
            drawY += CHAR_H * scale;
            text++;
            continue;
        }

        unsigned char c = (unsigned char)(*text);

        int index = c - 32;

        // 範囲外（コントロール文字など）や負の値のセーフティガード
        if (index < 0) index = 0;

        int sx = (index % 16) * CHAR_W;
        int sy = (index / 16) * CHAR_H;

        sprite->Render(
            rc,
            drawX,
            drawY,
            0,

            CHAR_W * scale,
            CHAR_H * scale,

            (float)sx,
            (float)sy,

            CHAR_W,
            CHAR_H,

            0,

            1, 1, 1, 1);

        drawX += CHAR_W * scale;

        text++;
    }
}
void Font::Draw(
    const RenderContext& rc,
    int value,
    float x,
    float y,
    float scale)
{
    std::string text = std::to_string(value);
    Draw(rc, text.c_str(), x, y, scale);
}

void Font::DrawNumber(const RenderContext& rc, int number, float centerX, float y, float scale)
{
    char text[16];
    sprintf_s(text, "%d", number);

    int len = strlen(text);

    float drawX = centerX - (len * NUM_W * scale) * 0.5f;

    for (int i = 0; i < len; i++)
    {
        int digit = text[i] - '0';

        for (int i = 0; i < 10; i++)
{
    sprite->Render(
        rc,
        i * 100,
        100,
        0,

        86,
        86,

        i * NUM_W,
        0,

        NUM_W,
        NUM_H,

        0,
        1,1,1,1);
}

        drawX += NUM_W * scale;
    }
}