#pragma once

#include "object_transform.h"
#include "includes.h"
#include <memory>

class GameObject
{
protected:
    ObjectTransform m_Transform;

public:
    GameObject();
    virtual ~GameObject() = default;

    ObjectTransform &GetTransform() { return m_Transform; }
    const ObjectTransform &GetTransform() const { return m_Transform; }
    void SetTransform(const ObjectTransform &transform);

    // hierarchy methods
    void SetParent(std::shared_ptr<GameObject> parent);
};