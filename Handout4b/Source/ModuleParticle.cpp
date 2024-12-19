#include "ModuleParticle.h"
#include "Application.h"

ModuleParticle::ModuleParticle(Application* app, bool start_enabled) : Module(app, start_enabled) {
}

ModuleParticle::~ModuleParticle() {}

bool ModuleParticle::Start() {
    LOG("Starting Particle System");
    return true;
}

update_status ModuleParticle::Update() {
    UpdateParticles();
    DrawParticles();
    return UPDATE_CONTINUE;
}

bool ModuleParticle::CleanUp() {
    LOG("Cleaning up Particle System");
    particles.clear();
    return true;
}

void ModuleParticle::SpawnParticles(Vector2 position, ParticleType type) {
    const int numParticles = 20; 

    for (int i = 0; i < numParticles; ++i) {
        Particle particle;
        particle.position = position;

        particle.velocity.x = GetRandomValue(-50, 50) / 100.0f;
        particle.velocity.y = GetRandomValue(-50, 50) / 100.0f;

        switch (type) {
        case ParticleType::SMOKE:
            particle.color = GRAY;
            particle.lifetime = 2.5f;
            particle.size = 14;
            break;
        case ParticleType::NITRO:
            particle.color = ORANGE;
            particle.lifetime = 2.5f;
            particle.size = 12;
            break;
        }

        particles.push_back(particle);
    }
}

void ModuleParticle::UpdateParticles() {
    for (auto it = particles.begin(); it != particles.end();) {
        it->lifetime -= 1.0f / 60.0f; 
        if (it->lifetime <= 0) {
            it = particles.erase(it); 
        }
        else {
            it->position.x += it->velocity.x;
            it->position.y += it->velocity.y;

            it->size *= 0.98f;

            ++it;
        }
    }
}

void ModuleParticle::DrawParticles() {
    for (const Particle& particle : particles) {
        DrawCircleV(particle.position, particle.size, particle.color);
    }
}
