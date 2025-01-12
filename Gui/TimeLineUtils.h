#ifndef SWW_GLUTILS_H
#define SWW_GLUTILS_H

#include <GL/glew.h>

#include <set>
#include <cmath>
#include <QString>
#include <vector>

#include "Gui/GuiGlobal.h"

// the following macro only tests the Control, Alt, and Shift modifiers, and discards the others
#define modCAS(e) ( (e)->modifiers() & (Qt::ControlModifier | Qt::AltModifier | Qt::ShiftModifier) )

#define modCASIsNone(e)            ( modCAS(e) == (Qt::NoModifier) )
#define modCASIsControl(e)         ( modCAS(e) == (Qt::ControlModifier) )
#define modCASIsControlShift(e)    ( modCAS(e) == (Qt::ControlModifier | Qt::ShiftModifier) )
#define modCASIsControlAlt(e)      ( modCAS(e) == (Qt::ControlModifier | Qt::AltModifier) )
#define modCASIsControlAltShift(e) ( modCAS(e) == (Qt::ControlModifier | Qt::AltModifier | Qt::ShiftModifier) )
#define modCASIsAlt(e)             ( modCAS(e) == (Qt::AltModifier) )
#define modCASIsAltShift(e)        ( modCAS(e) == (Qt::AltModifier | Qt::ShiftModifier) )
#define modCASIsShift(e)           ( modCAS(e) == (Qt::ShiftModifier) )

// macros to test if the modifier is present (but there may be other modifiers too)
#define modifierHasControl(e)         ( (e)->modifiers().testFlag(Qt::ControlModifier) )
#define modifierHasAlt(e)             ( (e)->modifiers().testFlag(Qt::AltModifier) )
#define modifierHasShift(e)           ( (e)->modifiers().testFlag(Qt::ShiftModifier) )

// macros to test if a button is held down (pressed), or a single-button compatibility combination
// Right click emulated with Left + MetaModifier (ctrl key on the keyboard), which is the way its done everywhere else on the mac
// Middle click emulated with Left + AltModifier (alt key on the keyboard)
#define buttonMetaAlt(e)          ( (e)->modifiers() & (Qt::MetaModifier | Qt::AltModifier) )

/// THE GOOD VERSION
// PLEASE DON'T BREAK THIS.
// THANK YOU VERY VERY VERY MUCH.
#define buttonDownIsLeft(e)              ( ( (e)->buttons() == Qt::LeftButton   && buttonMetaAlt(e) == Qt::NoModifier ) )

//Right click emulated with Left + MetaModifier, which is the way its done everywhere else on the mac
#define buttonDownIsRight(e)             ( ( (e)->buttons() == Qt::RightButton  && buttonMetaAlt(e) == Qt::NoModifier ) || \
                                           ( (e)->buttons() == Qt::LeftButton   && buttonMetaAlt(e) == Qt::MetaModifier ) )

#define triggerButtonIsLeft(e) ( (e)->button() == Qt::LeftButton && buttonMetaAlt(e) == Qt::NoModifier )
#define triggerButtonIsRight(e) ( (e)->button() == Qt::RightButton || \
                                  ( (e)->button() == Qt::LeftButton   && buttonMetaAlt(e) == Qt::MetaModifier ) )


#define buttonDownIsMiddle(e)            ( ( (e)->buttons() == Qt::MiddleButton && buttonMetaAlt(e) == Qt::NoModifier ) || \
                                           ( (e)->buttons() == Qt::LeftButton   && buttonMetaAlt(e) == Qt::AltModifier ) )


// macros to test the button that triggered the event
#define triggerButtonIsMiddle(e) ( (e)->button() == Qt::MiddleButton || \
                                   ( (e)->button() == Qt::LeftButton   && buttonMetaAlt(e) == Qt::AltModifier ) )

enum TimelineStateEnum
{
    eTimelineStateIdle,
    eTimelineStateDraggingCursor,
    eTimelineStateDraggingBoundary,
    eTimelineStatePanning,
    eTimelineStateSelectingZoomRange,
};

template <typename T>
T adjustSizeToDPIX(T x)
{
    return x * SwwGui::GuiGlobal::getLogicalDPIXRATIO();
}

template <typename T>
T adjustSizeToDPIY(T y)
{
    return y * SwwGui::GuiGlobal::getLogicalDPIYRATIO();
}

#define TO_DPIX(x) ( adjustSizeToDPIX(x) )
#define TO_DPIY(y) ( adjustSizeToDPIY(y) )

enum TimelineChangeReasonEnum
{
    eTimelineChangeReasonUserSeek = 0,
    eTimelineChangeReasonPlaybackSeek = 1,
    eTimelineChangeReasonCurveEditorSeek = 2,
    eTimelineChangeReasonDopeSheetEditorSeek = 3,
    eTimelineChangeReasonOtherSeek
};

namespace Image {
template <typename PIX>
static PIX clamp(PIX x, PIX minval, PIX maxval);
template <>
inline double
Image::clamp(double x,
             double minval,
             double maxval)
{
    return std::min(std::max(minval, x), maxval);
}

}

typedef int SequenceTime;

struct TimeLineKey
{
    SequenceTime time;
    bool isUserKey;

    explicit TimeLineKey(SequenceTime t,
                         bool isUserKey = false)
        : time(t)
        , isUserKey(isUserKey)
    {
    }

    explicit TimeLineKey()
        : time(0)
        , isUserKey(false)
    {
    }
};

struct TimeLineKey_compare
{
    bool operator() (const TimeLineKey& lhs,
                     const TimeLineKey& rhs) const
    {
        return lhs.time < rhs.time;
    }
};

