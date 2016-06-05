/* TIATracker, (c) 2016 Andre "Kylearan" Wichmann.
 * Website: https://bitbucket.org/kylearan/tiatracker
 * Email: andre.wichmann@gmx.de
 * See the file "license.txt" for information on usage and redistribution
 * of this file.
 */

#ifndef PERCUSSIONTAB_H
#define PERCUSSIONTAB_H

#include <QObject>
#include <QWidget>
#include "track/track.h"
#include "track/instrument.h"
#include "emulation/player.h"


class PercussionTab : public QWidget
{
    Q_OBJECT
public:
    static const QList<TiaSound::Distortion> availableWaveforms;

    static const int maxPercussionNameLength = 64;

    explicit PercussionTab(QWidget *parent = 0);

    void registerTrack(Track::Track *newTrack);

    /* Initializes the GUI components. Must be called once during init. */
    void initPercussionTab();

    /* Fills GUI elements with data from the track. Called upon changes. */
    void updatePercussionTab();

    /* Connects the tab with the Player */
    void connectPlayer(Emulation::Player *player);

    /* Returns the currently selected percussion from the percussions tab */
    int getSelectedPercussionIndex();
    Track::Percussion * getSelectedPercussion();

    QString curPercussionDialogPath;

signals:
    void playWaveform(TiaSound::Distortion waveform, int frequency, int volume);

public slots:
    void on_buttonPercussionExport_clicked();
    void on_buttonPercussionImport_clicked();
    void on_buttonPercussionDelete_clicked();

    void on_spinBoxPercussionLength_editingFinished();
    void on_spinBoxPercussionLength_valueChanged(int newLength);

    void on_checkBoxOverlay_stateChanged(int);

    void on_spinBoxPercussionVolume_editingFinished();
    void on_spinBoxPercussionVolume_valueChanged(int newVolume);

    void on_comboBoxPercussion_currentIndexChanged(int);
    void on_comboBoxPercussion_currentTextChanged(const QString &text);

    void newPercussionValue(int iFrame);

protected:

private:
    Track::Track *pTrack = nullptr;

};

#endif // PERCUSSIONTAB_H
