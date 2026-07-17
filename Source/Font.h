#pragma once

#include "System/Sprite.h"

class Font
{
public:

    Font(const char* filename);
    ~Font();

    void Draw(
        const RenderContext& rc,
        const char* text,
        float x,
        float y,
        float scale = 1.0f);
    void Draw(
        const RenderContext& rc,
        int value,
        float x,
        float y,
        float scale);
    void DrawNumber(
        const RenderContext& rc,
        int number,
        float x,
        float y,
        float scale);
    void DrawNumber0(
        const RenderContext& rc,
        int number,
        float x,
        float y,
        float scale);
private:

    Sprite* sprite = nullptr;

    static constexpr int CHAR_W = 32;
    static constexpr int CHAR_H = 32;
    static constexpr int COLS = 16;

    static constexpr int NUM_W = 42;
    static constexpr int NUM_H = 42;
};