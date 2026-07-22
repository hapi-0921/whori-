#pragma once
#include<Vector>
#include <System/Sprite.h>
#include <System/RenderContext.h>
#include <DirectXMath.h>
class ParticleManager
{
    struct Particle
    {
        DirectX::XMFLOAT2 pos;      // 位置
        DirectX::XMFLOAT2 vel;      // 速度
        float life;       // 残り時間
        float maxLife;    // 初期寿命
        float scale;      // 大きさ
        float angle;      // 回転
        float rotate;     // 回転速度
        float alpha;      // 透明度
        bool active;
        Sprite* sprite;
    };
public:
    void Initialize();
    void Update(float dt);
    void Draw(RenderContext& rc);

    void Emit(DirectX::XMFLOAT2 pos, int count, Sprite* useSprite);

private:
    std::vector<Particle> particles;
    Sprite* starSprite;
    Sprite* heartSprite;
    Sprite* smokeSprite;
};