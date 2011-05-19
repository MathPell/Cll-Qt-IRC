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

    void Lecture(QString);

    void on_pbFermerChat_clicked();

    void on_pbEnvoie_clicked();

    void on_teChatEnvoie_textChanged();

    void on_teUser_textChanged();

    void on_sbNbCracMax_valueChanged(QString );

    void on_sbUtilisateurMax_valueChanged(QString );

    void AfficherMsgBox(QString , QString);

signals:
  //  void DataReceive(QByteArray);
    void SortieChat();
   // void DebutLecture(QTcpSocket*);
private:
    Ui::MainClient *ui;
    QTcpSocket *sockClient;
    QByteArray baReception;
    bool m_etat;
    QString m_strCaracMax;
    QString m_strUtilisateur;
};

#endif // MAINCLIENT_H
