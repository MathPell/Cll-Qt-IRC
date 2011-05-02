#include "threadrecept.h"
#include "mainclient.h"
#include "ui_mainclient.h"
#include <QTcpSocket>
ThreadRecept::ThreadRecept(QTcpSocket *sockclient) :  QThread()
{
    socket = sockclient;
}
void ThreadRecept::FonctionLecture(QByteArray bRecue)
{

}
