#include "tracktab.h"
#include <QPushButton>
#include <QLayout>
#include <iostream>

TrackTab::TrackTab(QWidget *parent) : QWidget(parent)
{

}

/*************************************************************************/

void TrackTab::registerTrack(Track::Track *newTrack) {
    pTrack = newTrack;
}

/*************************************************************************/

void TrackTab::initTrackTab() {
    QLayout *insLayout = new QVBoxLayout;
    foreach (Track::Instrument ins, pTrack->instruments) {
        QPushButton *insButton = new QPushButton(ins.name, this);
        insLayout->addWidget(insButton);
    }
    foreach (Track::Percussion perc, pTrack->percussion) {
        QPushButton *percButton = new QPushButton(perc.name, this);
        insLayout->addWidget(percButton);
    }
    QWidget *insSelector = findChild<QWidget *>("trackInstrumentSelector");
    insSelector->setLayout(insLayout);
}

/*************************************************************************/

void TrackTab::updateTrackTab() {

}

