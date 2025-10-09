#include "game_object.h"
#include "includes.h"

GameObject::GameObject()
{
    m_Transform = ObjectTransform();
}

void GameObject::SetTransform(const ObjectTransform &transform)
{
    m_Transform = transform;
}

void GameObject::SetParent(std::shared_ptr<GameObject> parent)
{
    m_Transform.SetParent(std::make_shared<ObjectTransform>(parent->GetTransform()));
}