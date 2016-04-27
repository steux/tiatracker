#ifndef CREATEGUIDEDIALOG_H
#define CREATEGUIDEDIALOG_H

#include <QDialog>

namespace Ui {
class CreateGuideDialog;
}

class CreateGuideDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateGuideDialog(QWidget *parent = 0);
    ~CreateGuideDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::CreateGuideDialog *ui;
};

#endif // CREATEGUIDEDIALOG_H
