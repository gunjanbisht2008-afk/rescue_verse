#pragma once

#include <QString>

enum class VictimPriority {
    Normal = 1,
    Serious = 2,
    Critical = 3
};

enum class VictimStatus {
    Pending,
    Ready,
    Assigned,
    Admitted
};

QString priorityToString(VictimPriority priority);
QString statusToString(VictimStatus status);

class Victim {
public:
    Victim(int id = 0,
           const QString& name = {},
           const QString& location = {},
           VictimPriority priority = VictimPriority::Normal);

    int id() const;
    QString name() const;
    QString location() const;
    VictimPriority priority() const;
    VictimStatus status() const;

    void setStatus(VictimStatus status);

    // std::priority_queue uses the largest value first. Critical (3) must
    // therefore compare as "larger" than Serious (2) and Normal (1).
    bool operator<(const Victim& other) const;

private:
    int id_;
    QString name_;
    QString location_;
    VictimPriority priority_;
    VictimStatus status_;
};