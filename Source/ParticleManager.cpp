#include"ParticleManager.h"
void ParticleManager::Emit(DirectX::XMFLOAT2 pos, int count, Sprite* useSprite)
{
    for (int i = 0; i < count; i++)
    {
        Particle p;

        p.active = true;
        p.pos = pos;

        float angle = (rand() % 360) * 3.14159f / 180.0f;
        float speed = (rand() % 150 + 50);

        p.vel.x = cos(angle) * speed;
        p.vel.y = sin(angle) * speed;

        p.life = 1.0f;
        p.maxLife = 1.0f;

        p.scale = 0.8f + (rand() % 40) / 100.0f;

        p.alpha = 1.0f;

        p.rotate = (rand() % 200 - 100);
        p.sprite = useSprite;
        particles.push_back(p);
    }
}
void ParticleManager::Initialize()
{
    starSprite = new Sprite("Data/Sprite/star.png");
    //heartSprite = new Sprite("Data/Sprite/heart.png");
    //smokeSprite = new Sprite("Data/Sprite/smoke.png");

}
void ParticleManager::Update(float dt)
{
    for (auto& p : particles)
    {
        if (!p.active)
            continue;

        p.life -= dt;

        if (p.life <= 0)
        {
            p.active = false;
            continue;
        }

        p.pos.x += p.vel.x * dt;
        p.pos.y += p.vel.y * dt;
        // ­‚µŒ¸‘¬
        p.vel.x *= 0.96f;
        p.vel.x *= 0.96f;

        // ã‚É•‚‚©‚¹‚½‚¢‚È‚ç
        p.vel.y -= 20 * dt;

        p.angle += p.rotate * dt;

        p.alpha = p.life / p.maxLife;
    }
}
void ParticleManager::Draw(RenderContext& rc)
{
    for (auto& p : particles)
    {
        if (!p.active)
            continue;

        p.sprite->Render(
            rc,
            p.pos.x,
            p.pos.y,
            0,
            32 * p.scale,
            32 * p.scale,
            0, 0, 32, 32,
            p.angle,
            1, 1, 1, p.alpha
        );
    }
}