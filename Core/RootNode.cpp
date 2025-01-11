#include "RootNode.h"
#include "Core/PlugType.h"
#include "Core/NodeGraph.h"

//#include "Core/XmlSerializer.h"
//#include "Core/XmlDeserializer.h"

#include "JsonSerializer.h"
#include "JsonDeserializer.h"
#include <QFileInfo>

SWW_CORE_NAMESPACE_BEGIN

RootNode::RootNode():_currentFrame(0),_startFrame(0),_endFrame(100)
{
    initialize();
}

bool RootNode::initialize(){

    //createChildGraph();
    //setName("$root");

    setName("$root");

    _framePlug = new Sww::Core::IntPlug("frame",0);
    addPlug(_framePlug);

    //setGlobalFramePlug(_framePlug);
    Node::_rootNode = this;

    setInternal(true);

    return true;
}

void RootNode::setGlobalFrame(int current)
{
    _currentFrame = current;
    _framePlug->setInputValue(current);
}


void RootNode::adaptFrameRange(bool adaptToStart)
{
    if(_startFrame > _endFrame)
    {
        if(adaptToStart)
        {
            _endFrame = _startFrame;
        }else
        {
            _startFrame =_endFrame;
        }
    }

    if(_currentFrame <  _startFrame)
    {
        //_startFrame = _currentFrame;
        setGlobalFrame( _startFrame);
    }
    if(_currentFrame > _endFrame)
    {
        //_endFrame = _currentFrame;
        setGlobalFrame(_endFrame);
    }
}

bool RootNode::run(QObject* reciever, InfoToSubThread info)
{
    //SwwLog::setNoGuiMode();

    *(info.stopped) = false;
    info.end += 1;

    for(int i = info.start;i<info.end;i++)
    {
        //info.nodeGraph->setGlobalTime(i);
        info.node->setGlobalFrame(i);
        bool result = info.plug->pull();
        if(!result){
            SWW_DEBUG("render failed. " << info.plug->getDebugName());
            break;
        }

        int progress = (float(i - info.start) / float(info.end -info.start)) * 100;


        if(*(info.stopped)){
            SWW_INF("render has canceled.");
            break;
        }

        QMetaObject::invokeMethod(reciever,"setCurrentStatus",
                                  Q_ARG(int,info.node->currentFrame()),
                                  Q_ARG(int,progress));
    }//for loop i

    //info.nodeGraph->setGlobalTime(info.current);//timesliderとの整合性をとるため、カレントをもどす
    //info.nodeGraph->runFinished();
     info.node->setGlobalFrame(info.current);
     info.node->runFinished();

    delete info.stopped;

    //SwwLog::setGuiMode();

    //Q_ASSERT(0);
    return true;
}

bool RootNode::run()
{
    std::vector<PlugBase*> runnerPlugs;

    QMap<std::string, Node::Ptr>::iterator ite =  _children.begin();
    while (ite != _children.end()) {
        Node::Ptr& n = ite.value();
        if(n->isRunner())
        {
            std::copy(n->getRunnerPlugs().begin(),n->getRunnerPlugs().end(),std::back_inserter(runnerPlugs));//複製
          }
        ++ite;
    }

    for(std::vector<PlugBase*>::iterator it = runnerPlugs.begin();
        it != runnerPlugs.end();
        ++it)
    {
        PlugBase* p = *it;
        SWW_INF("run : " << p->getDebugName() );
    }

    for(int i = _startFrame;i<=_endFrame;i++)
    {
        setGlobalFrame(i);
        for( std::vector<PlugBase*>::iterator it = runnerPlugs.begin();
             it != runnerPlugs.end();
             ++it)
        {
            PlugBase* p = *it;
            bool result =  p->pull();

            if(!result)
            {
                SWW_ERR("running process failed. exit. in " << p->getDebugName());
                return result;
            }
        }

        int progress = 0;
        if(_startFrame == _endFrame){
            progress = 100;
        }
        progress = (float(i - _startFrame) / float(_endFrame - _startFrame)) * 100;

        SWW_INF("progress:" << progress << "%");
    }

    //Q_ASSERT(0);
    return true;
}

bool RootNode::save(const std::string& filename){
  QFileInfo info(filename.c_str());

/*  if(info.suffix() == "xml"){
      return saveAsXml(filename);
  }
  else */if(info.suffix() == "json"){
      return saveAsJson(filename);
  }
  else{
      SWW_ERR("invalid suffix. " << filename);
      return false;
  }
}

//bool RootNode::saveAsXml(const std::string& filename){
//    XmlSerializer s;
//    return s.save(filename.c_str(),this);
//}

bool RootNode::saveAsJson(const std::string& filename){
    JsonSerializer s;
    std::string str = s.serializeRoot(this);
    QFile file(filename.c_str());
    if (!file.open(QFile::WriteOnly| QFile::Truncate))return false;
    QTextStream out(&file);
    out << str.c_str();
    return true;
}

bool RootNode::load(const std::string& filename){
    QFileInfo info(filename.c_str());

   /* if(info.suffix() == "xml"){
        return loadFromXml(filename);
    }
    else*/ if(info.suffix() == "json"){
        return loadFromJson(filename);
    }
    else{
        SWW_ERR("invalid suffix. " << filename);
        return false;
    }
}

//bool RootNode::loadFromXml(const std::string& filename){
//    XmlDeserializer d;
//    return d.parse(filename.c_str(),this);
//}

bool RootNode::loadFromJson(const std::string& filename){
     JsonDeserializer j;
     return j.deserialize(filename,this);
}

void RootNode::clearGraph(){
    Node::clearGraph();
}

SWW_CORE_NAMESPACE_END
