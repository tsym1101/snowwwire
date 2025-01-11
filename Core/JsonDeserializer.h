#ifndef JSONDESERIALIZER_H
#define JSONDESERIALIZER_H

#include "Core/CoreAPI.h"
#include <string>
#include "Core/SwwJson.h"
#include <ImathBox.h>
#include <QFlags>

SWW_CORE_NAMESPACE_BEGIN

class RootNode;
class Node;
class PlugBase;

class SWWCORESHARED_EXPORT JsonDeserializer
{
public:
    enum JsonDeserializerFlag{
        kNone     = 0x00,
        kSolveNameConflict = 0x01,
        kJustRunCommand = 0x02
    };

//    enum ExcutedCommandTypeFlag{
//        kDoNothing  = 0x00,
//        kRemove     = 0x01,
//        kSelect     = 0x02,
//        kDeselect   = 0x04,
//        kTransition = 0x08
//    };

    Q_DECLARE_FLAGS(JsonDeserializerFlags, JsonDeserializerFlag)
    //Q_DECLARE_FLAGS(ExcutedCommandTypeFlags, ExcutedCommandTypeFlag)

    struct DelayedValue{
        std::vector<std::string> values; //パースした値
        std::vector<std::string> dynamicValues; //パースした値 dynamic
        Sww::Core::Node *targetNode;//値をセットするノード
    };

    JsonDeserializer();
    bool deserialize(const std::string& fileName, Sww::Core::RootNode *rootNode);
    //recurcive function.
    bool deserialize(SwwJson &j, Sww::Core::Node *node,
                     Imath::Box2f& area);
    void runCommand(SwwJson &j, Sww::Core::Node *node);

    void setFlags(JsonDeserializerFlags flag);
    void resetFlags();
    void removeFlags(JsonDeserializerFlags flag);

//    ExcutedCommandTypeFlags getExcutedCommandFlags()const{
//        return _excutedCommandflags;
//    }

private:
    std::vector<DelayedValue> _delayedValues;
    std::vector<Sww::Core::Node*> _delayedConncetTargets;
    int _depth;

    bool delayedConstruct();
    bool solveNameConflict(SwwJson &j, Sww::Core::Node *node);


    std::vector<Node*> _updatedNodes;

    JsonDeserializerFlags _flags;
    //ExcutedCommandTypeFlags _excutedCommandflags;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(JsonDeserializer::JsonDeserializerFlags)
//Q_DECLARE_OPERATORS_FOR_FLAGS(JsonDeserializer::ExcutedCommandTypeFlags)

SWW_CORE_NAMESPACE_END

#endif // JSONDESERIALIZER_H
