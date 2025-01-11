#ifndef SWW_NODEBASE_H
#define SWW_NODEBASE_H

#include "CoreAPI.h"
#include <string>
#include <memory> // std::shared_ptr
#include <QMutex>
#include "Core/Chainable.h"

//**************************
// Node インターフェイス
// PlugBase からアクセスする最低限の情報を保持
//**************************

SWW_CORE_NAMESPACE_BEGIN

class PlugBase;

class SWWCORESHARED_EXPORT NodeBase : public Meta {
public:
    typedef std::shared_ptr<NodeBase> Ptr;
    typedef std::shared_ptr<const NodeBase> ConstPtr;

    NodeBase(const std::string& name = "noName")
        : /*_name(name),*/ _isInternal(false), _isInitialized(false), _errorOccurred(false) {
        setName(name);
    }
    virtual ~NodeBase();

    virtual bool initialize() = 0;
    virtual bool execute(const PlugBase* plug) = 0;
    virtual bool postSetProcess(const PlugBase* p) { return true; }
    virtual bool preInitialConnect() { return true; }
    virtual bool postConnectionChanged(const PlugBase* p, bool connected) { return true; }
    virtual void setErrorOccurred(bool occurred) = 0;
    virtual std::string getPath() const = 0;

    bool errorOccurred() const { return _errorOccurred; }

    bool isInternal() const { return _isInternal; }
    void setInternal(bool b) { _isInternal = b; }

    void setInitialized() { _isInitialized = true; }
    bool isInitialized() const { return _isInitialized; }

    QMutex* mutex() { return &_mutex; }

protected:
    bool _errorOccurred;

private:
    // std::string _name;
    bool _isInternal;
    bool _isInitialized;

    QMutex _mutex;
};

SWW_CORE_NAMESPACE_END

#endif // SWW_NODEBASE_H
