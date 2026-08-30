#include "rescuecontroller.h"

#include <QCoreApplication>
#include <QDebug>

int main(int argc, char* argv[])
{
    QCoreApplication application(argc, argv);
    RescueController controller;

    const ActionResult next = controller.showNextVictim();
    Q_ASSERT(next.success);
    Q_ASSERT(controller.currentVictim().has_value());
    Q_ASSERT(controller.currentVictim()->priority() == VictimPriority::Critical);

    const ActionResult assignment = controller.assignAvailableTeam();
    Q_ASSERT(assignment.success);

    const int bedsBefore = controller.hospitals().first().availableBeds();
    const int bandagesBefore = controller.resources().quantity("Bandage");
    const ActionResult admission = controller.sendVictimToHospital();
    Q_ASSERT(admission.success);
    Q_ASSERT(controller.hospitals().first().availableBeds() == bedsBefore - 1);
    Q_ASSERT(controller.resources().quantity("Bandage") == bandagesBefore - 1);

    // Critical victim #4 is next because #1 and #4 share priority 3 and the
    // comparator resolves ties with the lower ID first.
    Q_ASSERT(controller.showNextVictim().success);
    Q_ASSERT(controller.currentVictim()->id() == 4);

    qDebug() << "RescueVerse logic test passed: priority queue, assignment,"
                "hospital bed, and resource update.";
    return 0;
}