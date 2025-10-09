#include "../include/object_transform.h"

ObjectTransform::ObjectTransform()
    : m_LocalPosition(0.0f), m_LocalRotation(0.0f), m_LocalScale(1.0f),
      m_WorldPosition(0.0f), m_WorldRotation(0.0f), m_WorldScale(1.0f),
      m_Parent(nullptr), m_IsDirty(true)
{
}

ObjectTransform::ObjectTransform(vec3 position, vec3 scale)
    : m_LocalPosition(position), m_LocalRotation(0.0f), m_LocalScale(scale),
      m_WorldPosition(position), m_WorldRotation(0.0f), m_WorldScale(scale),
      m_Parent(nullptr), m_IsDirty(true)
{
}

mat4 ObjectTransform::GetModelMatrix() const
{
    mat4 model = mat4(1.0f);
    model = translate(model, m_LocalPosition);
    model = rotate(model, radians(m_LocalRotation.x), vec3(1.0f, 0.0f, 0.0f));
    model = rotate(model, radians(m_LocalRotation.y), vec3(0.0f, 1.0f, 0.0f));
    model = rotate(model, radians(m_LocalRotation.z), vec3(0.0f, 0.0f, 1.0f));
    model = scale(model, m_LocalScale);
    return model;
}

void ObjectTransform::UpdateWorldTransform() const
{
    if (m_Parent)
    {
        // Если есть родитель, вычисляем мировые координаты на основе родительских
        vec3 parentWorldPos = m_Parent->GetWorldPosition();
        vec3 parentWorldRot = m_Parent->GetWorldRotation();
        vec3 parentWorldScale = m_Parent->GetWorldScale();

        m_WorldPosition = parentWorldPos + m_LocalPosition;
        m_WorldRotation = parentWorldRot + m_LocalRotation;
        m_WorldScale = parentWorldScale * m_LocalScale;
    }
    else
    {
        // Если нет родителя, локальные и мировые координаты совпадают
        m_WorldPosition = m_LocalPosition;
        m_WorldRotation = m_LocalRotation;
        m_WorldScale = m_LocalScale;
    }
    m_IsDirty = false;
}

void ObjectTransform::Translate(const vec3 &translation)
{
    m_LocalPosition += translation;
    m_IsDirty = true;
}

void ObjectTransform::Rotate(const vec3 &rotation)
{
    m_LocalRotation += rotation;
    m_IsDirty = true;
}

void ObjectTransform::SetPosition(const vec3 &position)
{
    m_LocalPosition = position;
    m_IsDirty = true;
}

void ObjectTransform::SetRotation(const vec3 &rotation)
{
    m_LocalRotation = rotation;
    m_IsDirty = true;
}

void ObjectTransform::SetScale(const vec3 &scale)
{
    m_LocalScale = scale;
    m_IsDirty = true;
}

void ObjectTransform::SetParent(std::shared_ptr<ObjectTransform> parent)
{
    m_Parent = parent;
    m_IsDirty = true;
}