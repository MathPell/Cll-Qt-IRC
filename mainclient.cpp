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
    m_etat= true;

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
      ui->teUser->setText(QString(baReception));//Lecture des Users
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
        baReception.clear();
        while (sockClient->waitForReadyRead(100)) // Attente des données pendant 0.1 sec maximum
             baReception.append(sockClient->read(sockClient->bytesAvailable())); // Lecture des données


        if(QString(baReception.left(4)) == Validation)
        {
            ui->teUser->setText(QString(baReception.right(5)));
        }
        ui->teChatRoom->setText(QString(baReception));
    }
    sockClient->disconnectFromHost(); // Annonce de déconnexion au serveur
    sockClient->close(); // Fermeture du socket
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
    QString Envoie = "Env#"+ui->leUser->text()+"-> "+ ui->teChatEnvoie->toPlainText();
    sockClient->write(Envoie.toAscii());
}

void MainClient::on_teChatEnvoie_textChanged()
{
    ui->txtNbDeLettres->setText(QString(ui->teChatEnvoie->toPlainText().size() + "/" + m_strCaracMax));
}

void MainClient::on_teUser_textChanged()
{
    ui->teNbDePersonne->setText(QString(ui->teUser->toPlainText().count("\n") + "/" + m_strUtilisateur));
}

void MainClient::on_sbNbCracMax_valueChanged(QString strCaracMax)
{
    m_strCaracMax = "/" + strCaracMax;
    on_teChatEnvoie_textChanged();
}

void MainClient::on_sbUtilisateurMax_valueChanged(QString strUtilisateurMax)
{
    m_strUtilisateur = "/" + strUtilisateurMax;
    on_teUser_textChanged();
}
