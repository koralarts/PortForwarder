#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "forwarding.h"
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QTextStream>
#include <QStringList>
#include <QThread>
#include <QList>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QStringList headers;
    headers.append("Service/Port");
    headers.append("Target");
    headers.append("Status");

    ui->setupUi(this);

    for(int i = 0; i < headers.size(); i++) {
        ui->portmapT->insertColumn(i);
        ui->portmapT->setHorizontalHeaderItem(i, new QTableWidgetItem(headers.at(i)));
    }

    ui->portmapT->horizontalHeader()->setStretchLastSection(true);

    parseConfFile();
    startListeners();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/****************** HELPER FUNCTIONS ***********************/
void MainWindow::parseConfFile()
{
    QFile conf(QDir::homePath() + "/forwarder.conf");
    QTextStream *stream;
    QString line;
    QStringList lineList;

    if(!conf.open(QIODevice::ReadOnly)) {
        QDebug("Cannot open: " + conf.fileName());
        conf.close();
        return;
    }

    stream = new QTextStream(&conf);
    while(!stream->atEnd()) {
        line = stream->readLine();
        lineList = line.split(",");

        portmap.insert(lineList[0].toInt(), lineList[1]);
        addToTable(lineList[0], lineList[1]);
    }

    conf.close();

    delete stream;

}

void MainWindow::QDebug(QString mesg)
{
    qDebug() << mesg;
}

void MainWindow::addToTable(QString service, QString ip)
{
    int numRows = ui->portmapT->rowCount();

    ui->portmapT->insertRow(numRows);
    ui->portmapT->setItem(numRows, 0, new QTableWidgetItem(service));
    ui->portmapT->setItem(numRows, 1, new QTableWidgetItem(ip));
    ui->portmapT->setItem(numRows, 2, new QTableWidgetItem("Not Running"));
    ui->portmapT->item(numRows,2)->setForeground(Qt::red);
}

void MainWindow::startListeners()
{
    QMap<int, QString>::iterator it;
    Forwarding *forwarding;
    QThread *thread;

    for(it = portmap.begin(); it != portmap.end(); ++it) {
        forwarding = new Forwarding(it.key(), it.value());
        thread = new QThread();
        thread->start();
        forwarding->moveToThread(thread);

        if(!connect(this, SIGNAL(startServiceListener(int)), forwarding,
                SLOT(startListening(int))))
        {
            qDebug() << "Cannot connect listener signal";
        }
        if(!connect(forwarding, SIGNAL(isRunning(QString,bool)), this,
                SLOT(updateStatus(QString,bool))))
        {
            qDebug() << "Cannot connect isRunning signal";
        }
        emit startServiceListener(it.key());
    }
}

/****************** GENERAL SLOTS **************************/
void MainWindow::updateStatus(QString port, bool running)
{
    QList<QTableWidgetItem *> items = ui->portmapT->findItems(port, Qt::MatchExactly);
    QTableWidgetItem *cell;

    if(items.size() > 0) {
        cell = ui->portmapT->item(items[0]->row(), 2);
        if(running) {
            cell->setForeground(Qt::green);
            cell->setText("Running");
        } else {
            cell->setForeground(Qt::red);
            cell->setText("Not Running");
        }
    }
}

/****************** MENU ITEM SLOTS ************************/
void MainWindow::on_actionExit_triggered()
{
    this->close();
}
