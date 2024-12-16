#include "Car.h"

Car::Car(ModulePhysics* physics, int _x, int _y, Module* _listener)
    : PhysicEntity(physics->CreateRectangle(_x, _y, 26, 43), _listener)
{
    texture = LoadTexture("Assets/Car.png");
}

Car::~Car()
{
    UnloadTexture(texture);
}

void Car::Update()
{
    int x, y;
    body->GetPhysicPosition(x, y);

    b2Vec2 velocity = body->body->GetLinearVelocity();
    b2Vec2 dir = velocity;

    float speed = velocity.Length();
    if (speed > 0.001f) {
        dir.Normalize();

        
        float mass = body->body->GetMass();
        float N = mass * 9.8f; 
        float dynamicFrictionCoeff = 0.2f;
        float frictionMagnitude = N * dynamicFrictionCoeff;

        b2Vec2 friction = b2Vec2(-dir.x * frictionMagnitude, -dir.y * frictionMagnitude);

        b2Vec2 newVelocity = b2Vec2(velocity.x + friction.x * (1.0f / 60.0f),
            velocity.y + friction.y * (1.0f / 60.0f)); 
        if (newVelocity.Length() < 0.001f) {
            newVelocity.SetZero();
        }
        body->body->SetLinearVelocity(newVelocity);
    }
}

bool Car::CleanUp()
{
    return true;
}

void Car::Accelerate()
{
    const float maxSpeed = 10.0f;
    const float accelerationRate = 0.2f;

    currentAcceleration += accelerationRate;
    if (currentAcceleration > maxSpeed) {
        currentAcceleration = maxSpeed;
    }

    b2Vec2 direction = b2Vec2(cosf(body->GetRotation()), sinf(body->GetRotation()));
    b2Vec2 newVelocity = b2Vec2(direction.x * currentAcceleration, direction.y * currentAcceleration);

    body->body->SetLinearVelocity(newVelocity);
}

void Car::Brake()
{
    b2Vec2 velocity = body->body->GetLinearVelocity();
    const float brakingRate = 0.5f;

    b2Vec2 braking = b2Vec2(-velocity.x * brakingRate, -velocity.y * brakingRate);
    b2Vec2 newVelocity = b2Vec2(velocity.x + braking.x, velocity.y + braking.y);

    if (newVelocity.Length() < 0.1f) {
        newVelocity.SetZero();
    }

    body->body->SetLinearVelocity(newVelocity);
}

void Car::Turn(float direction)
{
    const float maxAngularVelocity = 3.0f;
    body->body->SetAngularVelocity(maxAngularVelocity * direction);
}
