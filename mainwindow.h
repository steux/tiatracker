#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "track/track.h"
#include "tiasound/tiasound.h"
#include "tiasound/pitchguide.h"
#include "tiasound/pitchguidefactory.h"
#include "emulation/player.h"

#include <QList>
#include <QMenu>


namespace Ui {

class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static const int version = 1;

    /* Solarized-inspired colors */
    static const QColor dark;
    static const QColor darkHighlighted;
    static const QColor light;
    static const QColor lightHighlighted;
    static const QColor contentDark;
    static const QColor contentDarker;
    static const QColor contentLight;
    static const QColor contentLighter;
    static const QColor red;
    static const QColor orange;
    static const QColor blue;
    static const QColor violet;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    /* Initializes signal/slot connections */
    void initConnections();

    void registerTrack(Track::Track *newTrack);

    /* Get current pitch guide */
    TiaSound::PitchGuide *getPitchGuide();

signals:
    void playInstrument(Track::Instrument *instrument, int frequency);
    void stopInstrument();
    void playPercussion(Track::Percussion *percussion);
    void stopPercussion();

private slots:
    // Set a new pitch guide for the piano keyboard
    void setWaveform(TiaSound::Distortion dist);

    // Gets signalled if a valid key from the piano is pressed
    void newPianoKeyPressed(int frequency);
    // Gets signalled if a valid key from the piano has been released
    void pianoKeyReleased();

    void on_tabWidget_currentChanged(int index);

    // Signals for insert/delete context menu events for shapers
    void waveformContextEvent(int frame);
    void insertFrameBefore(bool);
    void insertFrameAfter(bool);
    void deleteFrame(bool);


    void on_actionSave_triggered();

    void on_actionSave_as_triggered();

    void on_actionOpen_triggered();

private:
    /* Tab index values */
    static const int iTabTrack = 0;
    static const int iTabInstruments = 1;
    static const int iTabPercussion = 2;
    static const int iTabInfo = 3;
    static const int iTabOptions = 4;

    Ui::MainWindow *ui = nullptr;
    Track::Track *pTrack = nullptr;
    TiaSound::PitchGuideFactory pgFactory;
    TiaSound::PitchGuide *pPitchGuide = pgFactory.getPitchPerfectPalGuide();

    QMenu waveformContextMenu{this};
    QAction actionInsertBefore{"Insert frame before", this};
    QAction actionInsertAfter{"Insert frame after", this};
    QAction actionDelete{"Delete frame", this};
    int waveformContextFrame;
};

#endif // MAINWINDOW_H
