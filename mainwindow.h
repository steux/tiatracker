#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static const QColor dark;
    static const QColor darkHighlighted;
    static const QColor light;
    static const QColor lightHighlighted;
    static const QColor contentDark;
    static const QColor contentDarker;
    static const QColor contentLight;
    static const QColor contentLighter;
    static const QColor red;



private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
