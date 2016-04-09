/* TIATracker, (c) 2016 Andre "Kylearan" Wichmann.
 * Website: https://bitbucket.org/kylearan/tiatracker
 * Email: andre.wichmann@gmx.de
 * See the file "license.txt" for information on usage and redistribution
 * of this file.
 */

#ifndef GUIDEKEYBOARD_H
#define GUIDEKEYBOARD_H

#include <QObject>
#include <QWidget>

class GuideKeyboard : public QWidget
{
    Q_OBJECT
public:
    explicit GuideKeyboard(QWidget *parent = 0);

signals:

public slots:
};

#endif // GUIDEKEYBOARD_H
