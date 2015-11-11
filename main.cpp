#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QString>

#include "tiasound/tiasound.h"
#include "pianokeyboard.h"
#include "tiasound/pitchperfectpal.h"

#include <iostream>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Load and set stylesheet
    QFile styleFile(":/style.qss");
    styleFile.open(QFile::ReadOnly);
    a.setStyleSheet(styleFile.readAll());

    // GUI
    MainWindow w;

    PianoKeyboard *pk = w.findChild<PianoKeyboard *>("pianoKeyboard");
    pk->setInstrumentPitchGuide(TiaSound::perfectPalDist1);

    w.show();

    return a.exec();
}
