/* TIATracker, (c) 2016 Andre "Kylearan" Wichmann.
 * Website: https://bitbucket.org/kylearan/tiatracker
 * Email: andre.wichmann@gmx.de
 * See the file "license.txt" for information on usage and redistribution
 * of this file.
 */

#ifndef SETFREQUENCYDIALOG_H
#define SETFREQUENCYDIALOG_H

#include <QDialog>

namespace Ui {
class SetFrequencyDialog;
}

class SetFrequencyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetFrequencyDialog(QWidget *parent = 0);
    ~SetFrequencyDialog();

    void setMaxFrequencyValue(int max);
    void setFrequencyValue(int value);
    int getFrequencyValue();


private:
    Ui::SetFrequencyDialog *ui;
};

#endif // SETFREQUENCYDIALOG_H
