#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QThread>


namespace Emulation {

class Player : public QThread {
    Q_OBJECT

public:
    explicit Player(QObject *parent = 0);

public slots:

signals:

private:

};

}

#endif // PLAYER_H
