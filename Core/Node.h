#ifndef SWW_NODE_H
#define SWW_NODE_H

#include "CoreAPI.h"
#include <vector>
#include <map>
#include <memory> // std::shared_ptr
#include <QStringList>
#include "SwwMacro.h"
#include "NodeBase.h"
#include "SwwAssert.h"
#include "Core/SwwTypeDef.h"

#include "Core/DualMap.h"
#include <QMap>
#include <QMutexLocker>

#define SWW_VAL_RETURN(plugPtr, result, returnType)  \
    auto result = plugPtr->initialValue();          \
    {                                               \
        bool valid;                                 \
        result = plugPtr->getConstInputValue(&valid);\
        if (!valid) {                               \
            SWW_ERR("getConstInputValue() failed. " << plugPtr->getDebugName())\
            return returnType;                      \
        }                                           \
    }

#define SWW_VAL(plugPtr, result)                   \
    auto result = plugPtr->initialValue();          \
    {                                               \
        bool valid;                                 \
        result = plugPtr->getConstInputValue(&valid);\
        if (!valid) {                               \
            SWW_ERR("getConstInputValue() failed. " << plugPtr->getDebugName())\
            return false;                           \
        }                                           \
    }

#define SWW_VAL_VOID(plugPtr, result) SWW_VAL_RETURN(plugPtr, result, )

#define SWW_JUST_UPDATE(plugPtr)                   \
    {                                               \
        bool valid;                                 \
        plugPtr->getConstInputValue(&valid);        \
        if (!valid) {                               \
            SWW_ERR("getConstInputValue() failed. " << plugPtr->getDebugName())\
            return false;                           \
        }                                           \
    }

namespace SwwGui {
class NodeGraphScene;
class NodeItem;
class XmlWriter;
}

namespace SwwTest {
class UnitTest;
}

SWW_CORE_NAMESPACE_BEGIN

class PlugBase;
class IntPlug;
class ConnectHandler;
class RootNode;

struct ConnectState {
    std::string outNodeName;
    std::string outPlugName;
    std::string inNodeName;
    std::string inPlugName;
};

class SWWCORESHARED_EXPORT Node : public QObject, public NodeBase {
    Q_OBJECT

public:
    friend class RootNode;
    friend class ConnectHandler;
    friend class JsonSerializer;
    friend class JsonDeserializer;
    friend class NodeRegister;
    friend class SwwGui::NodeItem;
    friend class SwwGui::NodeGraphScene;
    friend class ConnectDialog;
    friend class SwwTest::UnitTest;

    typedef DualMap<std::shared_ptr<PlugBase>> PlugBasePtrArray;
    typedef PlugBasePtrArray::thisVec PlugBasePtrVec;
    typedef PlugBasePtrArray::thisMap PlugBasePtrMap;
    typedef std::shared_ptr<Node> Ptr;

    Node();
    virtual ~Node();

    bool connect(std::string outputNodeName, const std::string &outputPlugName,
                 std::string inputNodeName, const std::string &inputPlugName);
    static bool disconnect(PlugBase* p1, PlugBase* p2);
    bool createNode(const std::string &nodeType);
    bool createNode(const std::string &nodeType, Node*& nodePtr, const std::string &name = std::string());
    std::string genUniqueNodeName(const std::string& baseName, bool useBaseNameIfUnique = true);
    void printAllNodes();
    void printClean();
    void printAllInternalConnection();
    bool removeNode(std::string nodeName);

    bool isEmpty() const { return _children.empty(); }

    bool connectByHandler();
    void add2ConnectHandler(ConnectHandler* c, int index = 0);

    void addPlug(PlugBase* plug);
    bool addDynamicPlug(PlugBase* plug);

    void printIntenalConnection();
    void printCleanState();

    void getConnPlugList(std::vector<std::string>& prevList,
                         std::vector<std::string>& nextList,
                         bool getInternal) const;

    void getValueList(QStringList& list, bool getDynamic);
    bool setValueString(const QString& plugName, const QString& value);

