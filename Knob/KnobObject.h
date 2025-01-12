#ifndef KNOBOBJECT_H
#define KNOBOBJECT_H

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QWidget>
#else
#include <QtWidgets/QtWidgets>
#endif

#include <QString>
#include "Core/SwwAssert.h"
#include "Core/Node.h"
#include "Core/Plug.h"

#include "KnobAPI.h"

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QUndoCommand>
#else
#include <QtWidgets/QUndoCommand>
#endif



namespace Sww::Core{
class PlugBase;
}

class SWWKNOBSHARED_EXPORT KnobObject : public QWidget
{
    Q_OBJECT

public:
    KnobObject(Sww::Core::PlugBase *plug ,QWidget *parent = 0);
    virtual ~KnobObject();
    virtual bool initialize(){return true;}
    const std::string& getName()const{return _plugBase->getName();}
    virtual bool needNameTag()const{return true;}

public Q_SLOTS:
    virtual void updateKnob(){} //pullした後、値が変更された後の処理を記述 NodeItem内のCreateKnob内でコネクトされる

Q_SIGNALS:
    void valueModified(bool);

protected:
    bool isOutput()const{return _plugBase->isOutput();}
    void emitValueModified(){Q_EMIT valueModified(true);}
    //bool exec();

private:
    Sww::Core::PlugBase* _plugBase;
    std::string _name;
};

namespace SwwKnob {
class UndoCommand : public QUndoCommand
{
public:
    UndoCommand(Sww::Core::PlugBase* plug);
    void storeUndoValue();
    void push();
public:
    virtual void undo();
    virtual void redo();
private:
    std::string _undoJson;
    std::string _redoJson;
    Sww::Core::Node* _node;
    Sww::Core::PlugBase* _plug;
    bool _first;
};

}//ns

#endif // KNOBOBJECT_H
