#ifndef CUSTOMTEXTEDIT_H
#define CUSTOMTEXTEDIT_H

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include  <QTextEdit>
#else
#include <QtWidgets/QTextEdit>
#endif

class CustomTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit CustomTextEdit(QWidget *parent = 0);
    
protected:
    virtual void focusOutEvent(QFocusEvent *e);
Q_SIGNALS:
    void editingFinished();
    
public Q_SLOTS:
    
};

#endif // CUSTOMTEXTEDIT_H
