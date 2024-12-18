#pragma once

#include "Module.h"
#include "Globals.h"
#include "raylib.h"
#include <vector>

enum class ParticleType {
    SMOKE,
    NITRO
};

struct Particle {
    Vector2 position;
    Vector2 velocity;
    Color color;
    float lifetime; 
    float size;     
};

class ModuleParticle : public Module {
public:
    ModuleParticle(Application* app, bool start_enabled = true);
    ~ModuleParticle();

    bool Start() override;
    update_status Update() override;
    bool CleanUp() override;

    void SpawnParticles(Vector2 position, ParticleType type);

private:
    std::vector<Particle> particles;

    void UpdateParticles();
    void DrawParticles();
};
