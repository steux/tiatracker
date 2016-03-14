/* TIATracker, (c) 2016 Andre "Kylearan" Wichmann.
 * Website: https://bitbucket.org/kylearan/tiatracker
 * Email: andre.wichmann@gmx.de
 * See the file "license.txt" for information on usage and redistribution
 * of this file.
 */

#include "instrumentpitchguide.h"

namespace TiaSound {

/*************************************************************************/

int InstrumentPitchGuide::getNumFrequencies()
{
    return freqPitchGuides.length();
}

/*************************************************************************/

Note InstrumentPitchGuide::getNote(int frequency) {
    if (frequency >= freqPitchGuides.size()) {
        return Note::NotANote;
    } else {
        return freqPitchGuides[frequency].note;
    }
}

/*************************************************************************/

int InstrumentPitchGuide::getPercentOff(int frequency) {
    return freqPitchGuides[frequency].percentOff;
}

}
