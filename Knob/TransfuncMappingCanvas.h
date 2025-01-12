#ifndef SWWGL_TRANSFUNCMAPPINGCANVAS_H
#define SWWGL_TRANSFUNCMAPPINGCANVAS_H

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QWidget>
#else
#include <QtWidgets/QtWidgets>
#endif
#include <QMenu>
#include <QThread>
#include <ImathVec.h>
#include "Core/SwwTypeDef.h"

class QAction;
class QColor;
class QMouseEvent;

namespace SwwGeom{

class TransFuncMappingKey;
class TransFuncIntensity;
class TransferFunctionPlug;

class TransFuncMappingCanvas : public QWidget{
Q_OBJECT
public:
    TransFuncMappingCanvas(QWidget* parent, TransferFunctionPlug* tfPlug, bool noColor = false,
                           bool clipThresholds = false, QString xAxisText = tr("intensity"),
                           QString yAxisText = tr("opacity"));

    virtual ~TransFuncMappingCanvas();
    virtual void paintEvent(QPaintEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mouseDoubleClickEvent(QMouseEvent* event);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);
    void setThreshold(float l, float u);
    virtual QSize minimumSizeHint() const;
    virtual QSize sizeHint() const;
    virtual QSizePolicy sizePolicy() const;
    void setTransFunc(TransFuncIntensity* tf);
    void setXAxisText(const std::string& text);
    void setYAxisText(const std::string& text);

    //const TransFuncIntensity* getTF()const{return tf_;}

Q_SIGNALS:
    void colorChanged(const QColor& c);
    void changed();
    void loadTransferFunction();
    void saveTransferFunction();
    void resetTransferFunction();
    void toggleInteractionMode(bool on);

public Q_SLOTS:
    void splitMergeKeys();
    void zeroKey();
    void deleteKey();
    void resetTransferFunc();
    void changeCurrentColor();
    void changeCurrentColor(const QColor& c);
    void toggleClipThresholds(bool enabled);

protected:
    // enum for the status of a key
    enum MarkerProps {
        MARKER_NORMAL   =  0, ///< key is not selected and not split
        MARKER_LEFT     =  1, ///< left part of a key
        MARKER_RIGHT    =  2, ///< right part of a key
        MARKER_SELECTED =  4  ///< key is selected
    };
    void insertNewKey(Imath::V2f& hit);
    TransFuncMappingKey* getOtherKey(TransFuncMappingKey* selectedKey, bool selectedLeftPart);
    int hitLine(const Imath::V2f& p);
    void paintKeys(QPainter& paint);
    void drawMarker(QPainter& paint, const Sww::Core::V4uc& color, const Imath::V2f& p,
                    int props = 0);

    void showKeyContextMenu(QMouseEvent* event);
    void showNoKeyContextMenu(QMouseEvent* event);
    virtual void resizeEvent(QResizeEvent* event);
    virtual void showEvent(QShowEvent* event);
    Imath::V2f wtos(Imath::V2f p);
    Imath::V2f stow(Imath::V2f p);
    QColor Col2QColor(const Sww::Core::V4uc& color);
    Sww::Core::V4uc QColor2Col(const QColor& color);
    void hideCoordinates();
    void updateCoordinates(QPoint pos, Imath::V2f values);
    //void updateHistogram();

    //TransFuncIntensity* tf_;             ///< pointer to the transfer function that is displayed
    //HistogramPainter* histogramPainter_; ///< painter that draws the histogram onto this widget

    TransferFunctionPlug* _tfPlug;

    int maximumIntensity_;               ///< maximum intensity of the associated volume

    float thresholdL_; ///< lower threshold in the interval [0, 1] (relative to maximumIntensity_)
    float thresholdU_; ///< upper threshold in the interval [0, 1] (relative to maximumIntensity_)

    // variables for interaction
    TransFuncMappingKey* selectedKey_; ///< key that was selected by the user
    bool selectedLeftPart_;            ///< when selected key is split, was the left part selected?
    bool dragging_;                    ///< is the user dragging a key?
    int dragLine_;                     ///< number that indicates the line that was dragged using the shift modifier
    int dragLineStartY_;               ///< y position where the drag of the line started
    float dragLineAlphaLeft_;          ///< floating alpha value of the left key of the dragged line
    float dragLineAlphaRight_;         ///< floating alpha value of the right key of the dragged line
    QPoint mousePos_;                  ///< current position of the mouse

    // variables for appearance of widget
    int padding_;           ///< additional border of the widget
    int arrowLength_;       ///< length of the arrows at the end of coordinate axes
    int arrowWidth_;        ///< width of the arrows at the end of coordinate axes
    float splitFactor_;     ///< offset between splitted keys
    int pointSize_;         ///< size of a key of the transfer function
    int minCellSize_;       ///< minimum size of a grid cell
    Imath::V2f xRange_;      ///< range in x direction
    Imath::V2f yRange_;      ///< range in y direction
    Imath::V2f gridSpacing_; ///< width and height of the underlying grid
    bool clipThresholds_;   ///< is the visible range clipped to threshold area?
    bool noColor_;          ///< when true the color of a key can not be changed

    QString xAxisText_;     ///< caption of the x axis
    QString yAxisText_;     ///< caption of the y axis

    QMenu keyContextMenu_;   ///< context menu for right mouse click when a key is selected
    QMenu noKeyContextMenu_; ///< context menu for right mouse click when no key is selected

    QAction* splitMergeAction_; ///< action for split/merge context menu entry
    QAction* zeroAction_;       ///< action for zero to right context menu entry
    QAction* deleteAction_;     ///< action for delete key context menu entry
    QAction* loadAction_;       ///< action for load transfer function context menu entry
    QAction* saveAction_;       ///< action for save transfer function context menu entry
    QAction* resetAction_;      ///< action for reset transfer function context menu entry

};

}

#endif // SWWGL_TRANSFUNCMAPPINGCANVAS_H
