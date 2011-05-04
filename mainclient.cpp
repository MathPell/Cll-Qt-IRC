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
   ////Connection au Serveur Chat et envoie de la requette////


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
    ////Connection à une ChatRoom////


    QMessageBox *msgBox = new QMessageBox;
        msgBox->setText("!!!Erreur lors de la connection au server!!!");
        msgBox->setInformativeText("Le nom de la ChatRoom est incorrect ou le username est déjà pris");
        msgBox->setStandardButtons(QMessageBox::Cancel);
        msgBox->setDefaultButton(QMessageBox::Cancel);

        //Création de la trame Connect et envoie//
    QString Connect = "Con#" +ui->lwChatRoom->currentItem()->text() +"#"+ui->leUser->text();
    sockClient->write(Connect.toAscii());

    ////Lecture de la Validation////
    QString Validation ="Accp";
    while (sockClient->waitForReadyRead(100)) // Attente des données pendant 0.1 sec maximum
        baReception.append(sockClient->read(sockClient->bytesAvailable())); // Lecture des données
    if( Validation.toAscii() != baReception)
    {
        msgBox->exec();
    }
    else
    {
      ui->gbConnection->hide();//Hide le groupbox connection
      ui->gbChatRoom->show();//show le groupbox chat
      baReception.clear();
      baReception.append(sockClient->read(sockClient->bytesAvailable())); // Lecture des données
      ui->teUser->setText(QString::append(baReception));//Lecture des Users
      Lecture();
     //ThreadRecept *clientLect = new ThreadRecept(sockClient);
     // connect(this, SIGNAL(DataReceive(QByteArray)),clientLect,SLOT(FonctionLecture(QByteArray)));
    }
}
void MainClient::on_pbCreeChat_clicked()
{
    ////création du chat////


    QMessageBox *msgBox = new QMessageBox;
        msgBox->setText("!!!Erreur lors de la création de la ChatRoom!!!");
        msgBox->setInformativeText("Le nom de la ChatRoom est déjà pris");
        msgBox->setStandardButtons(QMessageBox::Cancel);
        msgBox->setDefaultButton(QMessageBox::Cancel);

       ////Création de la trame Creation et envoie////
    QString Creation = "Cre#" + ui->leCreeChat->text();
    sockClient->write(Creation.toAscii());

        ////Lecture de la Validation////

    baReception.clear();
    QString Validation ="Accp";
    while (sockClient->waitForReadyRead(100)) // Attente des données pendant 0.1 sec maximum
        baReception.append(sockClient->read(sockClient->bytesAvailable())); // Lecture des données
    if( Validation.toAscii() != baReception)
    {
         msgBox->exec();
    }
    else
    {
         on_pbConnection_clicked();
    }
}
void MainClient::Lecture()
{
   QByteArray baReception;
   QString Validation ="Use#";
    while(m_etat)
    {
        int i =0;
        QString SerValid = "";
        baReception.clear();
        while (sockClient.waitForReadyRead(100)) // Attente des données pendant 0.1 sec maximum
             baReception.append(sockClient.read(sockClient.bytesAvailable())); // Lecture des données

        SerValid.append(baReception);
        SerValid.truncate(4);
        if(SerValid == Validation)
        {
            baReception.truncate();
            ui->teUser->setText(QString::startsWith());
        }
        ui->teChatRoom->setText(QString::append(baReception));

    }
    sockClient.disconnectFromHost(); // Annonce de déconnexion au serveur
    sockClient.close(); // Fermeture du socket
    ui->pbCreeChat->hide();
    ui->pbConnection->hide();
    ui->gbChatRoom->hide();
}


void MainClient::on_pbFermerChat_clicked()
{
    m_etat=false;
}

void MainClient::on_pbEnvoie_clicked()
{
   sockClient->write("Env#"+ui->leUser->text()+"-> "+ ui->teChatEnvoie->toPlainText());
}
