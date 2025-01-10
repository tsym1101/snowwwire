#ifndef SWW_PLUG_H
#define SWW_PLUG_H

#include "PlugBase.h"
#include "VectorHelper.h"
#include "SwwAssert.h"

#include "Core/AutoUpdateLog.h"

#include <memory> // std::shared_ptr, std::make_shared

#include "NodeBase.h"

SWW_CORE_NAMESPACE_BEGIN

#define SWW_GET(type, result, plugPtr, valid)  \
    const type& result = plugPtr->getConstInputValue(&valid); \
    if (!valid) { \
        SWW_ERR("getConstInputValue() failed. " << plugPtr->getDebugName()) \
        return false; \
    }

template <class T>
class Plug : public PlugBase {

public:
    typedef T thisType;
    typedef Plug<T> thisPlugType;
    typedef std::shared_ptr<T> dataPtr;

    typedef std::shared_ptr<thisPlugType> Ptr;
    typedef std::shared_ptr<const thisPlugType> ConstPtr;

    Plug(const std::string& name = "noName", const T& value = T())
        : PlugBase(name) {
        _data = std::make_shared<T>(value);
    }

    virtual ~Plug() {
        prepareDestroy(); // この中で_dataを操作するので、PlugBaseでは呼び出さない
    }

    bool setInputValue(const T& value, bool lock = true);
    void setOutputValue(const T& value);

    const T& getConstInputValue(bool* ok, int connectedIndex = 0);

    void getOutputValue(T& value);
    const T& getConstOutputValue() const;

    T& rawValue();
    const T& rawValue() const;

    bool dataIsNull() { return !_data; }
    void printDataValue() const { std::cout << getDebugName() << ":" << *_data << std::endl; }
    virtual void cloneDataTo(PlugBase* plg); // データのみコピー

    void resetData() { _data = std::make_shared<T>(T()); }

    virtual bool pull(bool locked = true);

    T initialValue() const { return T(); }

    virtual bool match(PlugBase* plug) { return PlugBase::match(plug); }

private:
    dataPtr _data;

    void set(const T& value);
    void get(T& value) const;

    Plug(const Plug& p);            // コピーコンストラクタ
    Plug& operator=(const Plug& p); // 代入演算子オーバーロード
};

template <class T>
inline void Plug<T>::set(const T& value) {
    *_data = value;
}

template <class T>
inline void Plug<T>::get(T& value) const {
    Q_ASSERT(_data);
    value = *_data;
}

template <class T>
inline bool Plug<T>::setInputValue(const T& value, bool lock) {
    Q_ASSERT_X(!isOutput(), getDebugName().c_str(), "setInputValue failed.");
    set(value);
    return postSetRoutine(lock);
}

template <class T>
inline void Plug<T>::setOutputValue(const T& value) {
    Q_ASSERT(isOutput());
    set(value);
}

template <class T>
inline const T& Plug<T>::getConstInputValue(bool* ok, int connectedIndex) {
    Q_ASSERT_X(!isOutput(), getDebugName().c_str(), "output.");

    if (isClean()) {
        *ok = true;
        return *_data;
    }

    if (prevCount()) {
        PlugBase* p = static_cast<PlugBase*>(_prev[connectedIndex]);
        thisPlugType* typedPlug = static_cast<thisPlugType*>(p);

        if (typedPlug->isOutput()) {
            if (!typedPlug->pull()) {
                *ok = false;
                return typedPlug->rawValue();
            }
            setClean();
            set(typedPlug->rawValue());
            *ok = true;
            return *_data;
        } else {
            const T& value = typedPlug->getConstInputValue(ok, connectedIndex);

            if (*ok == true) {
                setClean();
                set(value);
            }
            return value;
        }
    } else {
        setClean();
        *ok = true;
        return *_data;
    }
}

template <class T>
inline void Plug<T>::getOutputValue(T& value) {
    Q_ASSERT(isOutput());
    get(value);
}

template <class T>
inline const T& Plug<T>::getConstOutputValue() const {
    Q_ASSERT(isOutput());
    return *_data;
}

template <class T>
inline T& Plug<T>::rawValue() {
    return *_data;
}

template <class T>
inline const T& Plug<T>::rawValue() const {
    return *_data;
}

template <class T>
inline void Plug<T>::cloneDataTo(PlugBase* dst) {
    SWW_DEBUG("cloneDataTo : " << getName() << "," << dst->getName());

    thisPlugType* dstT = dynamic_cast<thisPlugType*>(dst);
    Q_ASSERT(dstT);
    dstT->_data.reset(new T(*_data));
}

template <class T>
inline bool Plug<T>::pull(bool locked) {
    Q_ASSERT_X(isOutput(), getDebugName().c_str(), "pull in input plug.");

    bool mustPull = true;
    for (auto c : _prev) {
        thisPlugType* typedPlug = static_cast<thisPlugType*>(c);
        if (sameParent(typedPlug)) continue;
        if (!typedPlug->pull(locked)) {
            SWW_ERR("pull failed. " << typedPlug->getDebugName());
            return false;
        }
        set(typedPlug->rawValue());
        mustPull = false;
        setClean();
        break;
    }

    if (mustPull) {
        if (locked) {
            _node->mutex()->lock();
        }

        if (!getNode()->execute(this)) {
            getNode()->setErrorOccurred(true);
            if (locked) _node->mutex()->unlock();
            return false;
        }
        getNode()->setErrorOccurred(false);
        setClean();

        if (locked) _node->mutex()->unlock();
    }

    return true;
}

SWW_CORE_NAMESPACE_END

#endif // SWW_PLUG_H
