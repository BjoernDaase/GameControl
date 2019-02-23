#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdio.h>

#ifdef __linux__
    #include <unistd.h>
    #define GetCurrentDir getcwd
    #define PATH_SEPARATOR "/"
#elif defined _WIN32 //We are on Windows, THIS BRANCH IS NOT TESTED
    #include <direct.h>
    #define GetCurrentDir _getcwd
    #define PATH_SEPARATOR "\\"
#endif


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), count(countStart)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/tusliLogo_quadratisch.png"));

    timer = new QBasicTimer();

    statusBar()->showMessage(tr("Created timer %1").arg(timer->timerId()), 1000);

    connect(ui->homeNameEdit, SIGNAL(textChanged(const QString&)), this, SLOT(WriteHomeTeamToFile()));
    connect(ui->guestNameEdit, SIGNAL(textChanged(const QString&)), this, SLOT(WriteGuestTeamToFile()));
    connect(ui->legComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(WriteLegToFile()));
    connect(ui->homeGoalsSpinBox, SIGNAL(editingFinished()), this, SLOT(WriteHomeTeamGoalsToFile()));
    connect(ui->guestGoalsSpinBox, SIGNAL(editingFinished()), this, SLOT(WriteGuestTeamGoalsToFile()));
    connect(ui->homeIncreaseGoalsButton, SIGNAL(clicked(bool)), this, SLOT(IncreaseHomeGoals()));
    connect(ui->homeDecreaseGoalsButton, SIGNAL(clicked(bool)), this, SLOT(DecreaseHomeGoals()));
    connect(ui->guestIncreaseGoalsButton, SIGNAL(clicked(bool)), this, SLOT(IncreaseGuestGoals()));
    connect(ui->guestDecreaseGoalsButton, SIGNAL(clicked(bool)), this, SLOT(DecreaseGuestGoals()));
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::Start);
    connect(ui->setButton, &QPushButton::clicked, this, &MainWindow::Set);
    connect(ui->set15Button, &QPushButton::clicked, this, &MainWindow::Set15);
    connect(ui->set30Button, &QPushButton::clicked, this, &MainWindow::Set30);
    connect(ui->set35Button, &QPushButton::clicked, this, &MainWindow::Set35);
    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::Clear);

    QPixmap image(QString::fromStdString(this->getCurrentWorkingDir() + PATH_SEPARATOR + "tusliLogo.png"));
    ui->logoLabel->setPixmap(image);

    ui->lcdNumber->display(this->formattedTime());
    this->WriteHomeTeamToFile();
    this->WriteGuestTeamToFile();
    this->WriteLegToFile();
    this->WriteHomeTeamGoalsToFile();
    this->WriteGuestTeamGoalsToFile();
    this->writeTimeToFile();
}

MainWindow::~MainWindow()
{
    delete timer;
    delete ui;
}

void MainWindow::Clear()
{
    count = countStart;
    ui->lcdNumber->display(this->formattedTime());

    ui->homeNameEdit->setText("");
    ui->guestNameEdit->setText("");
    ui->homeGoalsSpinBox->setValue(0);
    ui->guestGoalsSpinBox->setValue(0);
    ui->legComboBox->setCurrentIndex(0);

    this->WriteHomeTeamToFile();
    this->WriteGuestTeamToFile();
    this->WriteLegToFile();
    this->WriteHomeTeamGoalsToFile();
    this->WriteGuestTeamGoalsToFile();
    this->writeTimeToFile();
}

void MainWindow::Set()
{
    auto timeToBeSet = ui->timeEdit->time();
    count = timeToBeSet.minute() * 60 + timeToBeSet.second();
    ui->lcdNumber->display(this->formattedTime());
    this->writeTimeToFile();
}

void MainWindow::Set15()
{
    count = 900; //900 seconds = 15 minutes
    ui->lcdNumber->display(this->formattedTime());
    this->writeTimeToFile();
}

