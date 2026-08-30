#pragma once

#include <QMap>
#include <QString>

class ResourceManager {
public:
    ResourceManager();

    void clear();
    void addResource(const QString& name, int quantity);
    bool canConsume(const QString& name, int quantity) const;
    bool consumeResource(const QString& name, int quantity);
    int quantity(const QString& name) const;
    QMap<QString, int> allResources() const;

private:
    QMap<QString, int> resources_;
};