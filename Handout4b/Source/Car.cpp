#include "Car.h"

Car::Car(ModulePhysics* physics, int _x, int _y, Module* _listener)
    : PhysicEntity(physics->CreateRectangle(_x, _y, 26, 43), _listener)
{
    texture = LoadTexture("Assets/Car.png");
    body->body->SetTransform(body->body->GetPosition(), b2_pi / 2);
    body->colliderType = ColliderType::CAR;
    listener = _listener;
    currentAcceleration = 0.0f;
    nitroActive = false;
    nitroTimeLeft = 0.0f;
    nitroCooldownTimeLeft = 0.0f;
    isSpinning = false;
    spinningTimeLeft = 0.0f;
    oilCooldownActive = false;
    oilCooldownTimeLeft = 0.0f;
    preSpinDirection.SetZero();
}

Car::~Car()
{
    UnloadTexture(texture);
}

void Car::Update()
{

    if (isSpinning) {
        spinningTimeLeft -= (1.0f / 60.0f);

        float remainingFraction = spinningTimeLeft / spinningDuration;
        float currentAngularVelocity = spinningAngularVelocity * remainingFraction;
        body->body->SetAngularVelocity(currentAngularVelocity);

        if (spinningTimeLeft <= 0.0f) {
            isSpinning = false;
            body->body->SetAngularVelocity(0.0f);

            if (preSpinDirection.Length() > 0.0f) {
                float angle = atan2(preSpinDirection.y, preSpinDirection.x);
                body->body->SetTransform(body->body->GetPosition(), angle);
            }
        }
    }

    if (oilCooldownActive) 
    {
        oilCooldownTimeLeft -= (1.0f / 60.0f);
        if (oilCooldownTimeLeft <= 0.0f)
        {
            oilCooldownActive = false;
        }
    }

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

    if (nitroActive) {
        nitroTimeLeft -= (1.0f / 60.0f);
        if (nitroTimeLeft <= 0.0f) {
            nitroActive = false;
        }
    }

    if (nitroCooldownTimeLeft > 0.0f) {
        nitroCooldownTimeLeft -= (1.0f / 60.0f);
    }

    float angularVelocity = body->body->GetAngularVelocity();
    if (fabs(angularVelocity) > 0.001f) {
        float angularFriction = 0.1f;
        float newAngularVelocity = angularVelocity - angularFriction * angularVelocity * (1.0f / 60.0f);

        if (fabs(newAngularVelocity) < 0.001f) {
            newAngularVelocity = 0.0f;
        }

        body->body->SetAngularVelocity(newAngularVelocity);
    }


}


bool Car::CleanUp()
{
    return true;
}

void Car::Accelerate() {
    if (isSpinning) return;

    const float maxSpeed = 8.5f;
    const float accelerationRate = 0.9f;

    currentAcceleration += accelerationRate;
    if (currentAcceleration > maxSpeed) {
        currentAcceleration = maxSpeed;
    }

    b2Vec2 direction = b2Vec2(sinf(body->GetRotation()), -cosf(body->GetRotation()));
    direction.Normalize();

	b2Vec2 newVelocity = b2Vec2(direction.x * currentAcceleration, direction.y * currentAcceleration);

    if (applyBoost) {
		b2Vec2 boost = b2Vec2(direction.x * 40.0f, direction.y * 40.0f);
        newVelocity += boost;
        applyBoost = false; 
    }

    body->body->SetLinearVelocity(newVelocity);
}





void Car::Brake()
{
    if (isSpinning) return;

    b2Vec2 velocity = body->body->GetLinearVelocity();
    const float brakingRate = 0.5f;

    b2Vec2 braking = b2Vec2(-velocity.x * brakingRate, -velocity.y * brakingRate);
    b2Vec2 newVelocity = b2Vec2(velocity.x + braking.x, velocity.y + braking.y);

    if (newVelocity.Length() < 0.1f)
    {
        newVelocity.SetZero();
    }

    body->body->SetLinearVelocity(newVelocity);
}


void Car::Turn(float direction, bool isTurning)
{
    if (isSpinning) return;

    const float maxAngularVelocity = 3.5f;
    const float angularFriction = 0.2f;

    if (isTurning)
    {
        body->body->SetAngularVelocity(maxAngularVelocity * direction);

        b2Vec2 currentVelocity = body->body->GetLinearVelocity();
        float speed = currentVelocity.Length();

        if (speed > 0.01f)
        {
            b2Vec2 newDirection = b2Vec2(sinf(body->GetRotation()), -cosf(body->GetRotation()));
            b2Vec2 newVelocity = b2Vec2(newDirection.x * speed, newDirection.y * speed);

            body->body->SetLinearVelocity(newVelocity);
        }
    }
    else
    {
        float angularVelocity = body->body->GetAngularVelocity();
        angularVelocity -= angularVelocity * angularFriction;

        if (fabs(angularVelocity) < 0.01f)
        {
            angularVelocity = 0.0f;
        }

        body->body->SetAngularVelocity(angularVelocity);
    }
}


void Car::Nitro()
{
    if (isSpinning) return;

    if (nitroCooldownTimeLeft <= 0.0f && !nitroActive) {
        nitroActive = true;
        nitroTimeLeft = nitroDuration;

        b2Vec2 direction = b2Vec2(sinf(body->GetRotation()), -cosf(body->GetRotation()));
        b2Vec2 currentVelocity = body->body->GetLinearVelocity();
        b2Vec2 nitroBoost = b2Vec2(direction.x * nitroFactor, direction.y * nitroFactor);

        body->body->SetLinearVelocity(currentVelocity + nitroBoost);

        nitroCooldownTimeLeft = nitroCooldown;


    }
}

void Car::ApplyBoost(float boostFactor) {

    b2Vec2 direction = b2Vec2(sinf(body->GetRotation()), -cosf(body->GetRotation()));
    direction.Normalize();

    b2Vec2 currentVelocity = body->body->GetLinearVelocity();

	b2Vec2 boost = b2Vec2(direction.x * boostFactor, direction.y * boostFactor);

    b2Vec2 newVelocity = currentVelocity + boost;

    body->body->SetLinearVelocity(newVelocity);
}


