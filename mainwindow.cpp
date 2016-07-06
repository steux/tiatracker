/* TIATracker, (c) 2016 Andre "Kylearan" Wichmann.
 * Website: https://bitbucket.org/kylearan/tiatracker
 * Email: andre.wichmann@gmx.de
 * See the file "license.txt" for information on usage and redistribution
 * of this file.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "track/instrument.h"
#include "track/track.h"
#include <QLineEdit>
#include <QSpinBox>
#include <cassert>
#include <QMessageBox>
#include "tiasound/pitchguidefactory.h"
#include "tiasound/pitchguide.h"
#include "tiasound/instrumentpitchguide.h"
#include <QMenu>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QAction>
#include <QKeySequence>
#include <QTextStream>
#include "track/sequence.h"
#include "track/sequenceentry.h"
#include "emulation/player.h"
#include "aboutdialog.h"
#include <QFileInfo>
#include <QDesktopServices>
#include <QCloseEvent>
#include <QSettings>


const QColor MainWindow::dark{"#002b36"};
const QColor MainWindow::darkHighlighted{"#073642"};
const QColor MainWindow::light{"#fdf6e3"};
const QColor MainWindow::lightHighlighted{"#eee8d5"};
const QColor MainWindow::contentDark{"#586e75"};
const QColor MainWindow::contentDarker{"#657b83"};
const QColor MainWindow::contentLight{"#839496"};
const QColor MainWindow::contentLighter{"#93a1a1"};
const QColor MainWindow::red{"#dc322f"};
const QColor MainWindow::orange{"#cb4b16"};
const QColor MainWindow::blue{"#268bd2"};
const QColor MainWindow::violet{"#6c71c4"};
const QColor MainWindow::green{"#859900"};

/*************************************************************************/

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Read in settings
    QSettings settings("Kylearan", "TIATracker");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("state").toByteArray(), 1);
    if (settings.contains("songsPath")) {
        curSongsDialogPath = settings.value("songsPath").toString();
    } else {
        curSongsDialogPath = QDir::currentPath() + "/songs";
    }
    if (settings.contains("instrumentsPath")) {
        ui->tabInstruments->curInstrumentsDialogPath = settings.value("instrumentsPath").toString();
    } else {
        ui->tabInstruments->curInstrumentsDialogPath = QDir::currentPath() + "/instruments";
    }
    if (settings.contains("percussionPath")) {
        ui->tabPercussion->curPercussionDialogPath = settings.value("percussionPath").toString();
    } else {
        ui->tabPercussion->curPercussionDialogPath = QDir::currentPath() + "/instruments";
    }
    if (settings.contains("guidesPath")) {
        ui->tabOptions->curGuidesDialogPath = settings.value("guidesPath").toString();
    } else {
        ui->tabOptions->curGuidesDialogPath = QDir::currentPath() + "/guides";
    }

    // Context menu for envelope widgets
    waveformContextMenu.addAction(&actionInsertBefore);
    waveformContextMenu.addAction(&actionInsertAfter);
    waveformContextMenu.addAction(&actionDelete);
    ui->volumeShaper->contextMenu = &waveformContextMenu;
    ui->frequencyShaper->contextMenu = &waveformContextMenu;
    ui->percussionVolumeShaper->contextMenu = &waveformContextMenu;
    ui->percussionFrequencyShaper->contextMenu = &waveformContextMenu;
}

/*************************************************************************/

MainWindow::~MainWindow() {
    delete ui;
}

/*************************************************************************/

QJsonObject MainWindow::keymap;

void MainWindow::loadKeymap() {
    QFile keymapFile("keymap.cfg");
    if (!keymapFile.open(QIODevice::ReadOnly)) {
        std::cout << "Unable to open keyboard shortcuts file keymap.cfg!\n";
    } else {
        QJsonDocument keymapDoc(QJsonDocument::fromJson(keymapFile.readAll()));
        keymap = keymapDoc.object();
        keymapFile.close();
    }
}

/*************************************************************************/

