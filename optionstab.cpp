#include "optionstab.h"
#include <QComboBox>
#include <QSpinBox>
#include "createguidedialog.h"
#include <QLayout>
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonDocument>
#include "mainwindow.h"
#include <QLabel>


OptionsTab::OptionsTab(QWidget *parent) : QWidget(parent)
{
    TiaSound::PitchGuideFactory pgFactory;
    guides.append(pgFactory.getPitchPerfectPalGuide());
    guides.append(pgFactory.getOptimizedPurePalGuide());
    guides.append(pgFactory.getOptimizedDiv31PalGuide());
    guides.append(pgFactory.getOptimizedDiv6PalGuide());
    guides.append(pgFactory.getOptimizedDiv2PalGuide());
    guides.append(pgFactory.getPitchPerfectNtscGuide());
    guides.append(pgFactory.getOptimizedPureNtscGuide());
    guides.append(pgFactory.getOptimizedDiv31NtscGuide());
    guides.append(pgFactory.getOptimizedDiv6NtscGuide());
    guides.append(pgFactory.getOptimizedDiv2NtscGuide());
}

/*************************************************************************/

void OptionsTab::registerTrack(Track::Track *newTrack) {
    pTrack = newTrack;
}

/*************************************************************************/

void OptionsTab::initOptionsTab() {
    QComboBox *cbGuides = findChild<QComboBox *>("comboBoxPitchGuide");
    for (int i = 0; i < guides.size(); ++i) {
        cbGuides->addItem(guides[i].name);
    }
}

/*************************************************************************/

void OptionsTab::updateOptionsTab() {
    QLabel *infoLabel = findChild<QLabel *>("labelGuideInfo");
    QComboBox *cbGuides = findChild<QComboBox *>("comboBoxPitchGuide");
    TiaSound::PitchGuide *pg = &(guides[cbGuides->currentIndex()]);
    QString tvText = (pg->tvStandard == TiaSound::TvStandard::PAL ? "PAL" : "NTSC");
    infoLabel->setText("(" + tvText + ", " + QString::number(pg->baseFreq) + "Hz)");
    update();
}

/*************************************************************************/

void OptionsTab::on_comboBoxPitchGuide_currentIndexChanged(int index) {
    // Update guide data for track
    QComboBox *cbGuides = findChild<QComboBox *>("comboBoxPitchGuide");
    TiaSound::PitchGuide *pg = &(guides[cbGuides->currentIndex()]);
    pTrack->guideName = pg->name;
    pTrack->guideBaseFreq = pg->baseFreq;
    updateOptionsTab();
    emit setPitchGuide(guides[index]);
}

/*************************************************************************/

void OptionsTab::on_radioButtonPal_toggled(bool checked) {
    if (checked) {
        // Set to PAL
        pTrack->setTvMode(TiaSound::TvStandard::PAL);
        emit setTVStandard(TiaSound::TvStandard::PAL);
    } else {
        // Set to NTSC
        pTrack->setTvMode(TiaSound::TvStandard::NTSC);
        emit setTVStandard(TiaSound::TvStandard::NTSC);
    }
}

/*************************************************************************/

void OptionsTab::on_spinBoxOffTuneThreshold_editingFinished() {
    QSpinBox *sbThreshold = findChild<QSpinBox *>("spinBoxOffTuneThreshold");
    emit setOffTuneThreshold(sbThreshold->value());
}

/*************************************************************************/

void OptionsTab::addGuide(TiaSound::PitchGuide newGuide) {
    guides.append(newGuide);
    QComboBox *cbGuides = findChild<QComboBox *>("comboBoxPitchGuide");
    cbGuides->addItem(newGuide.name);
    cbGuides->setCurrentIndex(cbGuides->count() - 1);
    updateOptionsTab();
}

/*************************************************************************/

void OptionsTab::on_pushButtonGuideCreate_clicked(bool) {
    CreateGuideDialog newDialog(this);
    newDialog.layout()->setSizeConstraint(QLayout::SetFixedSize);
    if (newDialog.exec() == QDialog::Accepted) {
        if (newDialog.isGuideCreated) {
            addGuide(newDialog.newGuide);
        }
    }
}

/*************************************************************************/

void OptionsTab::on_pushButtonGuideImport_clicked(bool) {
    // Ask for filename
    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter("*.ttg");
    dialog.setDefaultSuffix("ttg");
    dialog.setViewMode(QFileDialog::Detail);

    QStringList fileNames;
    if (dialog.exec()) {
        fileNames = dialog.selectedFiles();
    }
    if (fileNames.isEmpty()) {
        return;
    }
    QString fileName = fileNames[0];
    QFile loadFile(fileName);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        MainWindow::displayMessage("Unable to open file!");
        return;
    }
    QJsonDocument loadDoc(QJsonDocument::fromJson(loadFile.readAll()));

    // Parse in data
    QJsonObject json = loadDoc.object();
    QString name = json["name"].toString();
    TiaSound::TvStandard standard = (json["TvStandard"] == "PAL" ? TiaSound::TvStandard::PAL : TiaSound::TvStandard::NTSC);
    double baseFreq = json["baseFrequency"].toDouble();
    TiaSound::PitchGuideFactory pgFactory;
    TiaSound::PitchGuide pg = pgFactory.calculateGuide(name, standard, baseFreq);

    addGuide(pg);
}

/*************************************************************************/

void OptionsTab::on_pushButtonGuideExport_clicked(bool) {
    QComboBox *cbGuides = findChild<QComboBox *>("comboBoxPitchGuide");
    TiaSound::PitchGuide *pg = &(guides[cbGuides->currentIndex()]);

    // Ask for filename
    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter("*.ttg");
    dialog.setDefaultSuffix("ttg");
    dialog.setViewMode(QFileDialog::Detail);
    dialog.selectFile(pg->name);
    QStringList fileNames;
    if (dialog.exec()) {
        fileNames = dialog.selectedFiles();
    }
    if (fileNames.isEmpty()) {
        return;
    }
    QString fileName = fileNames[0];
    QFile saveFile(fileName);

    // Export guide
    if (!saveFile.open(QIODevice::WriteOnly)) {
        MainWindow::displayMessage("Unable to open file!");
        return;
    }
    QJsonObject insObject;
    pg->toJson(insObject);
    QJsonDocument saveDoc(insObject);
    saveFile.write(saveDoc.toJson());
    saveFile.close();
}
