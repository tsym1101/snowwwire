#ifndef JSONSERIALIZER_H
#define JSONSERIALIZER_H

#include "Core/CoreAPI.h"
#include <string>
#include "Core/SwwJson.h"

#include <QFlags>
#include <QStringList>

SWW_CORE_NAMESPACE_BEGIN
class RootNode;
class Node;
class PlugBase;

class SWWCORESHARED_EXPORT JsonSerializer
{
public:
    enum JsonSerializerFlag{
        None     = 0x00,
        asOverWriteMode = 0x01,
        getChildren = 0x02,
        useType  = 0x04,
        usePos   = 0x08,
        useConn  = 0x10,
        useValue = 0x20,
        useDValue= 0x40,
        useUdd   = 0x80
    };

    enum CommandType{
        kRemove,
        kSelect,
        kDeselect
        //kTransition
    };

    Q_DECLARE_FLAGS(JsonSerializerFlags, JsonSerializerFlag)

    JsonSerializer();
    //std::string serialize()const;
    std::string serializeRoot(RootNode* rootNode)const;
    std::string serialize(const std::vector<Node *> &nodes)const;
    std::string serialize(PlugBase* plug);
    void serializeProperty(Node* node,SwwJson& j)const;

    SwwJson makeCommand(const std::vector<std::string> &nodeNames, CommandType cType);

    void setFlags(JsonSerializerFlags flag);
    void resetFlags();
    void removeFlags(JsonSerializerFlags flag);
    void addToValueList(PlugBase *plug);
private:
    SwwJson traverse(Node* node)const;//再帰
    void serializeChildren(Node* node,SwwJson& j)const;
    JsonSerializerFlags _flags;
    QStringList _valueList;//シリアライズしたい値がある場合に使用
};

Q_DECLARE_OPERATORS_FOR_FLAGS(JsonSerializer::JsonSerializerFlags)

SWW_CORE_NAMESPACE_END

#endif // JSONSERIALIZER_H
