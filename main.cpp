#include "mainwindow.h"
#include <QApplication>

#include "tiasound/tiasound.h"
#include "pianokeyboard.h"
#include "tiasound/pitchperfectpal.h"

#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    PianoKeyboard *pk = w.findChild<PianoKeyboard *>("pianoKeyboard");
    pk->setInstrumentPitchGuide(TiaSound::perfectPalDist1);

    w.show();

    return a.exec();
}
