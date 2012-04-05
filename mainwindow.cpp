/***********************************************************
 * SOURCE FILE: mainwindow.cpp
 *
 * PROGRAM: PortForwarder
 *
 * FUNCTIONS:
 * MainWindow(QWidget *parent);
 * ~MainWindow();
 * void parseConfFile();
 * void addToTable(QString service, QString listenPort, QString ip);
 * void startListener(int service, int listenPort, QString target);
 * void updateStatus(QString port, bool running);
 * void on_actionExit_triggered();
 * void on_addB_clicked();
 *
 * DATE: MARCH 11, 2012
 *
 * DESIGNER: Karl Castillo
 *
 * PROGRAMMER: Karl Castillo
 *
 * NOTES:
 * The Forwarding class is the class that will listen for
 * connections from clients and will do the forwarding for a
 * specific service.
 ***********************************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QTextStream>
#include <QStringList>
#include <QThread>
#include <QList>
#include <QMessageBox>

/***********************************************************
 * FUNCTION: MainWindow()
 *
 * DATE: March 11, 2012
 *
 * REVISIONS: (Date and Description)
 *
 * DESIGNER: Karl Castillo
 *
 * PROGRAMMER: Karl Castillo
 *
 * INTERFACE: MainWindow(QWidget *parent)
 *			   parent: pointer to the parent widget
 *
 * RETURN: void
 *
 * NOTES:
 * The MainWindow class constructor.
 ***********************************************************/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    changed(false)
{
    QStringList headers;
    headers.append("Service/Port");
    headers.append("Listening Port");
    headers.append("Target");
    headers.append("Status");

    ui->setupUi(this);

    for(int i = 0; i < headers.size(); i++) {
        ui->portmapT->insertColumn(i);
        ui->portmapT->setHorizontalHeaderItem(i, new QTableWidgetItem(headers.at(i)));
    }

    ui->portmapT->horizontalHeader()->setStretchLastSection(true);

    parseConfFile();
}

/***********************************************************
 * FUNCTION: ~MainWindow()
 *
 * DATE: March 11, 2012
 *
 * REVISIONS: (Date and Description)
 *
 * DESIGNER: Karl Castillo
 *
 * PROGRAMMER: Karl Castillo
 *
 * INTERFACE: ~MainWindow()
 *
 * RETURN: void
 *
 * NOTES:
 * The MainWindow class destructor.
 ***********************************************************/
MainWindow::~MainWindow()
{
    delete ui;
}

/***********************************************************
 * FUNCTION: parseConfFile()
 *
 * DATE: March 11, 2012
 *
 * REVISIONS: (Date and Description)
 *
 * DESIGNER: Karl Castillo
 *
 * PROGRAMMER: Karl Castillo
 *
 * INTERFACE: void parseConfFile()
 *
 * RETURN: void
 *
 * NOTES:
 * The function that will parse the config file and start the
 * listener.
 ***********************************************************/
void MainWindow::parseConfFile()
{
    QFile conf(QDir::homePath() + "/forwarder.conf");
    QTextStream *stream;
    QString line;
    QStringList lineList;

    int port;
    int listenPort;

    if(!conf.open(QIODevice::ReadOnly)) {
        qDebug() << "Cannot open: " << conf.fileName();
        conf.close();
        return;
    }

    stream = new QTextStream(&conf);
    while(!stream->atEnd()) {
        line = stream->readLine();
        lineList = line.split(",");

        port = lineList[0].toInt();
        listenPort = lineList[1].toInt();

        startListener(port, listenPort, lineList[2]);
        addToTable(lineList[0], lineList[1], lineList[2]);
    }

    conf.close();

    delete stream;

}

/***********************************************************
 * FUNCTION: addToTable()
 *
 * DATE: March 11, 2012
 *
 * REVISIONS: (Date and Description)
 *
 * DESIGNER: Karl Castillo
 *
 * PROGRAMMER: Karl Castillo
 *
 * INTERFACE: void addToTable()
 *
 * RETURN: void
 *
 * NOTES:
 * The function that will create the table that has information
 * about the target, service and listening port.
 ***********************************************************/
