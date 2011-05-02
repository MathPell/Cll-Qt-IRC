#ifndef THREADRECEPT_H
#define THREADRECEPT_H

#include <QThread>

class ThreadRecept : public QThread
{
    Q_OBJECT
public:
    explicit ThreadRecept(QObject *parent = 0);

signals:

public slots:

};

#endif // THREADRECEPT_H