void MainWindow::initConnections() {
    // Global
    addShortcut(ui->actionNew, "TrackNew");
    addShortcut(ui->actionOpen, "TrackOpen");
    addShortcut(ui->actionSave, "TrackSave");
    addShortcut(ui->actionSaveAs, "TrackSaveAs");
    addShortcut(ui->actionQuit, "Quit");
    addShortcut(ui->actionPlayFromStart, "TrackPlayFromStart");
    addShortcut(ui->actionPlay, "TrackPlay");
    addShortcut(ui->actionStop, "TrackStop");
    addShortcut(ui->actionPlay_pattern, "TrackPlayPattern");

    // Shaper context menu
    QObject::connect(&actionInsertBefore, SIGNAL(triggered(bool)), this, SLOT(insertFrameBefore(bool)));
    QObject::connect(&actionInsertAfter, SIGNAL(triggered(bool)), this, SLOT(insertFrameAfter(bool)));
    QObject::connect(&actionDelete, SIGNAL(triggered(bool)), this, SLOT(deleteFrame(bool)));

    // InstrumentsTab
    QObject::connect(ui->buttonInstrumentDelete, &QPushButton::clicked, ui->tabInstruments, &InstrumentsTab::on_buttonInstrumentDelete_clicked);
    QObject::connect(ui->buttonInstrumentExport, &QPushButton::clicked, ui->tabInstruments, &InstrumentsTab::on_buttonInstrumentExport_clicked);
    QObject::connect(ui->buttonInstrumentImport, &QPushButton::clicked, ui->tabInstruments, &InstrumentsTab::on_buttonInstrumentImport_clicked);
    QObject::connect(ui->spinBoxInstrumentEnvelopeLength, &QSpinBox::editingFinished, ui->tabInstruments, &InstrumentsTab::on_spinBoxInstrumentEnvelopeLength_editingFinished);
    QObject::connect(ui->spinBoxInstrumentEnvelopeLength, SIGNAL(valueChanged(int)), ui->tabInstruments, SLOT(on_spinBoxInstrumentEnvelopeLength_valueChanged(int)));
    QObject::connect(ui->spinBoxSustainStart, &QSpinBox::editingFinished, ui->tabInstruments, &InstrumentsTab::on_spinBoxSustainStart_editingFinished);
    QObject::connect(ui->spinBoxSustainStart, SIGNAL(valueChanged(int)), ui->tabInstruments, SLOT(on_spinBoxSustainStart_valueChanged(int)));
    QObject::connect(ui->spinBoxReleaseStart, &QSpinBox::editingFinished, ui->tabInstruments, &InstrumentsTab::on_spinBoxReleaseStart_editingFinished);
    QObject::connect(ui->spinBoxReleaseStart, SIGNAL(valueChanged(int)), ui->tabInstruments, SLOT(on_spinBoxReleaseStart_valueChanged(int)));
    QObject::connect(ui->spinBoxInstrumentVolume, &QSpinBox::editingFinished, ui->tabInstruments, &InstrumentsTab::on_spinBoxInstrumentVolume_editingFinished);
    QObject::connect(ui->spinBoxInstrumentVolume, SIGNAL(valueChanged(int)), ui->tabInstruments, SLOT(on_spinBoxInstrumentVolume_valueChanged(int)));
    QObject::connect(ui->comboBoxWaveforms, SIGNAL(currentIndexChanged(int)), ui->tabInstruments, SLOT(on_comboBoxWaveforms_currentIndexChanged(int)));
    QObject::connect(ui->volumeShaper, &EnvelopeShaper::newMaxValue, ui->spinBoxInstrumentVolume, &QSpinBox::setValue);
    QObject::connect(ui->comboBoxInstruments, SIGNAL(currentIndexChanged(int)), ui->tabInstruments, SLOT(on_comboBoxInstruments_currentIndexChanged(int)));
    QObject::connect(ui->comboBoxInstruments, SIGNAL(currentTextChanged(QString)), ui->tabInstruments, SLOT(on_comboBoxInstruments_currentTextChanged(QString)));
    QObject::connect(ui->volumeShaper, SIGNAL(envelopeContextEvent(int)), this, SLOT(waveformContextEvent(int)));
    QObject::connect(ui->frequencyShaper, SIGNAL(envelopeContextEvent(int)), this, SLOT(waveformContextEvent(int)));

    // PercussionTab
    QObject::connect(ui->buttonPercussionDelete, &QPushButton::clicked, ui->tabPercussion, &PercussionTab::on_buttonPercussionDelete_clicked);
    QObject::connect(ui->buttonPercussionExport, &QPushButton::clicked, ui->tabPercussion, &PercussionTab::on_buttonPercussionExport_clicked);
    QObject::connect(ui->buttonPercussionImport, &QPushButton::clicked, ui->tabPercussion, &PercussionTab::on_buttonPercussionImport_clicked);
    QObject::connect(ui->spinBoxPercussionLength, &QSpinBox::editingFinished, ui->tabPercussion, &PercussionTab::on_spinBoxPercussionLength_editingFinished);
    QObject::connect(ui->spinBoxPercussionLength, SIGNAL(valueChanged(int)), ui->tabPercussion, SLOT(on_spinBoxPercussionLength_valueChanged(int)));
    QObject::connect(ui->checkBoxOverlay, SIGNAL(stateChanged(int)), ui->tabPercussion, SLOT(on_checkBoxOverlay_stateChanged(int)));
    QObject::connect(ui->spinBoxPercussionVolume, &QSpinBox::editingFinished, ui->tabPercussion, &PercussionTab::on_spinBoxPercussionVolume_editingFinished);
    QObject::connect(ui->spinBoxPercussionVolume, SIGNAL(valueChanged(int)), ui->tabPercussion, SLOT(on_spinBoxPercussionVolume_valueChanged(int)));
    QObject::connect(ui->percussionVolumeShaper, &PercussionShaper::newMaxValue, ui->spinBoxPercussionVolume, &QSpinBox::setValue);
    QObject::connect(ui->comboBoxPercussion, SIGNAL(currentIndexChanged(int)), ui->tabPercussion, SLOT(on_comboBoxPercussion_currentIndexChanged(int)));
    QObject::connect(ui->comboBoxPercussion, SIGNAL(currentTextChanged(QString)), ui->tabPercussion, SLOT(on_comboBoxPercussion_currentTextChanged(QString)));
    QObject::connect(ui->percussionVolumeShaper, SIGNAL(newPercussionValue(int)), ui->tabPercussion, SLOT(newPercussionValue(int)));
    QObject::connect(ui->percussionFrequencyShaper, SIGNAL(newPercussionValue(int)), ui->tabPercussion, SLOT(newPercussionValue(int)));
    QObject::connect(ui->percussionVolumeShaper, SIGNAL(envelopeContextEvent(int)), this, SLOT(waveformContextEvent(int)));
    QObject::connect(ui->percussionFrequencyShaper, SIGNAL(envelopeContextEvent(int)), this, SLOT(waveformContextEvent(int)));

    // TrackTab
    QObject::connect(ui->spinBoxRowsPerBeat, SIGNAL(valueChanged(int)), ui->trackEditor, SLOT(setRowsPerBeat(int)));
    QObject::connect(ui->spinBoxEvenTempo, SIGNAL(valueChanged(int)), ui->tabTrack, SLOT(setEvenSpeed(int)));
    QObject::connect(ui->spinBoxOddTempo, SIGNAL(valueChanged(int)), ui->tabTrack, SLOT(setOddSpeed(int)));
    QObject::connect(this, SIGNAL(setRowToInstrument(int)), ui->trackEditor, SLOT(setRowToInstrument(int)));
    addShortcut(&actionToggleFollow, "TrackToggleFollow");
    QObject::connect(&actionToggleFollow, SIGNAL(triggered(bool)), ui->tabTrack, SLOT(toggleFollow(bool)));
    QObject::connect(ui->tabTrack, SIGNAL(validateEditPos()), ui->trackEditor, SLOT(validateEditPos()));
    addShortcut(&actionToggleLoop, "ToggleLoop");
    QObject::connect(&actionToggleLoop, SIGNAL(triggered(bool)), ui->tabTrack, SLOT(toggleLoop(bool)));

    // OptionsTab
    QObject::connect(ui->radioButtonPal, SIGNAL(toggled(bool)), ui->tabOptions, SLOT(on_radioButtonPal_toggled(bool)));
    QObject::connect(ui->comboBoxPitchGuide, SIGNAL(currentIndexChanged(int)), ui->tabOptions, SLOT(on_comboBoxPitchGuide_currentIndexChanged(int)));
    QObject::connect(ui->tabOptions, SIGNAL(setPitchGuide(TiaSound::PitchGuide)), this, SLOT(setPitchGuide(TiaSound::PitchGuide)));
    QObject::connect(ui->spinBoxOffTuneThreshold, SIGNAL(valueChanged(int)), ui->pianoKeyboard, SLOT(setOffThreshold(int)));
    QObject::connect(ui->tabOptions, SIGNAL(setOffTuneThreshold(int)), ui->pianoKeyboard, SLOT(setOffThreshold(int)));
    QObject::connect(ui->pushButtonGuideCreate, SIGNAL(clicked(bool)), ui->tabOptions, SLOT(on_pushButtonGuideCreate_clicked(bool)));
    QObject::connect(ui->pushButtonGuideExport, SIGNAL(clicked(bool)), ui->tabOptions, SLOT(on_pushButtonGuideExport_clicked(bool)));
    QObject::connect(ui->pushButtonGuideImport, SIGNAL(clicked(bool)), ui->tabOptions, SLOT(on_pushButtonGuideImport_clicked(bool)));
    QObject::connect(ui->lineEditAuthor, SIGNAL(textChanged(QString)), ui->tabOptions, SLOT(on_lineEditAuthor_textChanged(QString)));
    QObject::connect(ui->lineEditSongName, SIGNAL(textChanged(QString)), ui->tabOptions, SLOT(on_lineEditSongName_textChanged(QString)));
    QObject::connect(ui->plainTextEditComment, SIGNAL(textChanged()), ui->tabOptions, SLOT(on_plainTextEditComment_textChanged()));

    // PianoKeyboard
    ui->pianoKeyboard->initPianoKeyboard();
    QObject::connect(ui->tabInstruments, SIGNAL(setWaveform(TiaSound::Distortion)), this, SLOT(setWaveform(TiaSound::Distortion)));
    QObject::connect(ui->pianoKeyboard, SIGNAL(newKeyPressed(int)), this, SLOT(newPianoKeyPressed(int)));
    QObject::connect(ui->pianoKeyboard, SIGNAL(keyReleased()), this, SLOT(pianoKeyReleased()));

    // Pattern editor
    QObject::connect(ui->trackEditor, SIGNAL(editPosChanged(int)), ui->trackTimeline, SLOT(editPosChanged(int)));
    QObject::connect(ui->trackTimeline, SIGNAL(changeEditPos(int)), ui->trackEditor, SLOT(setEditPos(int)));
    QObject::connect(ui->trackTimeline, SIGNAL(changeEditPos(int, int)), ui->trackEditor, SLOT(setEditPos(int, int)));
    QObject::connect(ui->trackInstrumentSelector, SIGNAL(setWaveform(TiaSound::Distortion)), this, SLOT(setWaveform(TiaSound::Distortion)));
    QObject::connect(ui->trackInstrumentSelector, SIGNAL(setUsePitchGuide(bool)), ui->pianoKeyboard, SLOT(setUsePitchGuide(bool)));
    QObject::connect(ui->trackEditor, SIGNAL(channelContextEvent(int,int)), ui->tabTrack, SLOT(channelContextEvent(int,int)));
    QObject::connect(ui->tabTrack, SIGNAL(advanceEditPos()), ui->trackEditor, SLOT(advanceEditPos()));
    QObject::connect(ui->checkBoxFollow, SIGNAL(toggled(bool)), ui->trackEditor, SLOT(toggleFollow_clicked(bool)));
    QObject::connect(ui->checkBoxLoop, SIGNAL(toggled(bool)), ui->trackEditor, SLOT(toggleLoop_clicked(bool)));

    // Timeline
    QObject::connect(ui->trackTimeline, SIGNAL(channelContextEvent(int,int)), ui->tabTrack, SLOT(channelContextEvent(int,int)));
}

