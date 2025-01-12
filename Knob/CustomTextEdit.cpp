#include "CustomTextEdit.h"

CustomTextEdit::CustomTextEdit(QWidget *parent) :
    QTextEdit(parent)
{
}


void CustomTextEdit::focusOutEvent(QFocusEvent *e){

    Q_EMIT editingFinished();
    QTextEdit::focusOutEvent(e);
}
