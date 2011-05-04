#ifndef MAINCLIENT_H
#define MAINCLIENT_H

#include <QMainWindow>
#include <QtGui>
#include <QTcpSocket>

namespace Ui {
    class MainClient;
}

class MainClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainClient(QWidget *parent = 0);
    ~MainClient();

private slots:
    void on_pbConnection_clicked();

    void on_pbRequeteChat_clicked();

    void on_pbCreeChat_clicked();

    void Lecture();

    void on_pbFermerChat_clicked();

    void on_pbEnvoie_clicked();

signals:
    void DataReceive(QByteArray);
private:
    Ui::MainClient *ui;
    QTcpSocket *sockClient;
    QByteArray baReception;
    bool m_etat;
};

#endif // MAINCLIENT_H
