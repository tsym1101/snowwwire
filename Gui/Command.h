#ifndef SWW_COMMAND_H
#define SWW_COMMAND_H

#include <QUndoCommand>
#include <vector>
#include "Core/SwwJson.h"

namespace Sww::Core{
class Node;
}

class NodeGraphWidget;

namespace SwwGui {
class NodeGraphScene;

class RemoveCommand : public QUndoCommand{
public:
    RemoveCommand(const std::vector<Sww::Core::Node*>& target,NodeGraphScene* scene);
    virtual void undo();
    virtual void redo();

private:
    std::string _undoJson;
    std::string _redoJson;
    NodeGraphScene* _scene;
    bool _first;
};

class MoveCommand : public QUndoCommand{
public:
    MoveCommand(const std::vector<Sww::Core::Node*>& targetBeforeMove, NodeGraphScene* scene);
    virtual void undo();
    virtual void redo();
    void setAfterPos(const std::vector<Sww::Core::Node*>& targetAfterMove);
private:
    std::string _undoJson;
    std::string _redoJson;
    NodeGraphScene* _scene;
    bool _first;

};

class SelectCommand : public QUndoCommand{
public:
    SelectCommand(const std::vector<std::string> &targetBeforeSelect, NodeGraphScene* scene);
    virtual void undo();
    virtual void redo();
    void setAfterSelect(const std::vector<std::string> &targetAfterSelect);

private:
    void parse(SwwJson &j);
    std::string _undoJson;
    std::string _redoJson;
    NodeGraphScene* _scene;
    bool _first;
};

class ChangeSceneCommand : public QUndoCommand{
public:
    ChangeSceneCommand(const std::string& beforePath,
                      const std::string& afterPath,
                      NodeGraphWidget* ngw);
    virtual void undo();
    virtual void redo();

private:
    std::string _beforePath;
    std::string _afterPath;
    NodeGraphWidget* _ngw;
    bool _first;
};


}//ns

#endif // SWW_COMMAND_H
