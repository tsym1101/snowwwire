#ifndef SWW_PLUGBASE_H
#define SWW_PLUGBASE_H

#include "CoreAPI.h"

#include <vector>
#include <string>
#include <memory> // std::shared_ptr, std::static_pointer_cast
#include <QString>
#include "Core/SwwTypeDesc.h"
#include <map>
#include "Core/VectorHelper.h"

#include "Core/SwwAssert.h"

#include <QMutexLocker>
#include <QObject>
#include "Core/Chainable.h"

namespace SwwKnob {
class UndoCommand;
}

SWW_CORE_NAMESPACE_BEGIN

class SWWCORESHARED_EXPORT PlugColor {
public:
    PlugColor(unsigned char r, unsigned char g, unsigned char b) : _r(r), _g(g), _b(b) {}
    unsigned char r() const { return _r; }
    unsigned char g() const { return _g; }
    unsigned char b() const { return _b; }
private:
    unsigned char _r;
    unsigned char _g;
    unsigned char _b;
};

class PlugBase;
struct SWWCORESHARED_EXPORT PlugMeta {
    std::string name;
    bool isConnectable;
    bool isHidden;
    PlugMeta(PlugBase* p);
};

class NodeBase;

class SWWCORESHARED_EXPORT PlugBase : public QObject, public Chainable {
    Q_OBJECT

public:
    friend class JsonSerializer;
    friend class SwwKnob::UndoCommand;

    typedef std::shared_ptr<PlugBase> Ptr;
    typedef std::shared_ptr<const PlugBase> ConstPtr;

    PlugBase(const std::string& name = "noName");
    virtual ~PlugBase();

    virtual SwwTypeDesc getSwwTypeDesc() const = 0;
    virtual std::string getSwwTypeDescString() const = 0;
    virtual void cloneDataTo(PlugBase* plg) = 0;
    virtual bool setValueString(const QString& value) { return true; }
    virtual QString valueToString() { return QString(); }
    QString getValueStr();
    virtual void resetData() {}

    virtual bool match(PlugBase* plug);

    void affects(PlugBase* plug) { addNext(plug); }
    void unaffects(PlugBase* plug) { removeNext(plug); }

    void pushDirty(bool lock = true);
    bool postSetRoutine(bool lock = true);
    bool postConnectionChanged(bool connected);

    virtual bool pull(bool locked = true);

    std::string getDebugName() const;

    void setOutput(bool b);
    void setMultiConnect(bool b) { _isMultiConnect = b; }
    void setHidden(bool b) { _isHidden = b; }
    void setConnectable(bool b) { _isConnectable = b; }
    void setAutoUpdate(bool b) { _isAutoUpdate = b; }
    void setStorable(bool b) { _isStorable = b; }
    void setDynamic(bool b) { _isDynamic = b; }
    void setPassthrough(bool b) { _isPassthrough = b; }
    void setUserDefined(bool b) { _isUserDefined = b; }

    bool isOutput() const { return _isOutput; }
    bool isMultiConnect() const { return _isMultiConnect; }
    bool isConnectable() const { return _isConnectable; }
    bool isHidden() const { return _isHidden; }
    bool isAutoUpdate() const { return _isAutoUpdate; }
    bool isStorable() const { return _isStorable; }
    bool isDynamic() const { return _isDynamic; }
    bool isPassthrough() const { return _isPassthrough; }
    bool isUserDefined() const { return _isUserDefined; }

    virtual bool isFull() const;

    void setNode(NodeBase* nodePtr) { _node = nodePtr; }
    NodeBase* getNode() const { return _node; }
    int index() const { return _index; }
    void index(int index) { _index = index; }
    void setKnobType(int knobTypeDesc) { _knobTypeDesc = knobTypeDesc; }
    int getKnobType() const { return _knobTypeDesc; }
    void setClean();
    void setDirty();
    bool isClean() const { return _isClean; }

    void printConnections() const;

    void getConnPlugNames(std::vector<std::string>& prevNames,
                          std::vector<std::string>& nextNames,
                          bool getInternal = false) const;

    void getConnPlugNames(std::vector<std::string>& names,
                          bool getPrev,
                          bool getInternal = false) const;

    int getConnectedPlugCount() const { return _next.size(); }

    virtual PlugColor getPlugColor() const { return PlugColor(128, 128, 128); }
    virtual bool cloneWhenDisconnect() const { return false; }

    bool isInheritedAndHidden() const { return _isInheritedAndHidden; }
    void setInheritedAndHidden(bool b) { _isInheritedAndHidden = b; }

    bool isExternalConnection() const;
    void setExternalConnection(bool b);

    std::string getPath() const;
    bool inSameGraph(const PlugBase* p) const;

    void emitPlugStateChanged();

    bool raisePlug();

    bool sameParent(const PlugBase* p) const { return _node == p->_node; }

Q_SIGNALS:
    void plugStateChanged();

protected:
    NodeBase* _node;
    void prepareDestroy();

private:
    bool disconnect(PlugBase* plug, bool withCleanUp = true);
    void getConnPlugNames(const std::vector<Chainable*>& target,
                          std::vector<std::string>& names,
                          bool getInternal = false) const;

    bool connectOk(PlugBase* plug);

    int _index;
    int _knobTypeDesc;

    bool _isClean;
    bool _isOutput;
    bool _isMultiConnect;
    bool _isAutoUpdate;
    bool _isStorable;
    bool _isDynamic;
    bool _isUserDefined;
    bool _isHidden;
    bool _isConnectable;
    bool _isPassthrough;

    bool _isInheritedAndHidden;
    bool _firstPushDirtyCalled;

    bool _isExternalConnection;
};

template<typename PlugType>
inline typename PlugType::Ptr
plugPtrCast(const PlugBase::Ptr& plugBasePtr) {
    if (plugBasePtr && plugBasePtr->getSwwTypeDesc() == PlugType::typeDesc()) {
        return std::static_pointer_cast<PlugType>(plugBasePtr);
    }
    return typename PlugType::Ptr();
}

template<typename PlugType>
inline typename PlugType::ConstPtr
plugPtrCast(const PlugBase::ConstPtr& plugBaseConstPtr) {
    return std::const_pointer_cast<const PlugType>(
        plugPtrCast(std::const_pointer_cast<PlugBase>(plugBaseConstPtr))
    );
}

SWW_CORE_NAMESPACE_END
#endif // SWW_PLUGBASE_H