    void setRunner(PlugBase* p);
    std::vector<PlugBase*> getRunnerPlugs();

    bool connectToGlobalTime(PlugBase* plug);

    void runBackGround(PlugBase* p) {
        Q_EMIT runSignal(p);
    }

    void hideInheritedPlug(PlugBase* p, bool notStoreble = true);
    void showInheritedPlug(PlugBase* p, bool storeble = false);

    virtual QString toolTip() const { return QString(); }

    bool deleteDynamicPlugItem(const std::string& plugName);
    void destroyDynamicPlugItemArray();

    bool hasPlug(const std::string& plugName) const;

    void storeDynamicPlugValueState();
    void restoreDynamicPlugValueState();

    void storeDynamicPlugConnectState();
    void restoreDynamicPlugConnectState();

    virtual void setErrorOccurred(bool occurred);

    virtual std::string getPath() const;
    Node* getNodeFromPath(const std::string& path) const;
    std::string getRelativePath(const std::string& absPath) const;
    std::string getAbsolutePath(const std::string& relativePath) const;

    bool inSameGraph(Node* n) const;
    bool hasChild(const std::string& nodeName) const;

    Imath::V2i getGlobalFrameRange() const;

    bool isRunner() const { return !_runnerPlugContainer.empty(); }

    void emitNodeStateChanged();
    void emitRemoveMyself();
    void emitSelectMyself();
    void emitDeselectChildren();

public Q_SLOTS:

Q_SIGNALS:
    void runSignal(PlugBase* plug);
    void emitAddDynamicPlug(PlugBase* plug);
    void emitDeleteDynamicPlug(const PlugMetaPtr& plugMeta);
    void emitConnectDynamicPlug(const ConnectState* con);
    void emitErrorOccurred(bool occurred);
    void emitGraphCleared();
    void nodeStateChanged();
    void removeMyself();
    void selectMyself();
    void deselectChildren();

protected:
    void clearChildren() { _children.clear(); }
    QString readTooltipFromResource(const QString& resourcePath) const;

private:
    Node::Ptr searchNode(QStringList& pathChain) const;
    static bool connect(PlugBase* p1, PlugBase* p2);
    void clearGraph();
    void getUserDefinedDynamicPlugs(std::vector<PlugBase*>& plugs);

    RootNode* _rootNode;
    PlugBasePtrArray _plugArray;
    std::vector<PlugBase*> _runnerPlugContainer;
    std::map<std::string, std::string> _dynamicPlugValueState;
    std::vector<ConnectState> _dynamicPlugConnectState;
    QMap<std::string, Node::Ptr> _children;
    std::map<int, std::vector<ConnectHandler*>> _indexedConnectHndList;
    SWW_SIMPLE_MEMBER_READ_ONLY(Node*, parentNode)
    SWW_SIMPLE_MEMBER(std::string, nodeTypeName)
    SWW_SIMPLE_MEMBER_READ_ONLY(unsigned int, inputConnectablePlugCount)
    SWW_SIMPLE_MEMBER_READ_ONLY(unsigned int, outputConnectablePlugCount)
    SWW_SIMPLE_MEMBER_READ_ONLY(unsigned int, inputConnectablePlugCountDynamic)
    SWW_SIMPLE_MEMBER_READ_ONLY(unsigned int, outputConnectablePlugCountDynamic)
    SWW_SIMPLE_MEMBER(Imath::V2f, posInGraph)
    SWW_SIMPLE_MEMBER_READ_ONLY(bool, inSetValuePhase)
    SWW_SIMPLE_MEMBER_READ_ONLY(bool, inPreInitialConnectPhase)
    SWW_SIMPLE_MEMBER_READ_ONLY(bool, inConnectPhase)
    SWW_SIMPLE_MEMBER_READ_ONLY(bool, inSetDynamicValuePhase)
};

SWW_CORE_NAMESPACE_END

#endif // SWW_NODE_H
