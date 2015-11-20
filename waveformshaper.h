#ifndef WAVEFORMSHAPER_H
#define WAVEFORMSHAPER_H

#include <QWidget>
#include <QString>
#include <QList>

class WaveformShaper : public QWidget
{
    Q_OBJECT
public:
    QString name{};
    int scaleMin = 0;
    int scaleMax = 15;
    QList<int> values{};

    explicit WaveformShaper(QWidget *parent);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

};

#endif // WAVEFORMSHAPER_H
