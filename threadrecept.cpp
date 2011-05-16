#include "threadrecept.h"
#include "mainclient.h"
#include "ui_mainclient.h"
#include <QTcpSocket>
ThreadRecept::ThreadRecept(QTcpSocket *sockclient) :  QThread()
{
    sockClient= sockclient;
    m_etat=true;
}
void ThreadRecept::FonctionLecture(QByteArray baReception)
{
    while(m_etat)
    {
        baReception.clear();
             sockClient->waitForReadyRead(-1); // Attente des données pendant 0.1 sec maximum
             baReception.append(sockClient->read(sockClient->bytesAvailable())); // Lecture des données


        if(QString(baReception.left(4)) == QString("Use#"))
        {
            emit(On_Lecture(QString(baReception.right(baReception.length()-4))));
        }

    }

  }
void ThreadRecept::SortieChat()
{
    m_etat = false;
}
