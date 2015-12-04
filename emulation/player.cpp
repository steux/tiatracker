#include <QThread>

#include "player.h"

namespace Emulation {

Player::Player(QObject *parent) : QThread(parent)
{
    moveToThread(this);
}

}
