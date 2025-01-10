#ifndef SWWMACRO_H
#define SWWMACRO_H

#include <QDebug>

//ローカルスコープ内でメッセージボックスを表示するマクロ
#define NOW_LOADING     QMessageBox messageBox(QMessageBox::NoIcon,\
    QString("ds"),\
    QString("loading image..."),\
    QMessageBox::NoButton);\
    messageBox.setStandardButtons(0);\
    messageBox.show();

#define SWW_SIMPLE_MEMBER(type,name) \
    public:\
    void name(const type &v){_##name = v;}\
    type name()const {return _##name;}\
    private:\
    type _##name;

#define SWW_SIMPLE_MEMBER_READ_ONLY(type,name) \
    public:\
    type name()const {return _##name;}\
    private:\
    type _##name;

#define SWW_SIMPLE_MEMBER_PROTECTED(type,name) \
    public:\
    void name(const type &v){_##name = v;}\
    type name()const {return _##name;}\
    protected:\
    type _##name;

#define SWW_PRINT(value) qDebug() << #value << "=" << value;

//#define SWW_ERR_LOC  qDebug() << "Error at line" << __LINE__ << "in " << __FILE__;


#define SWW_CHECK_STATUS(value) \
    if(value != Sww::Core::SUCCESS){\
    SWW_ERR("check status is not SUCCESS.")\
    return Sww::Core::FAILED;\
    }

#define SWW_CHECK_BOOL(value) \
    if(!value){\
    SWW_ERR("invalid.")\
    return false;\
    }

#endif // SWWMACRO_H


