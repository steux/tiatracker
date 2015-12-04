#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "track/track.h"
#include "tiasound/tiasound.h"
#include <QList>


namespace Ui {

class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /* Solarized-inspired colors */
    static const QColor dark;
    static const QColor darkHighlighted;
    static const QColor light;
    static const QColor lightHighlighted;
    static const QColor contentDark;
    static const QColor contentDarker;
    static const QColor contentLight;
    static const QColor contentLighter;
    static const QColor red;
    static const QColor orange;
    static const QColor blue;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    /* Initializes signal/slot connections */
    void initConnections();


private slots:

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
