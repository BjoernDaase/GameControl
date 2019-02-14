#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QBasicTimer>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void Clear();
    void Start();
    void Set();
    void Set15();
    void Set30();
    void Set35();

protected:
    void timerEvent(QTimerEvent *event) override;

private:
    Ui::MainWindow *ui;
    QBasicTimer *timer;
    int countStart = 900; //Default gameTime in Seconds
    int count;
    QString formattedTime();
    std::string formattedTimeAsNormalString();
    void writeToFile(std::string file, std::string content);
};

#endif // MAINWINDOW_H
