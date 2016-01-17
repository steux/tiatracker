#include "timeline.h"

Timeline::Timeline(QWidget *parent) : QWidget(parent)
{
    setFixedWidth(2*channelWidth + channelMargin);
}

/*************************************************************************/

void Timeline::registerTrack(Track::Track *newTrack) {
    pTrack = newTrack;
}

