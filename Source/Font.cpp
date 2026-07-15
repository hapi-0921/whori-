#include "Font.h"

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