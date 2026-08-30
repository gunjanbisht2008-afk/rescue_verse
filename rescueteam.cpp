#include "rescueteam.h"

QString teamTypeToString(TeamType type)
{
    switch (type) {
    case TeamType::Ambulance:
        return "Ambulance";
    case TeamType::FireRescue:
        return "Fire Rescue";
    case TeamType::GeneralRescue:
        return "General Rescue";
    }

    return "Unknown";
}

RescueTeam::RescueTeam(int id, TeamType type, const QString& location)
    : id_(id),
    type_(type),
    location_(location),
    available_(true),
    currentVictimId_(-1)
{
}

int RescueTeam::id() const
{
    return id_;
}

TeamType RescueTeam::type() const
{
    return type_;
}

QString RescueTeam::location() const
{
    return location_;
}

bool RescueTeam::isAvailable() const
{
    return available_;
}

int RescueTeam::currentVictimId() const
{
    return currentVictimId_;
}

bool RescueTeam::assignTo(int victimId)
{
    if (!available_) {
        return false;
    }

    available_ = false;
    currentVictimId_ = victimId;
    return true;
}

void RescueTeam::release()
{
    available_ = true;
    currentVictimId_ = -1;
}