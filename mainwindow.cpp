#include "mainwindow.h"

#include <QAbstractItemView>
#include <QFont>
#include <QGridLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QStatusBar>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QWidget>

namespace {

QTableWidgetItem* cell(const QString& text)
{
    auto* item = new QTableWidgetItem(text);
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    return item;
}

QString availabilityText(const RescueTeam& team)
{
    return team.isAvailable() ? "Available" : "Busy";
}

}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    currentVictimLabel_(nullptr),
    activityLabel_(nullptr),
    victimsTable_(nullptr),
    teamsTable_(nullptr),
    hospitalsTable_(nullptr),
    resourcesTable_(nullptr)
{
    buildUi();
    refreshTables();
}

void MainWindow::buildUi()
{
    setWindowTitle("RescueVerse | Rescue & Resource Command Center");
    resize(1280, 780);

    auto* central = new QWidget(this);
    auto* rootLayout = new QVBoxLayout(central);
    rootLayout->setContentsMargins(24, 20, 24, 20);
    rootLayout->setSpacing(14);

    auto* title = new QLabel("RESCUEVERSE // RESCUE & RESOURCE MODULE");
    title->setObjectName("title");
    title->setProperty("data-testid", "dashboard-title");
    rootLayout->addWidget(title);

    auto* subtitle = new QLabel(
        "Student 2 operations console  |  Priority queue → team assignment → hospital → resources");
    subtitle->setObjectName("subtitle");
    rootLayout->addWidget(subtitle);

    auto* commandPanel = new QWidget;
    commandPanel->setObjectName("commandPanel");
    auto* commandLayout = new QGridLayout(commandPanel);
    commandLayout->setContentsMargins(14, 12, 14, 12);
    commandLayout->setHorizontalSpacing(10);

    currentVictimLabel_ = new QLabel("No victim selected");
    currentVictimLabel_->setObjectName("currentVictim");
    currentVictimLabel_->setProperty("data-testid", "current-victim-label");
    activityLabel_ = new QLabel("Ready. The highest priority victim is handled first.");
    activityLabel_->setObjectName("activity");
    activityLabel_->setWordWrap(true);

    auto* nextButton = new QPushButton("1  SHOW NEXT VICTIM");
    auto* assignButton = new QPushButton("2  ASSIGN TEAM");
    auto* hospitalButton = new QPushButton("3  SEND TO HOSPITAL");
    auto* resetButton = new QPushButton("RESET");
    nextButton->setObjectName("primaryButton");
    assignButton->setObjectName("primaryButton");
    hospitalButton->setObjectName("primaryButton");
    resetButton->setObjectName("resetButton");

    nextButton->setProperty("data-testid", "show-next-victim-button");
    assignButton->setProperty("data-testid", "assign-team-button");
    hospitalButton->setProperty("data-testid", "send-to-hospital-button");
    resetButton->setProperty("data-testid", "reset-simulation-button");

    connect(nextButton, &QPushButton::clicked, this, &MainWindow::showNextVictim);
    connect(assignButton, &QPushButton::clicked, this, &MainWindow::assignTeam);
    connect(hospitalButton, &QPushButton::clicked, this, &MainWindow::sendToHospital);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::resetSimulation);

    commandLayout->addWidget(new QLabel("ACTIVE CASE"), 0, 0);
    commandLayout->addWidget(currentVictimLabel_, 1, 0, 1, 2);
    commandLayout->addWidget(activityLabel_, 0, 2, 2, 2);
    commandLayout->addWidget(nextButton, 0, 4);
    commandLayout->addWidget(assignButton, 0, 5);
    commandLayout->addWidget(hospitalButton, 1, 4);
    commandLayout->addWidget(resetButton, 1, 5);
    rootLayout->addWidget(commandPanel);

    auto* grid = new QGridLayout;
    grid->setSpacing(14);

    victimsTable_ = createTable({"ID", "NAME", "LOCATION", "PRIORITY", "STATUS"});
    teamsTable_ = createTable({"TEAM", "TYPE", "LOCATION", "STATUS"});
    hospitalsTable_ = createTable({"HOSPITAL", "LOCATION", "BEDS"});
    resourcesTable_ = createTable({"RESOURCE", "AVAILABLE"});

    auto* victimsHeading = new QLabel("VICTIMS // PRIORITY QUEUE VIEW");
    victimsHeading->setObjectName("sectionHeading");
    auto* teamsHeading = new QLabel("RESCUE TEAMS");
    teamsHeading->setObjectName("sectionHeading");
    auto* hospitalsHeading = new QLabel("HOSPITAL CAPACITY");
    hospitalsHeading->setObjectName("sectionHeading");
    auto* resourcesHeading = new QLabel("MEDICINE & RESOURCES");
    resourcesHeading->setObjectName("sectionHeading");

    auto* victimsPanel = new QWidget;
    auto* victimsLayout = new QVBoxLayout(victimsPanel);
    victimsLayout->setContentsMargins(14, 12, 14, 14);
    victimsLayout->addWidget(victimsHeading);
    victimsLayout->addWidget(victimsTable_);
    victimsPanel->setObjectName("panel");

    auto* teamsPanel = new QWidget;
    auto* teamsLayout = new QVBoxLayout(teamsPanel);
    teamsLayout->setContentsMargins(14, 12, 14, 14);
    teamsLayout->addWidget(teamsHeading);
    teamsLayout->addWidget(teamsTable_);
    teamsPanel->setObjectName("panel");

    auto* hospitalsPanel = new QWidget;
    auto* hospitalsLayout = new QVBoxLayout(hospitalsPanel);
    hospitalsLayout->setContentsMargins(14, 12, 14, 14);
    hospitalsLayout->addWidget(hospitalsHeading);
    hospitalsLayout->addWidget(hospitalsTable_);
    hospitalsPanel->setObjectName("panel");

    auto* resourcesPanel = new QWidget;
    auto* resourcesLayout = new QVBoxLayout(resourcesPanel);
    resourcesLayout->setContentsMargins(14, 12, 14, 14);
    resourcesLayout->addWidget(resourcesHeading);
    resourcesLayout->addWidget(resourcesTable_);
    resourcesPanel->setObjectName("panel");

    grid->addWidget(victimsPanel, 0, 0, 2, 2);
    grid->addWidget(teamsPanel, 0, 2, 1, 1);
    grid->addWidget(resourcesPanel, 1, 2, 1, 1);
    grid->addWidget(hospitalsPanel, 2, 0, 1, 3);
    rootLayout->addLayout(grid, 1);

    setCentralWidget(central);
    statusBar()->showMessage("SYSTEM ONLINE // Local demo data loaded");

    setStyleSheet(R"(
        QWidget {
            background: #0B1120;
            color: #F8FAFC;
            font-family: "IBM Plex Sans", "Segoe UI", sans-serif;
            font-size: 13px;
        }
        QMainWindow { background: #0B1120; }
        QLabel#title {
            color: #F8FAFC;
            font-size: 24px;
            font-weight: 700;
            letter-spacing: 1px;
        }
        QLabel#subtitle { color: #94A3B8; }
        QLabel#sectionHeading {
            color: #F59E0B;
            font-weight: 700;
            letter-spacing: 1px;
            padding-bottom: 4px;
        }
        QLabel#currentVictim {
            color: #EF4444;
            font-size: 17px;
            font-weight: 700;
        }
        QLabel#activity { color: #CBD5E1; }
        QWidget#commandPanel, QWidget#panel {
            background: #111827;
            border: 1px solid rgba(255, 255, 255, 0.12);
        }
        QTableWidget {
            background: #111827;
            alternate-background-color: #172033;
            border: 1px solid rgba(255, 255, 255, 0.08);
            gridline-color: rgba(255, 255, 255, 0.08);
            selection-background-color: #243B63;
            selection-color: #F8FAFC;
        }
        QHeaderView::section {
            background: #1F2937;
            color: #94A3B8;
            border: none;
            padding: 7px;
            font-size: 11px;
            font-weight: 700;
        }
        QTableWidget::item { padding: 6px; }
        QPushButton {
            background: #1F2937;
            color: #F8FAFC;
            border: 1px solid rgba(255, 255, 255, 0.18);
            border-radius: 2px;
            padding: 9px 12px;
            font-weight: 700;
        }
        QPushButton:hover { background: #334155; color: #FFFFFF; }
        QPushButton#primaryButton {
            background: #B45309;
            border-color: #F59E0B;
        }
        QPushButton#primaryButton:hover { background: #D97706; }
        QPushButton#resetButton { color: #FCA5A5; }
        QStatusBar { color: #94A3B8; background: #0B1120; }
    )");
}

