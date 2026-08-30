#pragma once

#include <QString>

enum class TeamType {
    Ambulance,
    FireRescue,
    GeneralRescue
};

QString teamTypeToString(TeamType type);

class RescueTeam {
public:
    RescueTeam(int id = 0,
               TeamType type = TeamType::GeneralRescue,
               const QString& location = {});

    int id() const;
    TeamType type() const;
    QString location() const;
    bool isAvailable() const;
    int currentVictimId() const;

    bool assignTo(int victimId);
    void release();

private:
    int id_;
    TeamType type_;
    QString location_;
    bool available_;
    int currentVictimId_;
};