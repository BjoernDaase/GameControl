#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), count(countStart)
{
    ui->setupUi(this);

    timer = new QBasicTimer();

    statusBar()->showMessage(tr("Created timer %1").arg(timer->timerId()), 1000);

    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::Clear);
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::Start);
    connect(ui->setButton, &QPushButton::clicked, this, &MainWindow::Set);
    connect(ui->set15Button, &QPushButton::clicked, this, &MainWindow::Set15);
    connect(ui->set30Button, &QPushButton::clicked, this, &MainWindow::Set30);
    connect(ui->set35Button, &QPushButton::clicked, this, &MainWindow::Set35);
    connect(ui->homeNameEdit, &QLineEdit::textChanged, this, &MainWindow::WriteHomeTeamToFile);
    connect(ui->guestNameEdit, &QLineEdit::textChanged, this, &MainWindow::WriteGuestTeamToFile);
    connect(ui->homeGoalsSpinBox, &QSpinBox::editingFinished, this, &MainWindow::WriteHomeTeamGoalsToFile);
    connect(ui->guestGoalsSpinBox, &QSpinBox::editingFinished, this, &MainWindow::WriteGuestTeamGoalsToFile);

    ui->lcdNumber->display(this->formattedTime());
    this->WriteHomeTeamToFile();
    this->WriteGuestTeamToFile();
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
    count = 0;
    ui->lcdNumber->display(this->formattedTime());

    ui->homeNameEdit->setText("");
    ui->guestNameEdit->setText("");
    ui->homeGoalsSpinBox->setValue(0);
    ui->guestGoalsSpinBox->setValue(0);
    ui->legComboBox->setCurrentIndex(0);

    this->WriteHomeTeamToFile();
    this->WriteGuestTeamToFile();
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

void MainWindow::WriteHomeTeamToFile()
{
    this->writeToFile("/home/bjoern/home.txt", ui->homeNameEdit->text().toStdString());
}

void MainWindow::WriteGuestTeamToFile()
{
    this->writeToFile("/home/bjoern/guest.txt", ui->guestNameEdit->text().toStdString());
}

void MainWindow::WriteHomeTeamGoalsToFile()
{
    this->writeToFile("/home/bjoern/homeGoals.txt", ui->homeGoalsSpinBox->text().toStdString());
}

void MainWindow::WriteGuestTeamGoalsToFile()
{
    this->writeToFile("/home/bjoern/guestGoals.txt", ui->guestGoalsSpinBox->text().toStdString());
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

void MainWindow::writeTimeToFile()
{
    std::string formattedTime = this->formattedTimeAsNormalString();
    if (count >= 0) ui->lcdNumber->display(QString::fromStdString(formattedTime));
    this->writeToFile("/home/bjoern/timer.txt", formattedTime);
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    count--;
    this->writeTimeToFile();
    event->accept();
}