/*************************************************************************/

void MainWindow::initPlayer() {
    emit initPlayerTimer();
}

/*************************************************************************/

void MainWindow::stopPlayer() {
    emit stopPlayerTimer();
}

/*************************************************************************/

void MainWindow::registerTrack(Track::Track *newTrack) {
    pTrack = newTrack;
    setTrackName(pTrack->name);
}

/*************************************************************************/

TiaSound::PitchGuide *MainWindow::getPitchGuide() {
    return &curPitchGuide;
}

/*************************************************************************/

void MainWindow::setPitchGuide(TiaSound::PitchGuide newGuide) {
    curPitchGuide = newGuide;
    updateAllTabs();
    update();
}

/*************************************************************************/

void MainWindow::displayMessage(const QString &message) {

    QMessageBox msgBox(QMessageBox::NoIcon,
                       "ERROR",
                       message,
                       QMessageBox::Ok, QApplication::activeWindow(),
                       Qt::FramelessWindowHint);
    msgBox.exec();
}

/*************************************************************************/

void MainWindow::setWaveform(TiaSound::Distortion dist) {
    TiaSound::InstrumentPitchGuide *pIPG = &(curPitchGuide.instrumentGuides[dist]);
    ui->pianoKeyboard->setInstrumentPitchGuide(pIPG);
    ui->pianoKeyboard->setUsePitchGuide(true);
    ui->pianoKeyboard->update();
}

/*************************************************************************/

void MainWindow::newPianoKeyPressed(int frequency) {
    switch (ui->tabWidget->currentIndex()) {
    case iTabTrack:
    {
        emit setRowToInstrument(frequency);
        int insIndex = ui->trackInstrumentSelector->getSelectedInstrument();
        if (insIndex < Track::Track::numInstruments) {
            Track::Instrument *instrument = &(pTrack->instruments[insIndex]);
            emit playInstrumentOnce(instrument, frequency);
        } else {
            Track::Percussion *percussion = &(pTrack->percussion[insIndex - Track::Track::numInstruments]);
            emit playPercussion(percussion);
        }
        break;
    }
    case iTabInstruments:
    {
        Track::Instrument *instrument = ui->tabInstruments->getSelectedInstrument();
        emit playInstrument(instrument, frequency);
        break;
    }
    case iTabPercussion:
    {
        Track::Percussion *percussion = ui->tabPercussion->getSelectedPercussion();
        emit playPercussion(percussion);
        break;
    }
    default:
        break;
    }
}

/*************************************************************************/

void MainWindow::pianoKeyReleased() {
    switch (ui->tabWidget->currentIndex()) {
    case iTabInstruments:
        emit stopInstrument();
        break;
    case iTabPercussion:
        break;
    default:
        break;
    }
}

/*************************************************************************/

void MainWindow::on_tabWidget_currentChanged(int index) {
    switch (index) {
    case iTabTrack:
        ui->pianoKeyboard->setUsePitchGuide(true);
        ui->tabTrack->updateTrackTab();
        break;
    case iTabInstruments:
        emit stopTrack();
        ui->pianoKeyboard->setUsePitchGuide(true);
        ui->tabInstruments->updateInstrumentsTab();
        break;
    case iTabPercussion:
        emit stopTrack();
        ui->pianoKeyboard->setUsePitchGuide(false);
        ui->tabPercussion->updatePercussionTab();
        break;
    case iTabInfo:
        updateInfo();
        break;
    case iTabOptions:
        emit stopTrack();
        ui->tabOptions->updateOptionsTab();
        break;
    }
}

/*************************************************************************/

void MainWindow::waveformContextEvent(int frame) {
    waveformContextFrame = frame;
}

/*************************************************************************/

void MainWindow::insertFrameBefore(bool) {
    switch (ui->tabWidget->currentIndex()) {
    case iTabInstruments: {
        Track::Instrument *ins = ui->tabInstruments->getSelectedInstrument();
        ins->insertFrameBefore(waveformContextFrame);
        ui->tabInstruments->updateInstrumentsTab();
        ui->tabInstruments->update();
        break;
    }
    case iTabPercussion: {
        Track::Percussion *perc = ui->tabPercussion->getSelectedPercussion();
        perc->insertFrameBefore(waveformContextFrame);
        ui->tabPercussion->updatePercussionTab();
        ui->tabPercussion->update();
        break;
    }
    default:
        break;
    }
}

