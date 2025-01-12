#include "FloatRangeSlider.h"

#include <math.h>

#include <QColor>
#include <QMouseEvent>
#include <QPainter>

namespace SwwKnob{

FloatRangeSlider::FloatRangeSlider(QWidget* parent)
    : QWidget(parent)
{
    _leftOffset = 0;
    _rightOffset = 0;
    _minValue = 0.f;
    _maxValue = 1.f;
    _leftSliderActive = false;
    _rightSliderActive = false;
    _minimalAllowedSliderDistance = 1.f/255.f;
    _sliderWidth = 4;
    setFixedHeight(16);
}

void FloatRangeSlider::setOffsets(int left, int right) {
    _leftOffset = left;
    _rightOffset = right;
}

void FloatRangeSlider::paintEvent(QPaintEvent* event) {
    event->accept();
    int leftMarker = static_cast<int>(round (_minValue * (width()-_leftOffset-_rightOffset) + _leftOffset));
    int rightMarker = static_cast<int>(round (_maxValue * (width()-_leftOffset-_rightOffset) + _leftOffset));
    QPoint leftSlider[5] = {
        QPoint(leftMarker - _sliderWidth, static_cast<int>(0.3f * height())),
        QPoint(leftMarker - _sliderWidth, height()),
        QPoint(leftMarker + _sliderWidth, height()),
        QPoint(leftMarker + _sliderWidth, static_cast<int>(0.3f * height())),
        QPoint(leftMarker, 0)
    };
    QPoint rightSlider[5] = {
        QPoint(rightMarker - _sliderWidth, static_cast<int>(0.3f * height())),
        QPoint(rightMarker - _sliderWidth, height()),
        QPoint(rightMarker + _sliderWidth, height()),
        QPoint(rightMarker + _sliderWidth, static_cast<int>(0.3f * height())),
        QPoint(rightMarker, 0)
    };
    QColor sliderColor(255, 255, 255);
    QColor sliderDarkColor(75, 130, 89);
    QColor lineColor(75, 130, 89);

    QPainter paint(this);
    paint.setRenderHint(QPainter::Antialiasing);

    //draw horizontal line
    paint.setPen(lineColor);
    paint.drawLine(_leftOffset, height()/2, width()-_rightOffset, height()/2);

    //draw left marker
    paint.setBrush(sliderColor);
    paint.setPen(sliderDarkColor);

    paint.save();
    paint.drawConvexPolygon(leftSlider, 5);
    paint.restore();

    //draw right marker
    paint.save();
    paint.drawConvexPolygon(rightSlider, 5);
    paint.restore();

}

void FloatRangeSlider::mousePressEvent(QMouseEvent* e) {
    e->accept();
    //calculate which marker is nearest to mouse position
    _normalizedMousePos = static_cast<float>((e->pos()).x()-_leftOffset) / static_cast<float>(width()-_leftOffset-_rightOffset);
    _mV1 = _minValue;
    _mV2 = _maxValue;
    if (e->button() == Qt::LeftButton) {
        if (fabs(_minValue - _normalizedMousePos) < fabs(_maxValue - _normalizedMousePos)) {
            _leftSliderActive = true;
            _rightSliderActive = false;
        }
        else {
            _leftSliderActive = false;
            _rightSliderActive = true;
        }
    }
    else if (e->button() == Qt::RightButton) {
        _leftSliderActive = true;
        _rightSliderActive = true;
    }
    moveSlider(_normalizedMousePos);
    Q_EMIT toggleInteractionMode(true);
}

void FloatRangeSlider::mouseMoveEvent(QMouseEvent* e){
    e->accept();
    float normalizedMousePosTmp = static_cast<float>((e->pos()).x()-_leftOffset) / static_cast<float>(width()-_leftOffset-_rightOffset);
    if (normalizedMousePosTmp > 1.f)
        normalizedMousePosTmp = 1.f;
    else if (normalizedMousePosTmp < 0.f)
        normalizedMousePosTmp = 0.f;
    moveSlider(normalizedMousePosTmp);
    Q_EMIT valuesChanged(_minValue, _maxValue);
}

void FloatRangeSlider::mouseReleaseEvent(QMouseEvent* e) {
    e->accept();
    Q_EMIT toggleInteractionMode(false);
}

void FloatRangeSlider::moveSlider(float mousePos) {
    if (_leftSliderActive && !_rightSliderActive)
        setMinValue(mousePos);
    if (_rightSliderActive && !_leftSliderActive)
        setMaxValue(mousePos);
    if (_rightSliderActive && _leftSliderActive) {
        float mouseDiff = _normalizedMousePos - mousePos;
        setMinValue(_mV1 - mouseDiff);
        setMaxValue(_mV2 - mouseDiff);
    }
}

void FloatRangeSlider::setMinValue(float val) {
    if (val == _minValue)
        return;

    if (val < 0.f)
        val = 0.f;
    if (val + _minimalAllowedSliderDistance < _maxValue)
        _minValue = val;
    else {
        _maxValue = val + _minimalAllowedSliderDistance;
        if (_maxValue > 1.f) {
            _maxValue = 1.f;
            _minValue = 1.f - _minimalAllowedSliderDistance;
        }
        else
            _minValue = val;
    }
    update();
    Q_EMIT valuesChanged(_minValue, _maxValue);
}

void FloatRangeSlider::setMaxValue(float val) {
    if (val == _maxValue)
        return;

    if (val > 1.f)
        val = 1.f;
    if (_minValue + _minimalAllowedSliderDistance < val)
        _maxValue = val;
    else {
        _minValue = val - _minimalAllowedSliderDistance;
        if (_minValue < 0.f) {
            _minValue = 0.f;
            _maxValue = _minimalAllowedSliderDistance;
        }
        else
            _maxValue = val;
    }
    update();
    Q_EMIT valuesChanged(_minValue, _maxValue);
}

void FloatRangeSlider::setSliderWidth(int sliderW) {
    _sliderWidth = sliderW;
}

void FloatRangeSlider::setValues(float val1, float val2) {
    if (val1 < val2) {
        setMinValue(val1);
        setMaxValue(val2);
    }
    else {
        setMinValue(val2);
        setMaxValue(val1);
    }
}

float FloatRangeSlider::getMinValue() {
    return _minValue;
}

float FloatRangeSlider::getMaxValue() {
    return _maxValue;
}

void FloatRangeSlider::setMinimalAllowedSliderDistance(float dist) {
    _minimalAllowedSliderDistance = dist;
}

} //ns
