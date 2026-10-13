#pragma once

#include "rescuecontroller.h"

#include <QMainWindow>

class QLabel;
class QLineEdit;
class QPlainTextEdit;
class QTableWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void showNextVictim();
    void on_btnAddVictim_clicked();
    void on_btnDispatchTeam_clicked();
    void sendToHospital();
    void resetSimulation();

private:
    void buildUi();
    void initializeTables();
    void refreshTables();
    void showResult(const ActionResult& result);
    QTableWidget* createTable();

    RescueController controller_;
    QLabel* currentVictimLabel_;
    QLabel* activityLabel_;
    QLineEdit* lineEditVictimName_;
    QLineEdit* lineEditLocation_;
    QLineEdit* lineEditSeverity_;
    QPlainTextEdit* txtLogs_;
    QTableWidget* victimsTable_;
    QTableWidget* teamsTable_;
    QTableWidget* hospitalsTable_;
    QTableWidget* resourcesTable_;
};
