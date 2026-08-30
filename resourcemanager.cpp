#include "resourcemanager.h"

ResourceManager::ResourceManager()
{
    addResource("Bandage", 20);
    addResource("First Aid Kit", 10);
    addResource("Medicine", 15);
}

void ResourceManager::clear()
{
    resources_.clear();
}

void ResourceManager::addResource(const QString& name, int quantity)
{
    resources_[name] = quantity;
}

bool ResourceManager::canConsume(const QString& name, int quantity) const
{
    return quantity >= 0 && resources_.value(name, 0) >= quantity;
}

bool ResourceManager::consumeResource(const QString& name, int quantity)
{
    if (!canConsume(name, quantity)) {
        return false;
    }

    resources_[name] -= quantity;
    return true;
}

int ResourceManager::quantity(const QString& name) const
{
    return resources_.value(name, 0);
}

QMap<QString, int> ResourceManager::allResources() const
{
    return resources_;
}