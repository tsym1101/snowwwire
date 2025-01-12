#ifndef SWW_FloatRangeSlider_H
#define SWW_FloatRangeSlider_H

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QWidget>
#else
#include <QtWidgets/QtWidgets>
#endif

class QMouseEvent;

namespace SwwKnob {

class FloatRangeSlider : public QWidget {
    Q_OBJECT
public:
    FloatRangeSlider(QWidget* parent = 0);
    void setOffsets(int left, int right);
    void setMinimalAllowedSliderDistance(float dist);
    float getMinValue();
    float getMaxValue();

public Q_SLOTS:
     void setMaxValue(float max);
     void setMinValue(float min);
     void setValues(float min, float max);
     void setSliderWidth(int sliderW);

Q_SIGNALS:
     void valuesChanged(float min, float max);
     void toggleInteractionMode(bool on);

protected:

    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

private:

    void moveSlider(float pos);

    float _minValue;           ///< relative x coordinate of position of left slider
    float _maxValue;           ///< relative x coordinate of position of right slider
    float _normalizedMousePos; ///< position where the drag of a slider started
    float _mV1;                ///< x coordinate of left slider when dragging started
    float _mV2;                ///< x coordinate of right slider when dragging started
    float _minimalAllowedSliderDistance; ///< mininmal allowed distance between both sliders
    int _sliderWidth;          ///< pixel width of sliders
    int _leftOffset;           ///< offset to left before slider and line will start
    int _rightOffset;          ///< offset to right before slider and line will end
    bool _leftSliderActive;    ///< true when left slider was clicked, false otherwise
    bool _rightSliderActive;   ///< true when right slider was clicked, false otherwise
};

} // namespace voreen

#endif // SWW_FloatRangeSlider_H
