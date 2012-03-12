#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

private slots:
    void on_actionExit_triggered();

private:
    Ui::MainWindow *ui;
    QMap<int, QString> portmap;

    void parseConfFile();
    void QDebug(QString mesg);
    void addToTable(QString service, QString ip);
    void startListeners();
};

#endif // MAINWINDOW_H