/*************************************************************************/

void MainWindow::insertFrameAfter(bool) {
    switch (ui->tabWidget->currentIndex()) {
    case iTabInstruments: {
        Track::Instrument *ins = ui->tabInstruments->getSelectedInstrument();
        ins->insertFrameAfter(waveformContextFrame);
        ui->tabInstruments->updateInstrumentsTab();
        ui->tabInstruments->update();
        break;
    }
    case iTabPercussion: {
        Track::Percussion *perc = ui->tabPercussion->getSelectedPercussion();
        perc->insertFrameAfter(waveformContextFrame);
        ui->tabPercussion->updatePercussionTab();
        ui->tabPercussion->update();
        break;
    }
    default:
        break;
    }
}

/*************************************************************************/

void MainWindow::deleteFrame(bool) {
    switch (ui->tabWidget->currentIndex()) {
    case iTabInstruments: {
        Track::Instrument *ins = ui->tabInstruments->getSelectedInstrument();
        ins->deleteFrame(waveformContextFrame);
        ui->tabInstruments->updateInstrumentsTab();
        ui->tabInstruments->update();
        break;
    }
    case iTabPercussion: {
        Track::Percussion *perc = ui->tabPercussion->getSelectedPercussion();
        perc->deleteFrame(waveformContextFrame);
        ui->tabPercussion->updatePercussionTab();
        ui->tabPercussion->update();
        break;
    }
    default:
        break;
    }
}

/*************************************************************************/

void MainWindow::on_actionSave_triggered() {
    emit stopTrack();
    if (pTrack->name == "new track.ttt") {
        on_actionSaveAs_triggered();
    } else {
        saveTrackByName(pTrack->name);
    }
}

/*************************************************************************/

void MainWindow::saveTrackByName(const QString &fileName) {
    QFile saveFile(fileName);
    // Export track
    if (!saveFile.open(QIODevice::WriteOnly)) {
        displayMessage("Unable to open file!");
        return;
    }
    QJsonObject trackObject;
    pTrack->toJson(trackObject);
    QJsonDocument saveDoc(trackObject);
    saveFile.write(saveDoc.toJson());
    saveFile.close();
    setTrackName(fileName);
}

/*************************************************************************/

void MainWindow::loadTrackByName(const QString &fileName) {
    QFile loadFile(fileName);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        displayMessage("Unable to open file!");
        return;
    }
    QJsonDocument loadDoc(QJsonDocument::fromJson(loadFile.readAll()));

    pTrack->lock();
    // Parse in data
    if (!pTrack->fromJson(loadDoc.object())) {
        pTrack->unlock();
        return;
    }
    pTrack->unlock();
    setTrackName(fileName);
    ui->trackEditor->setEditPos(0);
    updateAllTabs();
}

/*************************************************************************/

void MainWindow::setTrackName(QString name) {
    pTrack->name = name;
    setWindowTitle("TIATracker v1.1 - " + pTrack->name);
}

/*************************************************************************/

void MainWindow::updateAllTabs() {
    TrackTab *trackTab = findChild<TrackTab *>("tabTrack");
    trackTab->updateTrackTab();
    trackTab->update();

    InstrumentsTab *insTab = findChild<InstrumentsTab *>("tabInstruments");
    insTab->updateInstrumentsTab();
    insTab->update();

    PercussionTab *percTab = findChild<PercussionTab *>("tabPercussion");
    percTab->updatePercussionTab();
    percTab->update();

    OptionsTab *optionsTab = findChild<OptionsTab *>("tabOptions");
    optionsTab->updateOptionsTab();
    optionsTab->update();

    InstrumentSelector *insSel = findChild<InstrumentSelector *>("trackInstrumentSelector");
    insSel->setSelectedInstrument(0);

    updateInfo();
}

/*************************************************************************/

void MainWindow::updateInfo() {
    ui->labelCoreRom->setText(QString::number(Emulation::Player::RomPlayerCore));
    bool usesGoto = pTrack->usesGoto();
    ui->labelGotoUsed->setText(usesGoto ? "X" : "-");
    ui->labelGotoRom->setText(usesGoto ? QString::number(Emulation::Player::RomGoto) : "<s>" + QString::number(Emulation::Player::RomGoto) + "</s>");
    bool usesSlide = pTrack->usesSlide();
    ui->labelSlideUsed->setText(usesSlide ? "X" : "-");
    ui->labelSlideRom->setText(usesSlide ? QString::number(Emulation::Player::RomSlide) : "<s>" + QString::number(Emulation::Player::RomSlide) + "</s>");
    bool usesOverlay = pTrack->usesOverlay();
    ui->labelOverlayUsed->setText(usesOverlay ? "X" : "-");
    ui->labelOverlayRom->setText(usesOverlay ? QString::number(Emulation::Player::RomOverlay) : "<s>" + QString::number(Emulation::Player::RomOverlay) + "</s>");
    bool usesFunk = pTrack->usesFunktempo();
    ui->labelFunktempoUsed->setText(usesFunk ? "X" : "-");
    ui->labelFunktempoRom->setText(usesFunk ? QString::number(Emulation::Player::RomFunktempo) : "<s>" + QString::number(Emulation::Player::RomFunktempo) + "</s>");
    bool startsWithHold = pTrack->startsWithHold();
    ui->labelStartsWithHold->setText(startsWithHold ? "X" : "-");
    ui->labelStartsWithHoldRom->setText(startsWithHold ? QString::number(Emulation::Player::RomStartsWithHold) : "<s>" + QString::number(Emulation::Player::RomStartsWithHold) + "</s>");

    int numInstruments = pTrack->getNumInstrumentsFromTrack();
    ui->labelInfoInstruments->setText(QString::number(numInstruments));
    int numPercussion = pTrack->getNumPercussionFromTrack();
    ui->labelInfoPercussion->setText(QString::number(numPercussion));
    int instrumentsSize = pTrack->calcInstrumentsSize();
    ui->labelInstrumentsRom->setText(QString::number(instrumentsSize));
    int percussionSize = pTrack->calcPercussionSize();
    ui->labelPercussionRom->setText(QString::number(percussionSize));
    int numPatterns = pTrack->numPatternsUsed();
    ui->labelInfoPatterns->setText(QString::number(numPatterns));
    int patternSize = pTrack->calcPatternSize();
    ui->labelPatternsRom->setText(QString::number(patternSize));
    int sequencesSize = pTrack->sequencesSize();
    ui->labelSequencesRom->setText(QString::number(sequencesSize));
    int totalRom =
            Emulation::Player::RomPlayerCore
            + (usesGoto ? Emulation::Player::RomGoto : 0)
            + (usesSlide ? Emulation::Player::RomSlide : 0)
            + (usesOverlay ? Emulation::Player::RomOverlay : 0)
            + (usesFunk ? Emulation::Player::RomFunktempo : 0)
            + (startsWithHold ? Emulation::Player::RomStartsWithHold : 0)
            + instrumentsSize
            + percussionSize
            + patternSize
            + sequencesSize;
    ui->labelInfoRomTotal->setText(QString::number(totalRom));

    ui->tabInfo->update();
}

