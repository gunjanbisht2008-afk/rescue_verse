#include "rescuecontroller.h"

RescueController::RescueController()
    : currentVictimId_(-1),
    assignedTeamId_(-1)
{
    reset();
}

void RescueController::reset()
{
    victims_.clear();
    teams_.clear();
    hospitals_.clear();
    resources_ = ResourceManager();
    priorityQueue_ = {};
    currentVictimId_ = -1;
    assignedTeamId_ = -1;

    victims_.append(Victim(1, "Ananya", "A", VictimPriority::Critical));
    victims_.append(Victim(2, "Ravi", "C", VictimPriority::Normal));
    victims_.append(Victim(3, "Meera", "B", VictimPriority::Serious));
    victims_.append(Victim(4, "Kabir", "D", VictimPriority::Critical));

    for (const Victim& victim : victims_) {
        priorityQueue_.push(victim);
    }

    teams_.append(RescueTeam(1, TeamType::Ambulance, "B"));
    teams_.append(RescueTeam(2, TeamType::FireRescue, "D"));
    teams_.append(RescueTeam(3, TeamType::GeneralRescue, "A"));

    hospitals_.append(Hospital(1, "City Hospital", "E", 5));
    hospitals_.append(Hospital(2, "Care Point", "F", 2));
}

const QVector<Victim>& RescueController::victims() const
{
    return victims_;
}

const QVector<RescueTeam>& RescueController::teams() const
{
    return teams_;
}

const QVector<Hospital>& RescueController::hospitals() const
{
    return hospitals_;
}

const ResourceManager& RescueController::resources() const
{
    return resources_;
}

std::optional<Victim> RescueController::currentVictim() const
{
    if (currentVictimId_ < 0) {
        return std::nullopt;
    }

    const Victim* victim = findVictim(currentVictimId_);
    if (victim == nullptr) {
        return std::nullopt;
    }

    return *victim;
}

ActionResult RescueController::showNextVictim()
{
    if (currentVictimId_ >= 0) {
        return {false, "Finish the current rescue before selecting another victim."};
    }

    if (priorityQueue_.empty()) {
        return {false, "The priority queue is empty. All demo victims are admitted."};
    }

    currentVictimId_ = priorityQueue_.top().id();
    Victim* victim = findVictim(currentVictimId_);
    if (victim == nullptr) {
        currentVictimId_ = -1;
        return {false, "The selected victim could not be found."};
    }

    victim->setStatus(VictimStatus::Ready);
    return {true,
            QString("Next victim: #%1 %2 (%3 priority).")
                .arg(victim->id())
                .arg(victim->name())
                .arg(priorityToString(victim->priority()))};
}

ActionResult RescueController::assignAvailableTeam()
{
    if (currentVictimId_ < 0) {
        return {false, "Click Show Next Victim first."};
    }

    if (assignedTeamId_ >= 0) {
        return {false, "A rescue team is already assigned to this victim."};
    }

    for (RescueTeam& team : teams_) {
        if (team.isAvailable() && team.assignTo(currentVictimId_)) {
            assignedTeamId_ = team.id();
            Victim* victim = findVictim(currentVictimId_);
            if (victim != nullptr) {
                victim->setStatus(VictimStatus::Assigned);
            }

            priorityQueue_.pop();
            return {true,
                    QString("Team %1 (%2) assigned to victim #%3.")
                        .arg(team.id())
                        .arg(teamTypeToString(team.type()))
                        .arg(currentVictimId_)};
        }
    }

    return {false, "No rescue team is currently available."};
}

ActionResult RescueController::sendVictimToHospital()
{
    if (currentVictimId_ < 0 || assignedTeamId_ < 0) {
        return {false, "Select a victim and assign a rescue team first."};
    }

    Hospital* hospital = findHospitalWithBed();
    if (hospital == nullptr) {
        return {false, "No hospital bed is available."};
    }

    // This module's treatment bundle is deliberately small and visible in the
    // UI: one bandage, one first-aid kit, and one medicine unit.
    if (!resources_.canConsume("Bandage", 1)
        || !resources_.canConsume("First Aid Kit", 1)
        || !resources_.canConsume("Medicine", 1)) {
        return {false, "Required medicines are unavailable for this admission."};
    }

    if (!hospital->admitVictim(currentVictimId_)) {
        return {false, "The selected hospital could not admit this victim."};
    }

    resources_.consumeResource("Bandage", 1);
    resources_.consumeResource("First Aid Kit", 1);
    resources_.consumeResource("Medicine", 1);

    Victim* victim = findVictim(currentVictimId_);
    if (victim != nullptr) {
        victim->setStatus(VictimStatus::Admitted);
    }

    RescueTeam* team = findTeam(assignedTeamId_);
    if (team != nullptr) {
        team->release();
    }

    const QString message = QString("Victim #%1 admitted to %2. Resources updated.")
                                .arg(currentVictimId_)
                                .arg(hospital->name());
    currentVictimId_ = -1;
    assignedTeamId_ = -1;
    return {true, message};
}

Victim* RescueController::findVictim(int victimId)
{
    for (Victim& victim : victims_) {
        if (victim.id() == victimId) {
            return &victim;
        }
    }

    return nullptr;
}

const Victim* RescueController::findVictim(int victimId) const
{
    for (const Victim& victim : victims_) {
        if (victim.id() == victimId) {
            return &victim;
        }
    }

    return nullptr;
}

RescueTeam* RescueController::findTeam(int teamId)
{
    for (RescueTeam& team : teams_) {
        if (team.id() == teamId) {
            return &team;
        }
    }

    return nullptr;
}

Hospital* RescueController::findHospitalWithBed()
{
    for (Hospital& hospital : hospitals_) {
        if (hospital.availableBeds() > 0) {
            return &hospital;
        }
    }

    return nullptr;
}