#pragma once

#include "hospital.h"
#include "resourcemanager.h"
#include "rescueteam.h"
#include "victim.h"

#include <QVector>

#include <optional>
#include <queue>

struct ActionResult
{
    bool success;
    QString message;
};

class RescueController
{
public:
    RescueController();

    void reset();

    const QVector<Victim> &victims() const;
    const QVector<RescueTeam> &teams() const;
    const QVector<Hospital> &hospitals() const;
    const ResourceManager &resources() const;
    std::optional<Victim> currentVictim() const;

    ActionResult addVictim(const QString &name,
                           const QString &location,
                           VictimPriority priority);
    ActionResult showNextVictim();
    ActionResult assignAvailableTeam();
    ActionResult sendVictimToHospital();

private:
    Victim *findVictim(int victimId);
    const Victim *findVictim(int victimId) const;
    RescueTeam *findTeam(int teamId);
    Hospital *findHospitalWithBed();
    void flushDeferredVictims();

    QVector<Victim> victims_;
    QVector<RescueTeam> teams_;
    QVector<Hospital> hospitals_;
    ResourceManager resources_;
    std::priority_queue<Victim> priorityQueue_;
    QVector<Victim> deferredVictims_;
    int currentVictimId_;
    int assignedTeamId_;
};