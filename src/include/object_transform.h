#pragma once

#include "includes.h"
#include <memory>

class ObjectTransform
{
private:
    vec3 m_LocalPosition;
    vec3 m_LocalRotation;
    vec3 m_LocalScale;

    mutable vec3 m_WorldPosition;
    mutable vec3 m_WorldRotation;
    mutable vec3 m_WorldScale;

    std::shared_ptr<ObjectTransform> m_Parent;

    mutable bool m_IsDirty;

public:
    ObjectTransform();
    ObjectTransform(vec3 position, vec3 scale = vec3(1.0f));

    mat4 GetModelMatrix() const;
    void UpdateWorldTransform() const;

    // transform methods
    void Translate(const vec3 &translation);
    void Rotate(const vec3 &rotation);
    void SetPosition(const vec3 &position);
    void SetRotation(const vec3 &rotation);
    void SetScale(const vec3 &scale);

    // hierarchy methods
    void SetParent(std::shared_ptr<ObjectTransform> parent);

    // gets
    inline vec3 GetLocalPosition() const { return m_LocalPosition; }
    inline vec3 GetLocalRotation() const { return m_LocalRotation; }
    inline vec3 GetLocalScale() const { return m_LocalScale; }

    inline vec3 GetWorldPosition() const
    {
        if (m_IsDirty)
            UpdateWorldTransform();
        return m_WorldPosition;
    }
    inline vec3 GetWorldRotation() const
    {
        if (m_IsDirty)
            UpdateWorldTransform();
        return m_WorldRotation;
    }
    inline vec3 GetWorldScale() const
    {
        if (m_IsDirty)
            UpdateWorldTransform();
        return m_WorldScale;
    }

    inline std::shared_ptr<ObjectTransform> GetParent() const { return m_Parent; }
};