/*************************************************************************/

void MainWindow::addShortcut(QAction *action, QString actionName) {
    if (keymap.contains(actionName) ) {
        QString shortcut = keymap[actionName].toString();
        action->setShortcut(QKeySequence(shortcut));
        addAction(action);
    }
}

/*************************************************************************/

void MainWindow::playTrackFrom(int channel, int row) {
    int otherChannel = 1 - channel;
    // Try to find parallel note in other channel
    int thisStartIndex = pTrack->startPatterns[channel];
    int thisStart = pTrack->channelSequences[channel].sequence[thisStartIndex].firstNoteNumber;
    int otherStartIndex = pTrack->startPatterns[otherChannel];
    int otherStart = pTrack->channelSequences[otherChannel].sequence[otherStartIndex].firstNoteNumber;
    QMap<int, bool> thisVisited;
    while (thisStart != -1 && otherStart != -1
           && thisStart != row && !thisVisited.contains(thisStart)) {
        thisVisited[thisStart] = true;
        thisStart = pTrack->getNextNoteWithGoto(channel, thisStart);
        otherStart = pTrack->getNextNoteWithGoto(otherChannel, otherStart);
    }
    if (thisStart != row) {
        displayMessage("Unable to reach this row from start pattern!");
        return;
    }
    // Play!
    if (channel == 0) {
        emit playTrack(row, otherStart);
    } else {
        emit playTrack(otherStart, row);
    }
}

/*************************************************************************/

void MainWindow::on_actionSaveAs_triggered() {
    emit stopTrack();
    QFileDialog dialog(this);
    dialog.setDirectory(curSongsDialogPath);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter("*.ttt");
    dialog.setDefaultSuffix("ttt");
    dialog.setViewMode(QFileDialog::Detail);
    dialog.selectFile(pTrack->name);
    QStringList fileNames;
    if (dialog.exec()) {
        fileNames = dialog.selectedFiles();
    }
    if (fileNames.isEmpty()) {
        return;
    }
    QString fileName = fileNames[0];
    curSongsDialogPath = dialog.directory().absolutePath();
    saveTrackByName(fileName);
}

/*************************************************************************/

void MainWindow::on_actionOpen_triggered() {
    emit stopTrack();
    // Ask if current track should really be discarded
    QMessageBox msgBox(QMessageBox::NoIcon,
                       "Open track",
                       "Do you really want to discard the current track?",
                       QMessageBox::Yes | QMessageBox::No, this,
                       Qt::FramelessWindowHint);
    if (msgBox.exec() != QMessageBox::Yes) {
        return;
    }

    // Ask for filename
    QFileDialog dialog(this);
    dialog.setDirectory(curSongsDialogPath);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter("*.ttt");
    dialog.setDefaultSuffix("ttt");
    dialog.setViewMode(QFileDialog::Detail);

    QStringList fileNames;
    if (dialog.exec()) {
        fileNames = dialog.selectedFiles();
    }
    if (fileNames.isEmpty()) {
        return;
    }
    QString fileName = fileNames[0];
    curSongsDialogPath = dialog.directory().absolutePath();
    loadTrackByName(fileName);
    // Create pitch guide if not there already
    if (pTrack->guideBaseFreq != 0.0) {
        bool isNewGuide = true;
        int guideIndex = -1;
        for (int i = 0; i < ui->tabOptions->guides.size(); ++i) {
            if (ui->tabOptions->guides[i].name == pTrack->guideName
                    && ui->tabOptions->guides[i].baseFreq == pTrack->guideBaseFreq
                    && ui->tabOptions->guides[i].tvStandard == pTrack->guideTvStandard) {
                isNewGuide = false;
                guideIndex = i;
                break;
            }
        }
        if (isNewGuide) {
            TiaSound::PitchGuideFactory pgFactory;
            TiaSound::PitchGuide newPG = pgFactory.calculateGuide(
                        pTrack->guideName,
                        pTrack->guideTvStandard,
                        pTrack->guideBaseFreq);
            ui->tabOptions->guides.append(newPG);
            ui->comboBoxPitchGuide->addItem(newPG.name);
            guideIndex = ui->tabOptions->guides.count() - 1;

        }
        ui->comboBoxPitchGuide->setCurrentIndex(guideIndex);
    }

    ui->trackEditor->setEditPos(0);
    update();
}

/*************************************************************************/

void MainWindow::on_actionQuit_triggered() {
    emit stopTrack();
    // Ask if current track should really be discarded
    QMessageBox msgBox(QMessageBox::NoIcon,
                       "Quit",
                       "Do you really want to quit?",
                       QMessageBox::Yes | QMessageBox::No, this,
                       Qt::FramelessWindowHint);
    if (msgBox.exec() != QMessageBox::Yes) {
        return;
    }

    // Save settings
    QSettings settings("Kylearan", "TIATracker");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState(1));
    settings.setValue("songsPath", curSongsDialogPath);
    settings.setValue("instrumentsPath", ui->tabInstruments->curInstrumentsDialogPath);
    settings.setValue("percussionPath", ui->tabPercussion->curPercussionDialogPath);
    settings.setValue("guidesPath", ui->tabOptions->curGuidesDialogPath);

    QApplication::quit();
}

/*************************************************************************/

void MainWindow::on_actionPlay_triggered(){
    emit stopTrack();
    PatternEditor *editor = findChild<PatternEditor *>("trackEditor");
    int thisChannel = editor->getSelectedChannel();
    int thisEditPos = editor->getEditPos();
    playTrackFrom(thisChannel, thisEditPos);
}

/*************************************************************************/

void MainWindow::on_actionPlayFromStart_triggered() {
    int startIndex1 = pTrack->startPatterns[0];
    int startNote1 = pTrack->channelSequences[0].sequence[startIndex1].firstNoteNumber;
    int startIndex2 = pTrack->startPatterns[1];
    int startNote2 = pTrack->channelSequences[1].sequence[startIndex2].firstNoteNumber;
    emit playTrack(startNote1, startNote2);
}

/*************************************************************************/

void MainWindow::on_actionPlay_pattern_triggered() {
    emit stopTrack();
    PatternEditor *editor = findChild<PatternEditor *>("trackEditor");
    int thisChannel = editor->getSelectedChannel();
    int thisEditPos = editor->getEditPos();
    int entryIndex = pTrack->getSequenceEntryIndex(thisChannel, thisEditPos);
    int startNote = pTrack->channelSequences[thisChannel].sequence[entryIndex].firstNoteNumber;
    playTrackFrom(thisChannel, startNote);
}

/*************************************************************************/

