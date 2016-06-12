/* TIATracker, (c) 2016 Andre "Kylearan" Wichmann.
 * Website: https://bitbucket.org/kylearan/tiatracker
 * Email: andre.wichmann@gmx.de
 * See the file "license.txt" for information on usage and redistribution
 * of this file.
 */

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
#include <QMessageBox>


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
    static const QColor green;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    /* Loads the key shortcuts for later use */
    static void loadKeymap();

    /* Initializes signal/slot connections */
    void initConnections();

    /* Initializes the player thread */
    void initPlayer();
    /* Stop the timer in the player thread */
    void stopPlayer();

    void registerTrack(Track::Track *newTrack);

    TiaSound::PitchGuide *getPitchGuide();

    /* Displays a message in an "OK" messagebox */
    static void displayMessage(const QString &message);

    void updateAllTabs();

    static QJsonObject keymap;

public slots:
    void setPitchGuide(TiaSound::PitchGuide newGuide);
    // Set a new pitch guide for the piano keyboard
    void setWaveform(TiaSound::Distortion dist);

signals:
    void initPlayerTimer();
    void stopPlayerTimer();
    void playInstrument(Track::Instrument *instrument, int frequency);
    void playInstrumentOnce(Track::Instrument *instrument, int frequency);
    void stopInstrument();
    void playPercussion(Track::Percussion *percussion);
    void stopPercussion();
    void setRowToInstrument(int frequency);
    void playTrack(int start1, int start2);
    void stopTrack();

private slots:

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

    void on_actionSaveAs_triggered();

    void on_actionOpen_triggered();

    void on_actionQuit_triggered();

    void on_actionPlayFromStart_triggered();

    void on_pushButtonPlay_clicked();

    void on_actionStop_triggered();

    void on_pushButtonStop_clicked();

    void on_actionNew_triggered();

    void on_actionExportDasm_triggered();

    void on_actionExport_complete_player_to_dasm_triggered();


    void on_actionAbout_triggered();

    void on_pushButtonPlayFromStart_clicked();

    void on_actionPlay_triggered();

    void on_actionPlay_pattern_triggered();

    void on_actionRead_the_manual_triggered();

    void on_spinBoxRowsPerBeat_valueChanged(int);

private:
    /* Tab index values */
    static const int iTabTrack = 0;
    static const int iTabInstruments = 1;
    static const int iTabPercussion = 2;
    static const int iTabInfo = 3;
    static const int iTabOptions = 4;

    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

    void saveTrackByName(const QString &fileName);
    void loadTrackByName(const QString &fileName);

    void setTrackName(QString name);

    void updateInfo();

    void addShortcut(QAction *action, QString actionName);
    void playTrackFrom(int channel, int row);

    QString readAsm(QString fileName);
    bool writeAsm(QString fileName, QString content, QString extension);
    QString listToBytes(QList<int> list);
    QString getExportFileName();
    bool exportFlags(QString fileName);
    bool exportTrackSpecifics(QString fileName);

    Ui::MainWindow *ui = nullptr;
    Track::Track *pTrack = nullptr;
    TiaSound::PitchGuideFactory pgFactory;
    TiaSound::PitchGuide curPitchGuide = pgFactory.getPitchPerfectPalGuide();

    QMenu waveformContextMenu{this};
    QAction actionInsertBefore{"Insert frame before", this};
    QAction actionInsertAfter{"Insert frame after", this};
    QAction actionDelete{"Delete frame", this};
    int waveformContextFrame;

    QAction actionToggleFollow{this};
    QAction actionToggleLoop{this};

    QString curSongsDialogPath;
};

#endif // MAINWINDOW_H
