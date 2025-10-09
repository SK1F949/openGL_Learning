#include "includes.h"
#include "test_object.h"

TestObject::TestObject()
    : GameObject(),
      m_Front(vec3(0.0f, 0.0f, -1.0f)),
      m_WorldUp(vec3(0.0f, 1.0f, 0.0f)),
      m_Yaw(360.0f),
      m_Pitch(0.0f),
      m_MovementSpeed(2.5f),
      m_MouseSensitivity(0.1f)
{
    updateRotateVectors();
}

TestObject::TestObject(vec3 position, vec3 up, float yaw, float pitch)
    : GameObject(),
      m_Front(vec3(0.0f, 0.0f, -1.0f)),
      m_WorldUp(up),
      m_Yaw(yaw),
      m_Pitch(pitch),
      m_MovementSpeed(2.5f),
      m_MouseSensitivity(0.1f)
{
    m_Transform.SetPosition(position);
    updateRotateVectors();
}

mat4 TestObject::GetViewMatrix() const
{
    vec3 worldPosition = m_Transform.GetWorldPosition();
    return lookAt(worldPosition, worldPosition + m_Front, m_Up);
}

void TestObject::ProcessKeyboard(int direction, float deltaTime)
{
    float velocity = m_MovementSpeed * deltaTime;

    switch (direction)
    {
    case GLFW_KEY_UP:
        m_Transform.SetRotation(m_Front * velocity);
        break;
    case GLFW_KEY_DOWN:
        m_Transform.SetRotation(-m_Front * velocity);
        break;
    case GLFW_KEY_LEFT:
        m_Transform.SetRotation(-normalize(cross(m_Front, m_Up)) * velocity);
        break;
    case GLFW_KEY_RIGHT:
        m_Transform.SetRotation(normalize(cross(m_Front, m_Up)) * velocity);
    }

    updateRotateVectors();
}

void TestObject::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= m_MouseSensitivity;
    yoffset *= m_MouseSensitivity;

    m_Yaw += xoffset;
    m_Pitch += yoffset;

    if (constrainPitch)
    {
        if (m_Pitch > 89.0f)
            m_Pitch = 89.0f;
        if (m_Pitch < -89.0f)
            m_Pitch = -89.0f;
    }

    updateRotateVectors();
}

void TestObject::updateRotateVectors()
{
    vec3 front;
    front.x = cos(radians(m_Yaw)) * cos(radians(m_Pitch));
    front.y = sin(radians(m_Pitch));
    front.z = sin(radians(m_Yaw)) * cos(radians(m_Pitch));
    m_Front = normalize(front);

    m_Up = normalize(cross(m_Front, cross(m_WorldUp, m_Front)));
}