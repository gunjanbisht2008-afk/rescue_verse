#include "hospital.h"

Hospital::Hospital(int id,
                   const QString& name,
                   const QString& location,
                   int beds)
    : id_(id),
    name_(name),
    location_(location),
    totalBeds_(beds),
    availableBeds_(beds)
{
}

int Hospital::id() const
{
    return id_;
}

QString Hospital::name() const
{
    return name_;
}

QString Hospital::location() const
{
    return location_;
}

int Hospital::totalBeds() const
{
    return totalBeds_;
}

int Hospital::availableBeds() const
{
    return availableBeds_;
}

bool Hospital::admitVictim(int victimId)
{
    if (availableBeds_ <= 0 || hasVictim(victimId)) {
        return false;
    }

    admittedVictimIds_.append(victimId);
    --availableBeds_;
    return true;
}

bool Hospital::hasVictim(int victimId) const
{
    return admittedVictimIds_.contains(victimId);
}