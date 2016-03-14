/* TIATracker, (c) 2016 Andre "Kylearan" Wichmann.
 * Website: https://bitbucket.org/kylearan/tiatracker
 * Email: andre.wichmann@gmx.de
 * See the file "license.txt" for information on usage and redistribution
 * of this file.
 */

#ifndef INSERTPATTERNDIALOG_H
#define INSERTPATTERNDIALOG_H

#include <QDialog>
#include "track/track.h"
#include "track/pattern.h"
#include "track/sequenceentry.h"


namespace Ui {
class InsertPatternDialog;
}

class InsertPatternDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InsertPatternDialog(QWidget *parent = 0);
    ~InsertPatternDialog();

    void prepare(Track::Track *newTrack);

    int getSelectedPattern();

private slots:
    void on_pushButtonCreateNewPattern_clicked();

private:
    Ui::InsertPatternDialog *ui;

    Track::Track *pTrack = nullptr;

};

#endif // INSERTPATTERNDIALOG_H
