#include "Knob/CustomSpinBox.h"
#include <limits>
#include <float.h>
#include <math.h>
#include "Core/SwwAssert.h"

#include <QMouseEvent>

CustomSpinBox::CustomSpinBox(QWidget *parent):QDoubleSpinBox(parent),_precision(1),_isDragging(false)
{
    double max = std::numeric_limits<double>::max();
    double min = std::numeric_limits<double>::min();
    setMaximum(max);
    setMinimum(-1E15); //minだと反映されなかった

    int maxDecimals =  DBL_DIG + DBL_MAX_10_EXP;
    setDecimals(maxDecimals);
    connect(this,SIGNAL(valueChanged(QString)),this,SLOT(changePrecision(QString)));
    setButtonSymbols(QAbstractSpinBox::NoButtons);
}

QString CustomSpinBox::textFromValue(double val) const
{
    return QLocale().toString(val,'f',_precision);
}

void CustomSpinBox::changePrecision(const QString &str)
{
    int index =  str.indexOf(".");
    double value = str.toDouble();
    if(value == 0)
    {
        _precision = 1;
        return;
    }
    if(index >= 0)
    {
        _precision = str.length() - (index + 1);
    }

}

void CustomSpinBox::calcPrecision(float value){
    // まず小数点以下の桁数を計算
    int precision = 0;
    float temp = value;

    // 小数部分が0になるまで10倍して桁数をカウント
    while (std::fabs(temp - std::round(temp)) > std::numeric_limits<float>::epsilon()) {
        temp *= 10;
        precision++;
    }
    _precision = precision;
}

void CustomSpinBox::mousePressEvent(QMouseEvent *event){

    _isDragging = true;

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    _oldPos = event->posF();
#else
    _oldPos = event->localPos();
#endif

    _oldValue = value();
    QDoubleSpinBox::mousePressEvent(event);
}

void CustomSpinBox::mouseMoveEvent(QMouseEvent *event){
    if(_isDragging){
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
        QPointF pos = event->posF();
#else
        QPointF pos = event->localPos();
#endif
        QPointF tmp = _oldPos + pos;

        int expornent =  (int)( (_oldPos - pos).y() / 100.0);

        if(expornent > 5)expornent = 5;
        if(expornent < -5)expornent = -5;

        double offset = (tmp - _oldPos).x() * std::pow(10,expornent) / 4.0;
        double newValue = _oldValue + offset;
        setValue(newValue);

        changePrecision(QString::number(newValue));
        Q_EMIT editingFinished();
    }
    QDoubleSpinBox::mouseMoveEvent(event);
}

void CustomSpinBox::mouseReleaseEvent(QMouseEvent *event){
    _isDragging = false;
    QDoubleSpinBox::mouseReleaseEvent(event);
}
