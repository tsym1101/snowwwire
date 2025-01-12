#include "App/App.h"

#include "App/Entry.h"
#include "Gui/NodeGraphWidget.h"
#include "Gui/NodeGraphScene.h"
#include "Gui/MainWindow.h"
#include "OpenGL/GLGlobal.h"
#include "OpenGL/GLUtility.h"
#include "InitHandler.h"

#include "Core/SwwAssert.h"
#include <QDir>
#include <QPluginLoader>

#include "Core/SwwPluginInterface.h"
#include "Core/VectorHelper.h"

#include "Knob/KnobEntry.h"

#include "Log/ConsoleWidget.h"
#include "Log/Log.h"

#include "Core/Node.h"
#include "Core/RootNode.h"
#include "Core/PlugRegister.h"
#include "Core/NodeRegister.h"

//#include "Util/IOUtility.h"
#include "Gui/GuiGlobal.h"

#include "Knob/CommandStack.h"

App::App()
{
}


int App::run(QApplication& a, int argc, char *argv[]){



    bool valid;

    //0
    Sww::Log::init();

//    //0.5
//    valid = Sww::Util::IOUtility::initOiio(); // for oiio bug on win7!!!!
//    Q_ASSERT(valid);

    //1
    valid = SwwOpenGL::GLGlobal::init();
    Q_ASSERT(valid);

    //1.5
    Sww::Core::PlugRegister::init();

    //2
    Sww::Entry::NodeEntry();//ng.initialize()より先に呼ぶ
    valid = loadPlugin();
    Q_ASSERT(valid);

    loadMod();

    //3
    SwwKnob::SwwKnobEntry();

    SwwKnob::CommandStack::init();

    Sww::Core::RootNode* rootNode = new Sww::Core::RootNode();

    //parse arg
    InitHandler handler;
    handler.parseArg(argc,argv);
    int result = handler.start(rootNode,a);

    Sww::Log::deinit();
    SwwKnob::CommandStack::deinit();

    SwwOpenGL::GLGlobal::makeCurrent();
    valid = unloadPlugin();
    Q_ASSERT(valid);
    Q_ASSERT_X(SwwOpenGL::GLUtility::getCurrentContext(),
               "after unload plugin.",
               "no context is current.");

    delete rootNode;

    Q_ASSERT_X(SwwOpenGL::GLUtility::getCurrentContext(),
               "after delete graph.",
               "no context is current.");

    SwwOpenGL::GLGlobal::doneCurrent();

    return result;
}

bool App::loadPlugin()
{
    SWW_DEBUG("start loading plugins...");

    QDir pluginDir(QApplication::applicationDirPath());

    if(!pluginDir.cd("plugins")){
        SWW_ERR("plugins dir not found.");
        return false;
    }

    Q_FOREACH(QString fileName,pluginDir.entryList(QDir::Files)){
        QString extension;
        extension = "dll";
        QStringList splitted = fileName.split(".");
        Q_ASSERT_X(splitted.size() > 1,"load plugin.","unexpected plugin file name.");
        if(fileName.split(".")[1] != extension)continue;
        //SWW_DEBUG_VALUE(fileName.toStdString());
        //QString abPath = pluginDir.absoluteFilePath(fileName);
        QPluginLoader loader(pluginDir.absoluteFilePath(fileName));

        //        SWW_DEBUG_VALUE(loader.load());
        //        SWW_DEBUG_VALUE(loader.isLoaded());
        //        SWW_DEBUG(fileName.toStdString() << " " <<  loader.errorString().toStdString());

        QObject* instance = loader.instance();

        Sww::Core::PluginInterface *p_interface = qobject_cast<Sww::Core::PluginInterface*>(instance);
        if(p_interface)
        {
            if(!p_interface->entry()){
                SWW_ERR("plugin entry failed. " << fileName.toStdString());
            }else{
                _interfaces.push_back(p_interface);
            }
        }
        else{
            SWW_ERR("load plugin failed." << fileName.toStdString());
        }
    }

    return true;
}

bool App::unloadPlugin(){
    SWW_VECTOR_FOREACH(Sww::Core::PluginInterface*,_interfaces){
        Sww::Core::PluginInterface* ptr = *i;

        if(!ptr->cleanup()){
            SWW_WARN("cleanup plugin failed. : " << ptr->metaInfo().name());
        }
    }

    return true;
}

bool App::loadMod(){
    QString  dir = QFileInfo(QCoreApplication::applicationFilePath()).dir().path();
    QString modDirPath = dir + QDir::separator() + "mod";

    QDir modDir(modDirPath);
    QStringList filter;
    filter += "*.json";

    QStringList entryList = modDir.entryList(filter,QDir::Files);
    if(entryList.isEmpty()){
        SWW_DEBUG("mod file is not exist.");
        return true;
    }
    else{
        //SWW_DEBUG("mod[0] -> " << entryList[0].toStdString());
    }

    Q_FOREACH(QString str,entryList){
        std::string modName = str.split(".").first().toStdString();
        QString modFilePath = modDirPath + QDir::separator() + str;
        QFile file(modFilePath);

        if (!file.open(QIODevice::ReadOnly))//読込のみでオープンできたかチェック
        {
            continue;
        }

        QTextStream in(&file);
        std::string jsonStr = in.readAll().toStdString();

        Sww::Core::NodeRegister::registerMod(modName,jsonStr,"Mod");
    }

    QStringList entryListDir = modDir.entryList(QStringList(),QDir::Dirs);
    entryListDir.removeOne(".");
    entryListDir.removeOne("..");
    Q_FOREACH(QString dirTmp,entryListDir){
        QString modCategory = dirTmp;
        QString modDirPathChild = modDirPath  + QDir::separator() + modCategory;
        QDir modDirChild(modDirPathChild);
        QStringList entryListChild = modDirChild.entryList(filter,QDir::Files);
         Q_FOREACH(QString str,entryListChild){

             std::string modName = str.split(".").first().toStdString();
             QString modFilePath = modDirPathChild + QDir::separator() + str;
             QFile file(modFilePath);
             if (!file.open(QIODevice::ReadOnly))//読込のみでオープンできたかチェック
             {
                 continue;
             }

             QTextStream in(&file);
             std::string jsonStr = in.readAll().toStdString();
             Sww::Core::NodeRegister::registerMod(modName,jsonStr,modCategory.toStdString());

         }
    }

    return true;
}
