/* TIATracker, (c) 2016 Andre "Kylearan" Wichmann.
 * Website: https://bitbucket.org/kylearan/tiatracker
 * Email: andre.wichmann@gmx.de
 * See the file "license.txt" for information on usage and redistribution
 * of this file.
 */

#ifndef SETSLIDEDIALOG_H
#define SETSLIDEDIALOG_H

#include <QDialog>

namespace Ui {
class SetSlideDialog;
}

class SetSlideDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetSlideDialog(QWidget *parent = 0);
    ~SetSlideDialog();

    int getSlideValue();
    void setSlideValue(int value);


private:
    Ui::SetSlideDialog *ui;
};

#endif // SETSLIDEDIALOG_H
