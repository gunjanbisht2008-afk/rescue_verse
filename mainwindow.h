#pragma once

#include "rescuecontroller.h"

#include <QMainWindow>

#include <QStringList>

class QLabel;
class QTableWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void showNextVictim();
    void assignTeam();
    void sendToHospital();
    void resetSimulation();

private:
    void buildUi();
    void refreshTables();
    void showResult(const ActionResult& result);
    QTableWidget* createTable(const QStringList& headers);

    RescueController controller_;
    QLabel* currentVictimLabel_;
    QLabel* activityLabel_;
    QTableWidget* victimsTable_;
    QTableWidget* teamsTable_;
    QTableWidget* hospitalsTable_;
    QTableWidget* resourcesTable_;
};
