#ifndef OPTIONSTAB_H
#define OPTIONSTAB_H

#include <QObject>
#include <QWidget>
#include "track/track.h"
#include "emulation/player.h"
#include "tiasound/pitchguide.h"
#include "tiasound/pitchguidefactory.h"
#include <QList>


class OptionsTab : public QWidget
{
    Q_OBJECT
public:
    explicit OptionsTab(QWidget *parent = 0);

    void registerTrack(Track::Track *newTrack);

    /* Initializes the GUI components. Must be called once during init. */
    void initOptionsTab();

    /* Fills GUI elements with data from the track. Called upon changes. */
    void updateOptionsTab();


    QList<TiaSound::PitchGuide> guides{};

signals:
    void setTVStandard(TiaSound::TvStandard newStandard);
    void setPitchGuide(TiaSound::PitchGuide newGuide);

public slots:

private:

    Track::Track *pTrack = nullptr;

private slots:
    void on_comboBoxPitchGuide_currentIndexChanged(int index);

    void on_radioButtonPal_toggled(bool checked);

};

#endif // OPTIONSTAB_H