void MainWindow::on_pushButtonPlay_clicked() {
    on_actionPlay_triggered();
}

/*************************************************************************/

void MainWindow::on_pushButtonPlayFromStart_clicked() {
    on_actionPlayFromStart_triggered();
}

/*************************************************************************/

void MainWindow::on_actionStop_triggered() {
    emit stopTrack();
}

/*************************************************************************/

void MainWindow::on_pushButtonStop_clicked() {
    on_actionStop_triggered();
}

/*************************************************************************/

void MainWindow::on_actionNew_triggered() {
    emit stopTrack();
    // Ask if current track should really be discarded
    QMessageBox msgBox(QMessageBox::NoIcon,
                       "Quit",
                       "Do you really want to discard the current track?",
                       QMessageBox::Yes | QMessageBox::No, this,
                       Qt::FramelessWindowHint);
    if (msgBox.exec() != QMessageBox::Yes) {
        return;
    }
    pTrack->lock();
    pTrack->newTrack();
    setTrackName(pTrack->name);
    ui->trackEditor->setEditPos(0);
    updateAllTabs();
    pTrack->unlock();
    QComboBox *cbGuides = findChild<QComboBox *>("comboBoxPitchGuide");
    cbGuides->setCurrentIndex(0);
    update();
}

/*************************************************************************/

QString MainWindow::readAsm(QString fileName) {
    QFile fileIn(fileName);
    if (!fileIn.open(QIODevice::ReadOnly)) {
        MainWindow::displayMessage("Unable to open file " + fileName + "!");
        return "";
    }
    QTextStream inStream(&fileIn);
    QString inString = inStream.readAll();
    fileIn.close();
    return inString;
}

bool MainWindow::writeAsm(QString fileName, QString content, QString extension) {
    QFile outFile(fileName + extension);
    if (!outFile.open(QIODevice::WriteOnly)) {
        return false;
    }
    QTextStream outStream(&outFile);
    outStream << content;
    outFile.close();
    return true;
}

QString MainWindow::listToBytes(QList<int> list) {
    QString out;
    for (int i = 0; i < list.size(); ++i) {
        if (i%8 == 0) {
            if (i > 0) {
                out.append("\n");
            }
            out.append("        dc.b ");
        }
        out = (out + "$%1").arg(list[i], 2, 16, QChar('0'));
        if (i%8 != 7 && i != list.size() - 1) {
            out.append(", ");
        }
    }
    out.append("\n");
    return out;
}

QString MainWindow::getExportFileName() {
    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setViewMode(QFileDialog::Detail);
    // Remove extension, if there
    QString trackName = pTrack->name;
    if (trackName.endsWith(".ttt")) {
        trackName.truncate(trackName.length() - 4);
    }
    dialog.selectFile(trackName);
    QStringList fileNames;
    if (dialog.exec()) {
        fileNames = dialog.selectedFiles();
    }
    if (fileNames.isEmpty()) {
        return "";
    }
    QString fileName = fileNames[0];
    // Remove extension, if there
    if (fileName.endsWith(".ttt")) {
        fileName.truncate(fileName.length() - 4);
    }
    return fileName;
}

bool MainWindow::exportFlags(QString fileName) {
    // Export flags
    QString flagsString = readAsm("player/dasm/tt_variables.asm");
    if (flagsString == "") {
        return false;
    }
    flagsString.replace("%%AUTHOR%%", pTrack->metaAuthor);
    flagsString.replace("%%NAME%%", pTrack->metaName);
    flagsString.replace("%%EVENSPEED%%", QString::number(pTrack->evenSpeed));
    flagsString.replace("%%ODDSPEED%%", QString::number(pTrack->oddSpeed));
    bool usesGoto = pTrack->usesGoto();
    flagsString.replace("%%USEGOTO%%", (usesGoto ? "1" : "0"));
    bool usesSlide = pTrack->usesSlide();
    flagsString.replace("%%USESLIDE%%", (usesSlide ? "1" : "0"));
    bool usesOverlay = pTrack->usesOverlay();
    flagsString.replace("%%USEOVERLAY%%", (usesOverlay ? "1" : "0"));
    bool usesFunk = pTrack->usesFunktempo();
    flagsString.replace("%%USEFUNKTEMPO%%", (usesFunk ? "1" : "0"));
    bool startsWithHold = pTrack->startsWithHold();
    flagsString.replace("%%STARTSWITHNOTES%%", (startsWithHold ? "0" : "1"));
    // Write flags
    if (!writeAsm(fileName, flagsString, "_variables.asm")) {
        displayMessage("Unable to write variables file!");
        return false;
    }
    return true;
}

