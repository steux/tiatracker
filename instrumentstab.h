#ifndef INSTRUMENTSTAB_H
#define INSTRUMENTSTAB_H

#include <QObject>
#include <QWidget>
#include "track/track.h"
#include "track/instrument.h"

class InstrumentsTab : public QWidget
{
    Q_OBJECT
public:
    static const int maxInstrumentNameLength = 64;

    explicit InstrumentsTab(QWidget *parent = 0);

    void registerTrack(Track::Track *newTrack);

    /* Initializes the GUI components. Must be called once during init. */
    void initInstrumentsTab();

    /* Fills GUI elements with data from the track. Called upon changes. */
    void updateInstrumentsTab();

    /* Returns the currently selected instrument from the instruments tab */
    int getSelectedInstrumentIndex();
    Track::Instrument * getSelectedInstrument();

signals:
    void setWaveform(TiaSound::Distortion dist);

public slots:

    void on_buttonInstrumentDelete_clicked();
    void on_buttonInstrumentExport_clicked();
    void on_buttonInstrumentImport_clicked();

    void on_spinBoxInstrumentEnvelopeLength_editingFinished();
    void on_spinBoxInstrumentEnvelopeLength_valueChanged(int newLength);

    void on_spinBoxSustainStart_editingFinished();
    void on_spinBoxSustainStart_valueChanged(int newStart);

    void on_spinBoxReleaseStart_editingFinished();
    void on_spinBoxReleaseStart_valueChanged(int newStart);

    void on_spinBoxInstrumentVolume_editingFinished();
    void on_spinBoxInstrumentVolume_valueChanged(int newVolume);

    void on_comboBoxWaveforms_currentIndexChanged(int index);

    void on_comboBoxInstruments_currentIndexChanged(int);
    void on_comboBoxInstruments_currentTextChanged(const QString &text);

protected:

private:
    Track::Track *pTrack = nullptr;

    static const QList<TiaSound::Distortion> availableWaveforms;

};

#endif // INSTRUMENTSTAB_H
