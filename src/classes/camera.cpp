#include "camera.h"
#include "includes.h"

Camera::Camera()
    : GameObject(),
      m_Front(vec3(0.0f, 0.0f, -1.0f)),
      m_WorldUp(vec3(0.0f, 1.0f, 0.0f)),
      m_Yaw(-90.0f),
      m_Pitch(0.0f),
      m_MovementSpeed(2.5f),
      m_MouseSensitivity(0.1f)
{
    updateCameraVectors();
}

Camera::Camera(vec3 position, vec3 up, float yaw, float pitch)
    : GameObject(),
      m_Front(vec3(0.0f, 0.0f, -1.0f)),
      m_WorldUp(up),
      m_Yaw(yaw),
      m_Pitch(pitch),
      m_MovementSpeed(2.5f),
      m_MouseSensitivity(0.1f)
{
    m_Transform.SetPosition(position);
    updateCameraVectors();
}

mat4 Camera::GetViewMatrix() const
{
    vec3 worldPosition = m_Transform.GetWorldPosition();
    return lookAt(worldPosition, worldPosition + m_Front, m_Up);
}

void Camera::ProcessKeyboard(int direction, float deltaTime)
{
    float velocity = m_MovementSpeed * deltaTime;

    switch (direction)
    {
    case GLFW_KEY_W:
        m_Transform.Translate(m_Front * velocity);
        break;
    case GLFW_KEY_S:
        m_Transform.Translate(-m_Front * velocity);
        break;
    case GLFW_KEY_A:
        m_Transform.Translate(-normalize(cross(m_Front, m_Up)) * velocity);
        break;
    case GLFW_KEY_D:
        m_Transform.Translate(normalize(cross(m_Front, m_Up)) * velocity);
        break;
    case GLFW_KEY_SPACE:
        m_Transform.Translate(m_Up * velocity);
        break;
    case GLFW_KEY_C:
        m_Transform.Translate(-m_Up * velocity);
        break;
    }
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
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

    updateCameraVectors();
}

void Camera::updateCameraVectors()
{
    vec3 front;
    front.x = cos(radians(m_Yaw)) * cos(radians(m_Pitch));
    front.y = sin(radians(m_Pitch));
    front.z = sin(radians(m_Yaw)) * cos(radians(m_Pitch));
    m_Front = normalize(front);

    m_Up = normalize(cross(m_Front, cross(m_WorldUp, m_Front)));
}