/* TIATracker, (c) 2016 Andre "Kylearan" Wichmann.
 * Website: https://bitbucket.org/kylearan/tiatracker
 * Email: andre.wichmann@gmx.de
 * See the file "license.txt" for information on usage and redistribution
 * of this file.
 */

#ifndef RENAMEPATTERNDIALOG_H
#define RENAMEPATTERNDIALOG_H

#include <QDialog>

namespace Ui {
class RenamePatternDialog;
}

class RenamePatternDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RenamePatternDialog(QWidget *parent = 0);
    ~RenamePatternDialog();

    void setPatternName(QString &name);
    QString getPatternName();

private:
    Ui::RenamePatternDialog *ui;
};

#endif // RENAMEPATTERNDIALOG_H
