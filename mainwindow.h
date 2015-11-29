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
    void initInstrumentsTab();
    int getSelectedInstrument();

private slots:
    void on_buttonInstrumentDelete_clicked();

    void on_spinBoxInstrumentEnvelopeLength_valueChanged(int newLength);

private:
    static const QList<TiaSound::Distortion> availableWaveforms;
    static const int maxInstrumentNameLength = 64;

    Track::Track *pTrack = nullptr;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
