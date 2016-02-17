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

private:
    Ui::SetSlideDialog *ui;
};

#endif // SETSLIDEDIALOG_H
