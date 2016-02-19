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
