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

private:
    Ui::MainClient *ui;
    QTcpSocket *sockClient;
    QByteArray baReception;
};

#endif // MAINCLIENT_H