void MainWindow::addToTable(QString service, QString listenPort, QString ip)
{
    int numRows = ui->portmapT->rowCount();

    ui->portmapT->insertRow(numRows);
    ui->portmapT->setItem(numRows, 0, new QTableWidgetItem(service));
    ui->portmapT->setItem(numRows, 1, new QTableWidgetItem(listenPort));
    ui->portmapT->setItem(numRows, 2, new QTableWidgetItem(ip));
    ui->portmapT->setItem(numRows, 3, new QTableWidgetItem("Not Running"));
    ui->portmapT->item(numRows,3)->setForeground(Qt::red);
}

/***********************************************************
 * FUNCTION: startListener()
 *
 * DATE: March 11, 2012
 *
 * REVISIONS: (Date and Description)
 *
 * DESIGNER: Karl Castillo
 *
 * PROGRAMMER: Karl Castillo
 *
 * INTERFACE: void startListener(int service, int listenPort, QString target)
 *             service: the service that will be forwarded
 *             listenPort: the port that the forwarder will listen on
 *             target: the target IP address
 *
 * RETURN: void
 *
 * NOTES:
 * The function that will instantiate a Forwarding class and start
 * the listener.
 ***********************************************************/
void MainWindow::startListener(int service, int listenPort, QString target)
{
    Forwarding *forwarding;
    QThread *thread;

    forwarding = new Forwarding(service, listenPort, target);
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
    emit startServiceListener(service);
    forwardingMap.insert(service, forwarding);
}

/***********************************************************
 * FUNCTION: updateStatus()
 *
 * DATE: March 11, 2012
 *
 * REVISIONS: (Date and Description)
 *
 * DESIGNER: Karl Castillo
 *
 * PROGRAMMER: Karl Castillo
 *
 * INTERFACE: void updateStatus(QString port, bool running)
 *             port: the service that will being forwarded
 *             running: true if the forwarding is running; false if not
 *
 * RETURN: void
 *
 * NOTES:
 * The function that will instantiate a Forwarding class and start
 * the listener.
 ***********************************************************/
void MainWindow::updateStatus(QString port, bool running)
{
    QList<QTableWidgetItem *> items = ui->portmapT->findItems(port, Qt::MatchExactly);
    QTableWidgetItem *cell;

    if(items.size() > 0) {
        cell = ui->portmapT->item(items[0]->row(), 3);
        if(running) {
            cell->setForeground(Qt::green);
            cell->setText("Running");
        } else {
            cell->setForeground(Qt::red);
            cell->setText("Not Running");
        }
    }
}

/***********************************************************
 * FUNCTION: on_actionExit_triggered()
 *
 * DATE: March 11, 2012
 *
 * REVISIONS: (Date and Description)
 *
 * DESIGNER: Karl Castillo
 *
 * PROGRAMMER: Karl Castillo
 *
 * INTERFACE: void on_actionExit_triggered()
 *
 * RETURN: void
 *
 * NOTES:
 * The function that will close the application when the menuitem
 * is clicked
 ***********************************************************/
void MainWindow::on_actionExit_triggered()
{
    this->close();
}

/***********************************************************
 * FUNCTION: on_addB_clicked()
 *
 * DATE: March 11, 2012
 *
 * REVISIONS: (Date and Description)
 *
 * DESIGNER: Karl Castillo
 *
 * PROGRAMMER: Karl Castillo
 *
 * INTERFACE: void on_addB_clicked()
 *
 * RETURN: void
 *
 * NOTES:
 * This function creates a new row in the table and starts the
 * listener for the added rule.
 ***********************************************************/
void MainWindow::on_addB_clicked()
{
    int service;
    int listenPort;
    bool error = false;
    QString target;


    service = ui->serviceE->value();
    listenPort = ui->listenE->value();

    if((target = ui->targetE->text()).isEmpty()) {
        error = true;
        return;
    }

    if(!error) {
        ui->serviceE->setValue(0);
        ui->listenE->setValue(0);
        ui->targetE->clear();

        addToTable(QString::number(service), QString::number(listenPort), target);
        startListener(service, listenPort, target);
        changed = true;
    } else {
        QMessageBox::warning(this, tr("Error"),
                             tr("Target IP must not be blank"),
                             QMessageBox::Ok);
    }
}

void MainWindow::on_delB_clicked()
{
    QModelIndexList selected = ui->portmapT->selectionModel()->selectedRows();
    int row = selected.at(0).row();

    qDebug() << row;
    qDebug() << ui->portmapT->item(row, 0)->text();

    if(selected.size() > 0) {
        delete forwardingMap.value(ui->portmapT->item(row, 0)->text().toInt());
        ui->portmapT->removeRow(row);
    }
}
