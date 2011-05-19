#ifndef THREADRECEPT_H
#define THREADRECEPT_H

#include <QMainWindow>
#include <QtGui>
#include <QThread>
#include <QTcpSocket>

class ThreadRecept : public QThread
{
    Q_OBJECT
public:
    explicit ThreadRecept(QTcpSocket *sockclient);

public slots:
  //  void Lecture();
    void SortieChat();

    void run();

signals:
    void Lecture(QString);

private:
     bool m_etat;
     QTcpSocket *sockClient;
public slots:

};

#endif // THREADRECEPT_H
