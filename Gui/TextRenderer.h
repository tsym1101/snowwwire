#ifndef SWWGUI_TEXTRENDERER_H
#define SWWGUI_TEXTRENDERER_H

#include <QtCore/QString>
#include <QtGui/QFont>
#include <QtGui/QImage>
#include <QtCore/QHash>

#include <GL/glew.h>
#include <QPainter>
#include <QOpenGLWidget>

class TextRenderer
{
public:

    TextRenderer();

    ~TextRenderer();

    void renderText(float x,
                    float y,
                    float scalex,
                    float scaley,
                    const QString &text,
                    const QColor &color,
                    const QFont &font,
                    int flags = 0) const; //!< see http://doc.qt.io/qt-4.8/qpainter.html#drawText-10

private:
    struct Implementation;
    std::unique_ptr<Implementation> _imp;
};

#endif // SWWGUI_TEXTRENDERER_H
