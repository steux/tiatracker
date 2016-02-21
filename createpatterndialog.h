#ifndef CREATEPATTERNDIALOG_H
#define CREATEPATTERNDIALOG_H

#include <QDialog>
#include "track/track.h"


namespace Ui {
class CreatePatternDialog;
}

class CreatePatternDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreatePatternDialog(QWidget *parent = 0);
    ~CreatePatternDialog();

    void prepare(Track::Track *newTrack, int length);
    QString getName();
    int getLength();

private:
    Ui::CreatePatternDialog *ui;

    Track::Track *pTrack = nullptr;

};

#endif // CREATEPATTERNDIALOG_H
