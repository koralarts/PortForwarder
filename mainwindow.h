#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "forwarding.h"
#include <QMainWindow>
#include <QMap>
#include <QString>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void updateStatus(QString port, bool running);

private slots:
    void on_actionExit_triggered();
    void on_addB_clicked();
    void on_delB_clicked();

private:
    Ui::MainWindow *ui;
    QMap <int, Forwarding*> forwardingMap;
    bool changed;

    void parseConfFile();
    void addToTable(QString service, QString listenPort, QString ip);
    void startListener(int service, int listenPort, QString target);

signals:
    void startServiceListener(int);
};

#endif // MAINWINDOW_H
