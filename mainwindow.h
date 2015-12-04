#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "track/track.h"
#include "tiasound/tiasound.h"
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

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void registerTrack(Track::Track *newTrack);

    /* Initializes the GUI components. Must be called once during init. */
    void initInstrumentsTab();

    /* Initializes signal/slot connections */
    void initConnections();

    /* Fills GUI elements with data from the track. Called upon changes. */
    void updateInstrumentsTab();

    /* Returns the currently selected instrument from the instruments tab */
    int getSelectedInstrument();

private slots:
    void on_buttonInstrumentDelete_clicked();

    void on_spinBoxInstrumentEnvelopeLength_editingFinished();
    void on_spinBoxInstrumentEnvelopeLength_valueChanged(int newLength);

    void on_spinBoxSustainStart_editingFinished();
    void on_spinBoxSustainStart_valueChanged(int newStart);

    void on_spinBoxReleaseStart_editingFinished();
    void on_spinBoxReleaseStart_valueChanged(int newStart);

    void on_spinBoxInstrumentVolume_editingFinished();
    void on_spinBoxInstrumentVolume_valueChanged(int newVolume);

private:
    static const QList<TiaSound::Distortion> availableWaveforms;
    static const int maxInstrumentNameLength = 64;

    Track::Track *pTrack = nullptr;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