bool MainWindow::exportTrackSpecifics(QString fileName) {
    if (!exportFlags(fileName)) {
        return false;
    }

    // Export track data
    QString trackString = readAsm("player/dasm/tt_trackdata.asm");
    if (trackString == "") {
        return false;
    }
    trackString.replace("%%AUTHOR%%", pTrack->metaAuthor);
    trackString.replace("%%NAME%%", pTrack->metaName);
    // Mapping of encountered to real, to weed out the unused
    QMap<int, int> insMapping{};
    int numInstruments = 0;
    QList<int> insWaveforms;
    QList<int> insADStarts;
    QList<int> insSustainStarts;
    QList<int> insReleaseStarts;
    int insEnvelopeIndex = 0;
    QString insString;
    QMap<int, int> percMapping{};
    int numPercussion = 0;
    QList<int> percStarts;
    int percEnvelopeIndex = 0;
    QString percFreqString;
    QString percCtrlVolString;
    QMap<int, int> patternMapping{};
    QString patternString;
    QString patternPtrString;
    QVector<QList<int>> sequence(2);
    int numPatterns = 0;
    for (int channel = 0; channel < 2; ++channel) {
        for (int entry = 0; entry < pTrack->channelSequences[channel].sequence.size(); ++entry) {
            int patternIndex = pTrack->channelSequences[channel].sequence[entry].patternIndex;
            if (!patternMapping.contains(patternIndex)) {
                // Pattern not encountered yet
                patternMapping[patternIndex] = numPatterns;
                numPatterns++;
                // Write out pattern
                patternString.append("; " + pTrack->patterns[patternIndex].name + "\n");
                patternString.append("tt_pattern" + QString::number(patternMapping[patternIndex]) + ":\n");
                // Loop over all notes
                QList<int> patternValues;
                for (int n = 0; n < pTrack->patterns[patternIndex].notes.size(); ++n) {
                    Track::Note *note = &(pTrack->patterns[patternIndex].notes[n]);
                    switch (note->type) {
                    case Track::Note::instrumentType::Hold:
                    {
                        patternValues.append(int(Emulation::Player::NoteHold));
                        break;
                    }
                    case Track::Note::instrumentType::Instrument:
                    {
                        if (!insMapping.contains(note->instrumentNumber)) {
                            // Instrument not encountered yet
                            insMapping[note->instrumentNumber] = numInstruments;
                            Track::Instrument *ins = &(pTrack->instruments[note->instrumentNumber]);
                            // insSize includes end marker that is not in vol/freq lists, so do -1
                            int insSize = ins->calcEffectiveSize() - 1;
                            QList<int> insEnvelopeValues;
                            for (int i = 0; i < insSize; ++i) {
                                int freqValue = ins->frequencies[i] + 8;
                                int volValue = ins->volumes[i];
                                insEnvelopeValues.append((freqValue<<4)|volValue);
                            }
                            // Insert dummy byte between sustain and release
                            insEnvelopeValues.insert(ins->getReleaseStart(), 0);
                            // Insert end marker
                            insEnvelopeValues.append(0);
                            // Insert indexes. Two times if PURE_COMBINED
                            for (int i = 0; i < (ins->baseDistortion == TiaSound::Distortion::PURE_COMBINED ? 2 : 1); ++i) {
                                insADStarts.append(insEnvelopeIndex);
                                insSustainStarts.append(insEnvelopeIndex + ins->getSustainStart());
                                // +1 for dummy byte, -1 because player expects that
                                insReleaseStarts.append(insEnvelopeIndex + ins->getReleaseStart());
                            }
                            // Store waveform(s)
                            if (ins->baseDistortion == TiaSound::Distortion::PURE_COMBINED) {
                                insWaveforms.append(TiaSound::getDistortionInt(TiaSound::Distortion::PURE_HIGH));
                                insWaveforms.append(TiaSound::getDistortionInt(TiaSound::Distortion::PURE_LOW));
                            } else {
                                insWaveforms.append(TiaSound::getDistortionInt(ins->baseDistortion));
                            }
                            // Write out instrument data
                            insString.append("; " + QString::number(numInstruments));
                            if (ins->baseDistortion == TiaSound::Distortion::PURE_COMBINED) {
                                insString.append("+" + QString::number(numInstruments + 1));
                            }
                            insString.append(": " + ins->name + "\n");
                            insString.append(listToBytes(insEnvelopeValues));
                            // Increase running instrument index
                            numInstruments += (ins->baseDistortion == TiaSound::Distortion::PURE_COMBINED ? 2 : 1);
                            // +1 for dummy byte, +1 for end marker
                            insEnvelopeIndex += insSize + 2;
                        }
                        // +1 because first instrument number is 1
                        int valueIns = insMapping[note->instrumentNumber] + 1;
                        if (pTrack->instruments[note->instrumentNumber].baseDistortion == TiaSound::Distortion::PURE_COMBINED
                                && note->value > 31) {
                            valueIns++;
                        }
                        int valueFreq = (note->value)%32;
                        patternValues.append((valueIns<<5)|valueFreq);
                        break;
                    }
                    case Track::Note::instrumentType::Pause:
                    {
                        patternValues.append(int(Emulation::Player::NotePause));
                        break;
                    }
                    case Track::Note::instrumentType::Percussion:
                    {
                        if (!percMapping.contains(note->instrumentNumber)) {
                            // Percussion not encountered yet
                            percMapping[note->instrumentNumber] = numPercussion;
                            Track::Percussion *perc = &(pTrack->percussion[note->instrumentNumber]);
                            // percSize includes end marker that is not in lists, so do -1
                            int percSize = perc->calcEffectiveSize() - 1;
                            QList<int> percFreqValues;
                            QList<int> percCtrlVolValues;
                            for (int i = 0; i < percSize; ++i) {
                                int freqValue = perc->frequencies[i];
                                if (perc->overlay && i == percSize - 1) {
                                    freqValue += 128;
                                }
                                percFreqValues.append(freqValue);
                                int ctrlValue = TiaSound::getDistortionInt(perc->waveforms[i]);
                                int volValue = perc->volumes[i];
                                percCtrlVolValues.append((ctrlValue<<4)|volValue);
                            }
                            // Insert end marker
                            percFreqValues.append(0);
                            percCtrlVolValues.append(0);
                            // Insert index. +1 because player expects that
                            percStarts.append(percEnvelopeIndex + 1);
                            // Write out percussion data
                            percFreqString.append("; " + QString::number(numPercussion) + ": " + perc->name + "\n");
                            percFreqString.append(listToBytes(percFreqValues));
                            percCtrlVolString.append("; " + QString::number(numPercussion) + ": " + perc->name + "\n");
                            percCtrlVolString.append(listToBytes(percCtrlVolValues));
                            // Increase running percussion index
                            numPercussion++;
                            // +1 for end marker
                            percEnvelopeIndex += percSize + 1;
                        }
                        patternValues.append(percMapping[note->instrumentNumber] + Emulation::Player::NoteFirstPerc);
                        break;
                    }
                    case Track::Note::instrumentType::Slide:
                    {
                        patternValues.append(Emulation::Player::NoteHold + note->value);
                        break;
                    }
                    }
                }
                // Pattern end marker
                patternValues.append(0);
                patternString.append(listToBytes(patternValues));
                patternString.append("\n");
                // Pattern ptr
                if ((patternMapping.size())%4 == 1) {
                    patternPtrString.append("        dc.b ");
                } else {
                    patternPtrString.append(", ");
                }
                patternPtrString.append("<tt_pattern" + QString::number(patternMapping[patternIndex]));
                if ((patternMapping.size())%4 == 0) {
                    patternPtrString.append("\n");
                }
            }
            sequence[channel].append(patternMapping[patternIndex]);
            int gotoTarget = pTrack->channelSequences[channel].sequence[entry].gotoTarget;
            if (gotoTarget != -1) {
                int value = 128 + gotoTarget;
                if (channel == 1) {
                    value += sequence[0].size();
                }
                sequence[channel].append(value);
            }
        }
    }
    trackString.replace("%%INSFREQVOLTABLE%%", insString);
    trackString.replace("%%INSCTRLTABLE%%", listToBytes(insWaveforms));
    trackString.replace("%%INSADINDEXES%%", listToBytes(insADStarts));
    trackString.replace("%%INSSUSTAININDEXES%%", listToBytes(insSustainStarts));
    trackString.replace("%%INSRELEASEINDEXES%%", listToBytes(insReleaseStarts));
    trackString.replace("%%PERCINDEXES%%", listToBytes(percStarts));
    trackString.replace("%%PERCFREQTABLE%%", percFreqString);
    trackString.replace("%%PERCCTRLVOLTABLE%%", percCtrlVolString);
    trackString.replace("%%SEQUENCECHANNEL0%%", listToBytes(sequence[0]));
    trackString.replace("%%SEQUENCECHANNEL1%%", listToBytes(sequence[1]));
    trackString.replace("%%PATTERNDEFS%%", patternString);
    trackString.replace("%%PATTERNPTRLO%%", patternPtrString);
    patternPtrString.replace("<", ">");
    trackString.replace("%%PATTERNPTRHI%%", patternPtrString);

    // Write track data
    if (!writeAsm(fileName, trackString, "_trackdata.asm")) {
        displayMessage("Unable to write trackdata file!");
        return false;
    }

    // Export Init
    QString initString = readAsm("player/dasm/tt_init.asm");
    if (initString == "") {
        return false;
    }
    initString.replace("%%AUTHOR%%", pTrack->metaAuthor);
    initString.replace("%%NAME%%", pTrack->metaName);
    initString.replace("%%C0INIT%%", QString::number(pTrack->startPatterns[0]));
    initString.replace("%%C1INIT%%", QString::number(pTrack->startPatterns[1] + sequence[0].size()));
    // Write init
    if (!writeAsm(fileName, initString, "_init.asm")) {
        displayMessage("Unable to write init file!");
        return false;
    }

    return true;
}