typedef std::set<TimeLineKey, TimeLineKey_compare> TimeLineKeysSet;

static
QString
timecodeString(double value, double fps)
{
    QString sign;
    if (value < 0) {
        value = -value;
        sign = QLatin1Char('-');
    }
    long rvalue = (long)value;
    int rfps = (int)std::ceil(fps);

    int f = rvalue % rfps;
    rvalue /= rfps;
    int s = rvalue % 60;
    rvalue /= 60;
    int m = rvalue % 60;
    int h = rvalue / 60;
    return sign + QString::fromUtf8("%1:%2:%3:%4")
            .arg(h, 2, 10, QLatin1Char('0'))
            .arg(m, 2, 10, QLatin1Char('0'))
            .arg(s, 2, 10, QLatin1Char('0'))
            .arg(f, 2, 10, QLatin1Char('0'));
}

// Exponentiation by squaring
// works with positive or negative integer exponents.
template<typename T>
double
ipow(T base,
     int exp)
{
    double result = 1.;

    if (exp >= 0) {
        while (exp) {
            if (exp & 1) {
                result *= base;
            }
            exp >>= 1;
            base *= base;
        }
    } else {
        exp = -exp;
        while (exp) {
            if (exp & 1) {
                result /= base;
            }
            exp >>= 1;
            base *= base;
        }
    }

    return result;
}

void ticks_size(
        double xmin,              // 表示範囲の最小値
        double xmax,              // 表示範囲の最大値
        double range_units,       // 表示幅（ピクセル単位）
        double min_tick_size_units, // 最小の目盛り間隔（ピクセル単位）
        double *t_size,          // 出力：計算された目盛りの間隔
        bool* half_tick          // 出力：半分の間隔を使うかどうか
        );

void
ticks_bounds(double xmin,
             double xmax,
             double tick_width,
             bool half_tick,
             int tick_max,
             double *offset,
             int* m1,
             int* m2);

void
ticks_fill(bool half_tick,
           int tick_max,
           int m1,
           int m2,
           std::vector<int>* ticks);

// compute alpha value for drawing the ticks
double
ticks_alpha(double min,
            double max,
            double val);


// an RAII helper class to push/pop attribs
class GLProtectAttrib
{
public:
    GLProtectAttrib(GLbitfield mask = GL_ALL_ATTRIB_BITS)
    {
#ifdef DEBUG
        GLint d = -1, m0 = -1, m = -1;
        m0 = 16; // https://www.opengl.org/sdk/docs/man2/xhtml/glPushAttrib.xml
        glGetIntegerv(GL_ATTRIB_STACK_DEPTH, &d);
        glGetIntegerv(GL_MAX_ATTRIB_STACK_DEPTH, &m);
        assert(m >= m0);
        if (d >= m0) {
            overflow(d, m0, m);
        }
#endif
        glPushAttrib(mask);
#ifdef DEBUG
        glCheckError();
#endif
    }

    ~GLProtectAttrib() { glPopAttrib(); }

private:
#ifdef DEBUG
    void overflow(GLint d,
                  GLint m0,
                  GLint m)
    {
        if (d >= m) {
            std::cout << "GLProtectAttrib: stack overflow (max depth is " << m << ")";
        } else {
            std::cout << "GLProtectAttrib: stack may overflow on a minimal OpenGL system (depth is " << d << ", max depth is " << m << " - but at least " << m0 << ")";
        }
        std::cout << " Add breakpoint in GLProtectAttrib::overflow()." << std::endl;
    }

#endif
};

// an RAII helper class to push/pop matrix
class GLProtectMatrix
{
public:
    GLProtectMatrix(GLenum mode)
        : _mode(mode)
    {
#ifdef DEBUG
        GLint d = -1, m0 = -1, m = -1;
        if (mode == GL_PROJECTION) {
            m0 = 2; // https://www.opengl.org/sdk/docs/man2/xhtml/glPushMatrix.xml
            glGetIntegerv(GL_PROJECTION_STACK_DEPTH, &d);
            //std::cout << "GLProtectMatrix(GL_PROJECTION): depth is " << d << "\n";
            glGetIntegerv(GL_MAX_PROJECTION_STACK_DEPTH, &m);
        } else {
            m0 = 32; // https://www.opengl.org/sdk/docs/man2/xhtml/glPushMatrix.xml
            glGetIntegerv(GL_MODELVIEW_STACK_DEPTH, &d);
            glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH, &m);
        }
        assert(m >= m0);
        if (d >= m0) {
            overflow(d, m0, m);
        }
#endif
        glMatrixMode(_mode);
        glPushMatrix();
    }

    ~GLProtectMatrix() { glMatrixMode(_mode); glPopMatrix(); }

private:
    GLenum _mode;

#ifdef DEBUG
    void overflow(GLint d,
                  GLint m0,
                  GLint m)
    {
        if (d >= m) {
            std::cout << "GLProtectMatrix(GL_" << (_mode == GL_PROJECTION ? "PROJECTION" : "MODELVIEW") << "): stack overflow (max depth is " << m << ")";
        } else {
            std::cout << "GLProtectMatrix(GL_" << (_mode == GL_PROJECTION ? "PROJECTION" : "MODELVIEW") << "): stack may overflow on a minimal OpenGL system (depth is " << d << ", max depth is " << m << " - but at least " << m0 << ")";
        }
        std::cout << " Add breakpoint in GLProtectMatrix::overflow()." << std::endl;
    }

#endif
};


#endif // SWW_GLUTILS_H
