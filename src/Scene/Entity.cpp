#include "Entity.h"

Entity::Entity(entt::entity handle, Scene* scene) 
: entityHandle(handle), scene(scene)
{
    
}

entt::entity Entity::GetHandle()
{
    return entityHandle;
}
