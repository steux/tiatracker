#include "patterneditor.h"
#include <QFontMetrics>


PatternEditor::PatternEditor(QWidget *parent) : QWidget(parent)
{
    legendFont.setPixelSize(legendFontSize);
    QFontMetrics legendFontMetrics(legendFont);
    legendFontHeight = legendFontMetrics.height();
}

/*************************************************************************/

void PatternEditor::registerTrack(Track::Track *newTrack) {
    pTrack = newTrack;
}

