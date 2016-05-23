/* TIATracker, (c) 2016 Andre "Kylearan" Wichmann.
 * Website: https://bitbucket.org/kylearan/tiatracker
 * Email: andre.wichmann@gmx.de
 * See the file "license.txt" for information on usage and redistribution
 * of this file.
 */

#ifndef TIMELINE_H
#define TIMELINE_H

#include <QObject>
#include <QWidget>
#include "track/track.h"
#include <QMenu>


class Timeline : public QWidget
{
    Q_OBJECT
public:
    explicit Timeline(QWidget *parent = 0);

    void registerTrack(Track::Track *newTrack);
    void registerPatternMenu(QMenu *newPatternMenu);

    QSize sizeHint() const;

signals:
    void changeEditPos(int newPos);
    void changeEditPos(int newChannel, int newPos);
    void channelContextEvent(int channel, int row);

public slots:
    void editPosChanged(int newPos);
    void playerPosChanged(int pos1, int pos2);

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;
    bool event(QEvent *event) Q_DECL_OVERRIDE;

private:
    static const int channelWidth = 80;
    static const int channelMargin = 8;
    static const int channelGap = 8;
    static const int minHeight = 400;

    /* Calc row height in pixels */
    double calcRowHeight();

    int widgetWidth;
    int editPos = 0;
    int playerPos[2]{0};

    Track::Track *pTrack = nullptr;
    QMenu *pPatternMenu = nullptr;

};

#endif // TIMELINE_H
