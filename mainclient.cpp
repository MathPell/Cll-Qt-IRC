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
    ui->lblUsername->hide();
    ui->leConnect->hide();
    ui->leCreeChat->hide();
    ui->leUser->hide();
    ui->gbChatRoom->hide();
    m_etat= true;
}

MainClient::~MainClient()
{
    if(sockClient->isOpen())
        sockClient->close();
    delete ui;
}


void MainClient::on_pbRequeteChat_clicked()
{
   //Connection au Serveur Chat et envoie de la requette

    sockClient = new QTcpSocket;
    try
    {
        sockClient->connectToHost(ui->leHostConnect->text(),60123);

        if(!sockClient->waitForConnected(1000))
        {
            AfficherMsgBox("Erreur lors de la connection au serveur.","Le numéro de l'IP entré n'est pas valide.");
        }
        else
        {
            ui->pbCreeChat->show();
            ui->pbConnection->show();

            ui->lblUsername->show();
            ui->leConnect->show();
            ui->leCreeChat->show();
            ui->leUser->show();

            sockClient->waitForReadyRead(-1);
            baReception.append(sockClient->read(sockClient->bytesAvailable())); // Lecture des données
            ui->teRoom->setText(QString(baReception));
        }
    }
    catch (...)
    {
        AfficherMsgBox("Erreur lors de la connection au serveur.","Le numéro de l'IP entré n'estp as valide.");
    }

}
void MainClient::on_pbConnection_clicked()
{
    //Connection à une ChatRoom

    //Création de la trame Connect et envoie de celle-ci
    QString Connect = "Con#" +ui->leCreeChat->text() +"#"+ui->leUser->text();
    sockClient->write(Connect.toAscii());
    sockClient->waitForBytesWritten();

    ////Lecture de la Validation////
    QString Validation ="Accp";
    baReception.clear();
    sockClient->waitForReadyRead(-1);// Attente des données
    baReception.append(sockClient->read(sockClient->bytesAvailable())); // Lecture des données
        QString ba(baReception);
    if(Validation.toAscii() != baReception)
    {
        AfficherMsgBox("Erreur lors de la connection au serveur.","Le nom de la ChatRoom est incorrect ou le username est déjà pris.");
    }
    else
    {
      ui->gbConnection->hide(); //Hide le groupbox connection
      ui->gbChatRoom->show(); //Show le groupbox chat
      baReception.clear();
      //baReception.append(sockClient->read(sockClient->bytesAvailable())); // Lecture des données
      //ui->teUser->setText(QString(baReception));//Lecture des Users
     // Lecture();
     ThreadRecept *clientLect = new ThreadRecept(sockClient);
     connect(this, SIGNAL(DataReceive(QByteArray)),clientLect,SLOT(FonctionLecture(QByteArray)));
     connect(clientLect, SIGNAL(On_Lecture(QString)),clientLect,SLOT(Lecture(QString)));
     connect(this,SIGNAL(SortieChat()),clientLect,SLOT(SortieChat()));
 }
    }

void MainClient::on_pbCreeChat_clicked()
{
    //Création d'un ChatRoom

    //Création de la trame Creation et envoie
    QString Creation = "Cre#" + ui->leCreeChat->text();
    sockClient->write(Creation.toAscii());
    sockClient->waitForBytesWritten();

    //Lecture de la Validation
    baReception.clear();
    QString Validation ="Accp";
    sockClient->waitForReadyRead(-1);// Attente des données
    baReception.append(sockClient->read(sockClient->bytesAvailable())); // Lecture des données
    QString ba(baReception);
    if( Validation.toAscii() != baReception)
    sockClient->waitForReadyRead(-1); //Attente des données pendant 0.1 sec maximum
    baReception.append(sockClient->read(sockClient->bytesAvailable())); //Lecture des données

    //Si pas accepté
    if( QString("Accp").toAscii() != baReception)
    {
        AfficherMsgBox("Erreur lors de la création de la ChatRoom.","Le nom de la ChatRoom est déjà pris.");
    }
    else
    {
        AfficherMsgBox("Aucune erreur lors de la création de la ChatRoom .","Connexion...");
        on_pbConnection_clicked();
    }
}

void MainClient::Lecture(QString Envoie)
{
    ui->teChatRoom->setText(Envoie);

}

void MainClient::on_pbFermerChat_clicked()
{
    m_etat=false;   
    sockClient->disconnectFromHost(); //Annonce de déconnexion au serveur
    sockClient->close(); //Fermeture du socket
    ui->pbCreeChat->hide();
    ui->pbConnection->hide();
    ui->gbChatRoom->hide();
    emit(SortieChat());
}

void MainClient::on_pbEnvoie_clicked()
{
    QString Envoie = "Env#"+ui->leUser->text()+"-> "+ ui->teChatEnvoie->toPlainText();
    sockClient->write(Envoie.toAscii());
    sockClient->waitForBytesWritten();
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

void MainClient::AfficherMsgBox(QString strText, QString strInformation)
{
    QMessageBox *msgBox = new QMessageBox;
    msgBox->setText(strText);
    msgBox->setInformativeText(strInformation);
    msgBox->setStandardButtons(QMessageBox::Cancel);
    msgBox->setDefaultButton(QMessageBox::Cancel);
    msgBox->exec();
}
