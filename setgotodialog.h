#ifndef SETGOTODIALOG_H
#define SETGOTODIALOG_H

#include <QDialog>

namespace Ui {
class SetGotoDialog;
}

class SetGotoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetGotoDialog(QWidget *parent = 0);
    ~SetGotoDialog();

    void setMaxValue(int max);
    void setGotoValue(int value);
    int getGotoValue();

private:
    Ui::SetGotoDialog *ui;
};

#endif // SETGOTODIALOG_H