QTableWidget* MainWindow::createTable(const QStringList& headers)
{
    auto* table = new QTableWidget;
    table->setColumnCount(headers.size());
    table->setHorizontalHeaderLabels(headers);
    table->verticalHeader()->setVisible(false);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(true);
    table->horizontalHeader()->setStretchLastSection(true);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setMinimumHeight(130);
    return table;
}

void MainWindow::refreshTables()
{
    victimsTable_->setRowCount(controller_.victims().size());
    for (int row = 0; row < controller_.victims().size(); ++row) {
        const Victim& victim = controller_.victims().at(row);
        victimsTable_->setItem(row, 0, cell(QString::number(victim.id())));
        victimsTable_->setItem(row, 1, cell(victim.name()));
        victimsTable_->setItem(row, 2, cell(victim.location()));
        victimsTable_->setItem(row, 3, cell(priorityToString(victim.priority())));
        victimsTable_->setItem(row, 4, cell(statusToString(victim.status())));
    }

    teamsTable_->setRowCount(controller_.teams().size());
    for (int row = 0; row < controller_.teams().size(); ++row) {
        const RescueTeam& team = controller_.teams().at(row);
        teamsTable_->setItem(row, 0, cell(QString("Team %1").arg(team.id())));
        teamsTable_->setItem(row, 1, cell(teamTypeToString(team.type())));
        teamsTable_->setItem(row, 2, cell(team.location()));
        teamsTable_->setItem(row, 3, cell(availabilityText(team)));
    }

    hospitalsTable_->setRowCount(controller_.hospitals().size());
    for (int row = 0; row < controller_.hospitals().size(); ++row) {
        const Hospital& hospital = controller_.hospitals().at(row);
        hospitalsTable_->setItem(row, 0, cell(hospital.name()));
        hospitalsTable_->setItem(row, 1, cell(hospital.location()));
        hospitalsTable_->setItem(
            row, 2, cell(QString("%1 / %2 available")
                     .arg(hospital.availableBeds())
                     .arg(hospital.totalBeds())));
    }

    const QMap<QString, int> resources = controller_.resources().allResources();
    resourcesTable_->setRowCount(resources.size());
    int row = 0;
    for (auto it = resources.cbegin(); it != resources.cend(); ++it, ++row) {
        resourcesTable_->setItem(row, 0, cell(it.key()));
        resourcesTable_->setItem(row, 1, cell(QString::number(it.value())));
    }

    const std::optional<Victim> current = controller_.currentVictim();
    currentVictimLabel_->setText(current.has_value()
                                     ? QString("#%1  %2  |  %3  |  %4")
                                           .arg(current->id())
                                           .arg(current->name())
                                           .arg(current->location())
                                           .arg(priorityToString(current->priority()))
                                     : "No victim selected");
}

void MainWindow::showResult(const ActionResult& result)
{
    activityLabel_->setText(result.message);
    statusBar()->showMessage(result.message, 5000);
    refreshTables();
}

void MainWindow::showNextVictim()
{
    showResult(controller_.showNextVictim());
}

void MainWindow::assignTeam()
{
    showResult(controller_.assignAvailableTeam());
}

void MainWindow::sendToHospital()
{
    showResult(controller_.sendVictimToHospital());
}

void MainWindow::resetSimulation()
{
    controller_.reset();
    showResult({true, "Simulation reset. Priority queue restored with four victims."});
}
