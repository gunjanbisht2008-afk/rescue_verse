#pragma once

#include <QList>
#include <QString>

class Hospital {
public:
    Hospital(int id = 0,
             const QString& name = {},
             const QString& location = {},
             int beds = 0);

    int id() const;
    QString name() const;
    QString location() const;
    int totalBeds() const;
    int availableBeds() const;

    bool admitVictim(int victimId);
    bool hasVictim(int victimId) const;

private:
    int id_;
    QString name_;
    QString location_;
    int totalBeds_;
    int availableBeds_;
    QList<int> admittedVictimIds_;
};
