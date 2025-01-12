#include "Gui/Command.h"

#include "Gui/NodeGraphScene.h"
//#include <GL/glew.h>

#include "Core/JsonSerializer.h"
#include "Core/JsonDeserializer.h"
#include "Core/SwwAssert.h"
#include "Core/Node.h"
#include "Gui/NodeGraphWidget.h"

namespace SwwGui {

RemoveCommand::RemoveCommand(const std::vector<Sww::Core::Node *> &target, NodeGraphScene *scene):_scene(scene),_first(true){
    using namespace Sww::Core;

    std::vector<std::string> nodeNames;
    QString commnadText = "remove";
    for(auto n:target){
        nodeNames.push_back(n->getName());
        commnadText += QString(" ") + n->getName().c_str();
    }

    JsonSerializer js;
    _undoJson = js.serialize(target);
    _redoJson = js.makeCommand(nodeNames,JsonSerializer::kRemove).dump(2);

    setText(commnadText);
}

void RemoveCommand::undo(){
    using namespace Sww::Core;
    try{
        SwwJson j = SwwJson::parse(_undoJson);
        JsonDeserializer jd;
        Imath::Box2f dummy;
        jd.deserialize(j,_scene->node(),dummy);

        _scene->setup();
    }
    catch(...){
        SWW_ERR("undo failed.");
    }
}

void RemoveCommand::redo(){
    using namespace Sww::Core;
    if(_first){
        _first = false;
        return;
    }
    try{
        SwwJson j = SwwJson::parse(_redoJson);
        JsonDeserializer jd;
        jd.setFlags(JsonDeserializer::kJustRunCommand);
        jd.runCommand(j,_scene->node());

        _scene->setup();
    }
    catch(...){
        SWW_ERR("redo failed.");
    }
}

MoveCommand::MoveCommand(const std::vector<Sww::Core::Node *> &targetBeforeMove, NodeGraphScene *scene):_scene(scene),_first(true){
    using namespace Sww::Core;

    JsonSerializer j;
    j.resetFlags();
    j.setFlags(JsonSerializer::asOverWriteMode | JsonSerializer::usePos);
    _undoJson = j.serialize(targetBeforeMove);
}

void MoveCommand::setAfterPos(const std::vector<Sww::Core::Node *> &targetAfterMove){
    using namespace Sww::Core;
    JsonSerializer j;
    j.resetFlags();
    j.setFlags(JsonSerializer::asOverWriteMode | JsonSerializer::usePos);
    _redoJson = j.serialize(targetAfterMove);

    QString commandText = "move";
    for(auto n:targetAfterMove){
        QString posStr = " [" + QString::number(n->posInGraph().x) + "," + QString::number(n->posInGraph().y) + "]";
        commandText += QString(" ") + n->getName().c_str() + posStr;
    }
    setText(commandText);
}

void MoveCommand::undo(){
    using namespace Sww::Core;
    try{
        SwwJson j = SwwJson::parse(_undoJson);
        JsonDeserializer jd;
        Imath::Box2f dummy;
        jd.deserialize(j,_scene->node(),dummy);
        _scene->update();
    }
    catch(...){
        SWW_ERR("undo failed.");
    }
}

void MoveCommand::redo(){

    Q_ASSERT(!_redoJson.empty());

    using namespace Sww::Core;
    if(_first){
        _first = false;
        return;
    }
    try{
        SwwJson j = SwwJson::parse(_redoJson);
        JsonDeserializer jd;
        Imath::Box2f dummy;
        jd.deserialize(j,_scene->node(),dummy);
        _scene->update();
    }
    catch(...){
        SWW_ERR("redo failed.");
    }

}

SelectCommand::SelectCommand(const std::vector<std::string> &targetBeforeSelect, NodeGraphScene *scene):_scene(scene),_first(true){
    using namespace Sww::Core;

    JsonSerializer js;
    if(targetBeforeSelect.empty()){
        _undoJson = js.makeCommand(targetBeforeSelect,JsonSerializer::kDeselect).dump(2);
    }else{
        _undoJson = js.makeCommand(targetBeforeSelect,JsonSerializer::kSelect).dump(2);
    }

}

void SelectCommand::setAfterSelect(const std::vector<std::string> &targetAfterSelect){
    using namespace Sww::Core;

    JsonSerializer js;
    if(targetAfterSelect.empty()){
        _redoJson = js.makeCommand(targetAfterSelect,JsonSerializer::kDeselect).dump(2);
    }else{
        _redoJson = js.makeCommand(targetAfterSelect,JsonSerializer::kSelect).dump(2);
    }

    QString commnadText = "select";
    for(auto name:targetAfterSelect){
        commnadText += QString(" ") + name.c_str();
    }
    setText(commnadText);

}

void SelectCommand::undo(){
    using namespace Sww::Core;
    try{
        SwwJson j = SwwJson::parse(_undoJson);
        parse(j);
    }
    catch(...){
        SWW_ERR("undo failed.");
    }
}

void SelectCommand::redo(){

    Q_ASSERT(!_redoJson.empty());

    using namespace Sww::Core;
    if(_first){
        _first = false;
        return;
    }
    try{
        SwwJson j = SwwJson::parse(_redoJson);
        parse(j);
    }
    catch(...){
        SWW_ERR("redo failed.");
    }

}

void SelectCommand::parse(SwwJson& j){
    _scene->clearSelection();
    SwwJson jCmd = j["%cmd"];
    SwwJson jSel = jCmd["sel"];
    std::vector<std::string> nodeNames;
    if(jSel.is_null()){
        jSel = jCmd["desel"];
        Q_ASSERT(!jSel.is_null());
    }else{
        nodeNames = jSel;
    }
    _scene->selectNodeItems(nodeNames);
}

ChangeSceneCommand::ChangeSceneCommand(const std::string &beforePath, const std::string &afterPath, NodeGraphWidget *ngw):
_beforePath(beforePath),_afterPath(afterPath),_ngw(ngw)
{

}

void ChangeSceneCommand::undo(){
    _ngw->setScene(_beforePath,false);
}

void ChangeSceneCommand::redo(){
    if(_first){
        _first = false;
        return;
    }
    _ngw->setScene(_afterPath,false);
}


}//ns