void MainWindow::on_actionExportDasm_triggered() {
    emit stopTrack();
    QString fileName = getExportFileName();
    if (!exportTrackSpecifics(fileName)) {
        return;
    }
}

/*************************************************************************/

void MainWindow::on_actionExport_complete_player_to_dasm_triggered() {
    emit stopTrack();
    QString fileName = getExportFileName();
    if (!exportTrackSpecifics(fileName)) {
        return;
    }
    // Player
    QString playerString = readAsm("player/dasm/tt_player.asm");
    if (playerString == "") {
        return;
    }
    playerString.replace("%%AUTHOR%%", pTrack->metaAuthor);
    playerString.replace("%%NAME%%", pTrack->metaName);
    if (!writeAsm(fileName, playerString, "_player.asm")) {
        displayMessage("Unable to write player file!");
        return;
    }
    // Framework
    QString frameworkString = readAsm("player/dasm/tt_player_framework.asm");
    if (frameworkString == "") {
        return;
    }
    frameworkString.replace("%%AUTHOR%%", pTrack->metaAuthor);
    frameworkString.replace("%%NAME%%", pTrack->metaName);
    if (pTrack->getTvMode() == TiaSound::TvStandard::PAL) {
        frameworkString.replace("%%PAL%%", "1");
        frameworkString.replace("%%NTSC%%", "0");
    } else {
        frameworkString.replace("%%PAL%%", "0");
        frameworkString.replace("%%NTSC%%", "1");
    }
    // Remove path info from filename
    QString baseName(QFileInfo(fileName).fileName());
    frameworkString.replace("%%FILENAME%%", baseName);
    if (!writeAsm(fileName, frameworkString, "_player_framework.asm")) {
        displayMessage("Unable to write framework file!");
        return;
    }
}

/*************************************************************************/

void MainWindow::on_actionAbout_triggered() {
    AboutDialog about;
    about.setWindowFlags(Qt::FramelessWindowHint);
    about.exec();
}

/*************************************************************************/

void MainWindow::on_actionRead_the_manual_triggered() {
    QDesktopServices::openUrl(QUrl("TIATracker_manual.pdf", QUrl::TolerantMode));
}

/*************************************************************************/

void MainWindow::on_spinBoxRowsPerBeat_valueChanged(int) {
    ui->tabTrack->updateTrackStats();
}

/*************************************************************************/

void MainWindow::closeEvent(QCloseEvent *event) {
    on_actionQuit_triggered();
    event->ignore();
}

/*************************************************************************/

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    if (!event->isAutoRepeat()) {
        pianoKeyReleased();
    }
}

/*************************************************************************/

void MainWindow::on_actionExport_track_data_to_k65_triggered() {

}

/*************************************************************************/

void MainWindow::on_actionExport_complete_player_to_k65_triggered() {

}

/*************************************************************************/

void MainWindow::on_actionExport_track_data_to_csv_triggered() {
    emit stopTrack();

    // Ask for filename
    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter("*.csv");
    dialog.setDefaultSuffix("csv");
    dialog.setViewMode(QFileDialog::Detail);
    QString trackName = pTrack->name;
    if (trackName.endsWith(".ttt")) {
        trackName.truncate(trackName.length() - 4);
    }
    if (!trackName.endsWith(".csv")) {
        trackName.append(".csv");
    }
    dialog.selectFile(trackName);
    QStringList fileNames;
    if (dialog.exec()) {
        fileNames = dialog.selectedFiles();
    }
    if (fileNames.isEmpty()) {
        return;
    }
    QString fileName = fileNames[0];

    // Export to csv
    QFile outFile(fileName);
    if (!outFile.open(QIODevice::WriteOnly)) {
        displayMessage("Unable to open file!");
        return;
    }
    QTextStream outStream(&outFile);

    // Write format description
    outStream << "tick, C0 sequence, C0 pattern, C0 pattern name, C0 row, C0 note, C1 sequence, C1 pattern, C1 pattern name, C1 row, C1 note\n";

    // Go through notes and write
    int startIndex1 = pTrack->startPatterns[0];
    int curNote1 = pTrack->channelSequences[0].sequence[startIndex1].firstNoteNumber;
    int startIndex2 = pTrack->startPatterns[1];
    int curNote2 = pTrack->channelSequences[1].sequence[startIndex2].firstNoteNumber;

    QMap<int, bool> visited;
    int numRow = 0;
    while (curNote1 != -1 && curNote2 != -1
           && !visited.contains(curNote1)) {
        // Construct and write line
        QString line = QString::number(numRow);

        int seqEntry1 = pTrack->getSequenceEntryIndex(0, curNote1);
        int pattern1 = pTrack->getPatternIndex(0, curNote1);
        int row1 = pTrack->getNoteIndexInPattern(0, curNote1);
        QString patName1 = pTrack->patterns[pattern1].name;
        patName1.replace(',', ' ');
        line.append(", " + QString::number(seqEntry1));
        line.append(", " + QString::number(pattern1));
        line.append(", " + patName1);
        line.append(", " + QString::number(row1));
        line.append(", " + ui->trackEditor->constructRowString(row1, &(pTrack->patterns[pattern1])));

        int seqEntry2 = pTrack->getSequenceEntryIndex(1, curNote2);
        int pattern2 = pTrack->getPatternIndex(1, curNote2);
        int row2 = pTrack->getNoteIndexInPattern(1, curNote2);
        QString patName2 = pTrack->patterns[pattern2].name;
        patName2.replace(',', ' ');
        line.append(", " + QString::number(seqEntry2));
        line.append(", " + QString::number(pattern2));
        line.append(", " + patName2);
        line.append(", " + QString::number(row2));
        line.append(", " + ui->trackEditor->constructRowString(row2, &(pTrack->patterns[pattern2])));

        outStream << line << "\n";

        // Mark visited and go to next row
        visited[curNote1] = true;
        curNote1 = pTrack->getNextNoteWithGoto(0, curNote1);
        curNote2 = pTrack->getNextNoteWithGoto(1, curNote2);
        numRow++;
    }

    outFile.close();

}
