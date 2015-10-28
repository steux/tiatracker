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

    QColor dark{"#002b36"};
    QColor darkHighlighted{"#073642"};
    QColor light{"#fdf6e3"};
    QColor lightHighlighted{"#eee8d5"};
    QColor contentDark{"#657b83"};
    QColor contenLight{"#839496"};
    QColor red{"#dc322f"};



private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
