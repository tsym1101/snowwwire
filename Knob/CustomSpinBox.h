#ifndef CUSTOMSPINBOX_H
#define CUSTOMSPINBOX_H

#include "KnobAPI.h"
#include <QDoubleSpinBox>

class SWWKNOBSHARED_EXPORT CustomSpinBox:public QDoubleSpinBox
{
    Q_OBJECT
public:
    CustomSpinBox(QWidget *parent = 0);
    virtual QString textFromValue(double val) const;
    int getPrecision()const {return _precision;}
    void setPrecision(int i){_precision = i;}
    void calcPrecision(float value);

Q_SIGNALS:
    //void precisionChanged(int i);

public Q_SLOTS:
    void changePrecision(const QString& str);
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);


private:
    int _precision;
    bool _isDragging;
    QPointF _oldPos;
    double _oldValue;
};

#endif // CUSTOMSPINBOX_H
