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

    connect(ui->quitButton, &QPushButton::clicked, qApp, &QApplication::quit);
    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::Clear);
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::Start);
    connect(ui->setButton, &QPushButton::clicked, this, &MainWindow::Set);

    ui->lcdNumber->display(this->formattedTime());
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
}

void MainWindow::Set()
{
    auto timeToBeSet = ui->timeEdit->time();
    count = timeToBeSet.minute() * 60 + timeToBeSet.second();
    ui->lcdNumber->display(this->formattedTime());
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

void MainWindow::timerEvent(QTimerEvent *event)
{
    count--;
    std::string formattedTime = this->formattedTimeAsNormalString();

    if (count >= 0) ui->lcdNumber->display(QString::fromStdString(formattedTime));

    std::ofstream outputFile("/home/bjoern/test.txt");
    outputFile << formattedTime;
    outputFile.close();

    event->accept();
}

