#pragma once
#include "includes.h"
#include "game_object.h"

class Camera : public GameObject
{
private:
    vec3 m_Front;
    vec3 m_Up;
    vec3 m_WorldUp;

    float m_Yaw;
    float m_Pitch;
    float m_MovementSpeed;
    float m_MouseSensitivity;

public:
    Camera();
    Camera(vec3 position, vec3 up = vec3(0.0f, 1.0f, 0.0f),
           float yaw = -90.0f, float pitch = 0.0f);

    mat4 GetViewMatrix() const;
    void ProcessKeyboard(int direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

    // gets
    inline vec3 GetFront() const { return m_Front; }
    inline vec3 GetUp() const { return m_Up; }

    // sets
    inline void SetMovementSpeed(float speed) { m_MovementSpeed = speed; }

private:
    void updateCameraVectors();
};