#include "mainclient.h"
#include "ui_mainclient.h"
#include "threadrecept.h"
#include <QtGui>
#include <QTcpSocket>
#include <QTimer>

MainClient::MainClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainClient)
{
    ui->setupUi(this);
    ui->pbCreeChat->hide();
    ui->pbConnection->hide();
    ui->gbChatRoom->hide();

}

MainClient::~MainClient()
{
    sockClient->close();
    delete ui;

}


void MainClient::on_pbRequeteChat_clicked()
{
    //Connection au Serveur Chat et envoie de la requette
    QMessageBox *msgBox = new QMessageBox;
    msgBox->setText("!!!Erreur lors de la connection au server!!!");
    msgBox->setInformativeText("Le  numero d'IP entrer n'est pas valide");
    msgBox->setStandardButtons(QMessageBox::Cancel);
    msgBox->setDefaultButton(QMessageBox::Cancel);
    sockClient = new QTcpSocket;
    try
    {
        sockClient->connectToHost(ui->leHostConnect->text(),60123);
        if(!sockClient->waitForConnected(1000))
        {
            msgBox->exec();
        }
        else
        {
            ui->pbCreeChat->show();
            ui->pbConnection->show();
        }
    }
    catch (...)
    {
        msgBox->exec();
    }

}
void MainClient::on_pbConnection_clicked()
{
    QMessageBox *msgBox = new QMessageBox;
        msgBox->setText("!!!Erreur lors de la connection au server!!!");
        msgBox->setInformativeText("Le nom de la ChatRoom est incorrect ou le username est déjà pris");
        msgBox->setStandardButtons(QMessageBox::Cancel);
        msgBox->setDefaultButton(QMessageBox::Cancel);
    QString Connect = "Con#" + ui->leConnect->text() +"#"+ui->leUser->text();
    QString Validation ="Accp";
    sockClient->write(Connect.toAscii());

    while (sockClient->waitForReadyRead(100)) // Attente des données pendant 0.1 sec maximum
        baReception.append(sockClient->read(sockClient->bytesAvailable())); // Lecture des données
    if( Validation.toAscii() != baReception)
    {
        msgBox->exec();
    }
    else
    {
      ui->gbConnection->hide();
      ui->gbChatRoom->show();
      ThreadRecept *clientLect = new ThreadRecept(sockClient);
      connect(this, SIGNAL(DataReceive(QByteArray)),clientLect,SLOT(FonctionLecture(QByteArray)));
    }
}
void MainClient::on_pbCreeChat_clicked()
{
    //création du chat

    QString Creation = "Cre#" + ui->leCreeChat->text();
    sockClient->write(Creation.toAscii());

    //attente du serveur
    sockClient->waitForReadyRead();

    //connection au chatroom nouvellement créée
    QMessageBox *msgBox = new QMessageBox;
        msgBox->setText("!!!Erreur lors de la connection au server!!!");
        msgBox->setInformativeText("Le nom de la ChatRoom est incorrect ou est déjà pris");
        msgBox->setStandardButtons(QMessageBox::Cancel);
        msgBox->setDefaultButton(QMessageBox::Cancel);
    QString Connect = "Con#" + ui->leConnect->text() +"#"+ui->leUser->text();
    QString Validation ="Accp";
    sockClient->write(Connect.toAscii());

    while (sockClient->waitForReadyRead(100)) // Attente des données pendant 0.1 sec maximum
        baReception.append(sockClient->read(sockClient->bytesAvailable())); // Lecture des données
    if( Validation.toAscii() != baReception)
    {
        msgBox->exec();
    }
    else
    {
      ui->gbConnection->hide();
      ui->gbChatRoom->show();
    }
}

