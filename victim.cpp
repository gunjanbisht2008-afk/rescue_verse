#include "victim.h"

QString priorityToString(VictimPriority priority)
{
    switch (priority) {
    case VictimPriority::Critical:
        return "Critical";
    case VictimPriority::Serious:
        return "Serious";
    case VictimPriority::Normal:
        return "Normal";
    }

    return "Unknown";
}

QString statusToString(VictimStatus status)
{
    switch (status) {
    case VictimStatus::Pending:
        return "Pending";
    case VictimStatus::Ready:
        return "Ready";
    case VictimStatus::Assigned:
        return "Assigned";
    case VictimStatus::Admitted:
        return "Admitted";
    }

    return "Unknown";
}

Victim::Victim(int id,
               const QString& name,
               const QString& location,
               VictimPriority priority)
    : id_(id),
    name_(name),
    location_(location),
    priority_(priority),
    status_(VictimStatus::Pending)
{
}

int Victim::id() const
{
    return id_;
}

QString Victim::name() const
{
    return name_;
}

QString Victim::location() const
{
    return location_;
}

VictimPriority Victim::priority() const
{
    return priority_;
}

VictimStatus Victim::status() const
{
    return status_;
}

void Victim::setStatus(VictimStatus status)
{
    status_ = status;
}

bool Victim::operator<(const Victim& other) const
{
    const int thisPriority = static_cast<int>(priority_);
    const int otherPriority = static_cast<int>(other.priority_);

    if (thisPriority != otherPriority) {
        return thisPriority < otherPriority;
    }

    // For equal priority, the smaller ID is served first. Because this is a
    // max-heap comparison, the larger ID is considered "less" here.
    return id_ > other.id_;
}
