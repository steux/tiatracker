#include "tracktab.h"
#include <QPushButton>
#include <QLayout>
#include <QButtonGroup>
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
    QButtonGroup *group = new QButtonGroup();

    int id = 0;
    foreach (Track::Instrument ins, pTrack->instruments) {
        QPushButton *insButton = new QPushButton(ins.name, this);
        insButton->setCheckable(true);
        if (id == 0) {
            insButton->setChecked(true);
        }
        insLayout->addWidget(insButton);
        group->addButton(insButton, id++);
    }
    foreach (Track::Percussion perc, pTrack->percussion) {
        QPushButton *percButton = new QPushButton(perc.name, this);
        percButton->setCheckable(true);
        insLayout->addWidget(percButton);
        group->addButton(percButton, id++);
    }
    QWidget *insSelector = findChild<QWidget *>("trackInstrumentSelector");
    insSelector->setLayout(insLayout);
}

/*************************************************************************/

void TrackTab::updateTrackTab() {

}

