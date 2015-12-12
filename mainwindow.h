#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "track/track.h"
#include "tiasound/tiasound.h"
#include "emulation/player.h"

#include <QList>


namespace Ui {

class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
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

signals:
    void playInstrument(Track::Instrument *instrument, int frequency);
    void stopInstrument();

private slots:

    // Gets signalled if a valid key from the piano is pressed
    void newPianoKeyPressed(int frequency);
    // Gets signalled if a valid key from the piano has been released
    void pianoKeyReleased();

private:
    Ui::MainWindow *ui;
    Track::Track *pTrack;
};

#endif // MAINWINDOW_H