void MainWindow::Set30()
{
    count = 1800; //1800 seconds = 30 minutes
    ui->lcdNumber->display(this->formattedTime());
    this->writeTimeToFile();
}

void MainWindow::Set35()
{
    count = 2100; //2100 seconds = 35 minutes
    ui->lcdNumber->display(this->formattedTime());
    this->writeTimeToFile();
}

void MainWindow::Start()
{
    if (timer->isActive()) {
        timer->stop();
        ui->startButton->setText(tr("Start"));
    } else {
        Qt::TimerType type = Qt::PreciseTimer;
        timer->start(1000, type, this);
        ui->startButton->setText(tr("Stop"));
    }
}
void MainWindow::IncreaseHomeGoals()
{
   ui->homeGoalsSpinBox->setValue(ui->homeGoalsSpinBox->value() + 1);
   this->WriteHomeTeamGoalsToFile();
}


void MainWindow::DecreaseHomeGoals()
{
    ui->homeGoalsSpinBox->setValue(ui->homeGoalsSpinBox->value() - 1);
    this->WriteHomeTeamGoalsToFile();
}


void MainWindow::IncreaseGuestGoals()
{
    ui->guestGoalsSpinBox->setValue(ui->guestGoalsSpinBox->value() + 1);
    this->WriteGuestTeamGoalsToFile();
}

void MainWindow::DecreaseGuestGoals()
{
    ui->guestGoalsSpinBox->setValue(ui->guestGoalsSpinBox->value() - 1);
    this->WriteGuestTeamGoalsToFile();
}

void MainWindow::WriteHomeTeamToFile()
{
    this->writeToFile(this->getCurrentWorkingDir() + PATH_SEPARATOR + "home.txt", ui->homeNameEdit->text().toStdString());
}

void MainWindow::WriteGuestTeamToFile()
{
    this->writeToFile(this->getCurrentWorkingDir() + PATH_SEPARATOR + "guest.txt", ui->guestNameEdit->text().toStdString());
}

void MainWindow::WriteLegToFile()
{
    this->writeToFile(this->getCurrentWorkingDir() + PATH_SEPARATOR + "leg.txt", ui->legComboBox->currentText().toStdString());
}

void MainWindow::WriteHomeTeamGoalsToFile()
{
    this->writeToFile(this->getCurrentWorkingDir() + PATH_SEPARATOR + "homeGoals.txt", ui->homeGoalsSpinBox->text().toStdString());
}

void MainWindow::WriteGuestTeamGoalsToFile()
{
    this->writeToFile(this->getCurrentWorkingDir() + PATH_SEPARATOR + "guestGoals.txt", ui->guestGoalsSpinBox->text().toStdString());
}

void MainWindow::writeTimeToFile()
{
    std::string formattedTime = this->formattedTimeAsNormalString();
    if (count >= 0) ui->lcdNumber->display(QString::fromStdString(formattedTime));
    this->writeToFile(this->getCurrentWorkingDir() + PATH_SEPARATOR + "timer.txt", formattedTime);
}

QString MainWindow::formattedTime()
{
    return QString::fromStdString(this->formattedTimeAsNormalString());
}

std::string MainWindow::formattedTimeAsNormalString()
{
    std::stringstream stringStream;
    int remainingMinutes = count / 60;
    int remainingSeconds = count % 60;

    if (remainingMinutes < 10) stringStream << std::string("0");
    stringStream << remainingMinutes << ":";
    if (remainingSeconds < 10) stringStream << std::string("0");
    stringStream << remainingSeconds;

    return stringStream.str();
}

void MainWindow::writeToFile(std::string file, std::string content)
{
    std::ofstream outputFile(file);
    outputFile << content;
    outputFile.close();
}

std::string MainWindow::getCurrentWorkingDir()
{
  char buff[FILENAME_MAX];
  GetCurrentDir(buff, FILENAME_MAX);
  std::string current_working_dir(buff);
  return current_working_dir;
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    count--;
    this->writeTimeToFile();
    event->accept();
}

