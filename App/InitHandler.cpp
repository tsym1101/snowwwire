#include "InitHandler.h"
#include "OptionParser.h"

#include <QFileInfo>
#include <QDesktopWidget>
#include <QScreen>
#include "Core/SwwAssert.h"
#include "Core/NodeGraph.h"
#include "Log/Log.h"
#include "Core/RootNode.h"
#include "OpenGL/GLGlobal.h"
#include "Gui/GuiGlobal.h"
#include "Gui/NodeGraphScene.h"
#include "Gui/MainWindow.h"

InitHandler::InitHandler():
    _nogui(false),_fileName(QString()),_start(1),_end(10),_startFrameSpecified(false),_endFrameSpecified(false),
    _mw(0)
{
}

InitHandler::~InitHandler(){
    if(_mw){
        delete _mw;
    }
}

void InitHandler::parseArg(int argc, char *argv[])
{
    if(argc == 1) return;

    bool stat = false;  //引数の有無
    bool dummy;

    QFileInfo info(argv[argc -1]);

    if(!info.exists())
    {
        SWW_ERR("no file assigned.");
        exit(2);
    }

    _fileName = info.absoluteFilePath();
    SWW_DEBUG(_fileName.toStdString());

    OptionParser op(argc,argv,dummy);

    QVariant value =  op.GetValue(stat,"nogui");
    if(stat)
    {
        _nogui = value.toInt();
    }

    value =  op.GetValue(stat,"s");
    if(stat)
    {
        _start = value.toInt();
        _startFrameSpecified = true;
    }

    value =  op.GetValue(stat,"e");
    if(stat)
    {
        _end = value.toInt();
        _endFrameSpecified = true;
    }
}

int InitHandler::start(Sww::Core::RootNode *rootNode, QApplication& a)
{
    if(_nogui)
    {
        SWW_INF("start no GUI mode.");
        if(!rootNode->load(_fileName.toStdString()))
        {
            SWW_ERR("load failed. " << _fileName.toStdString());
            return 50;
        }

        SWW_INF("range " << _start << " " << _end);

        if(_startFrameSpecified)
        {
            //ngs->node()->startFrame(_start);

            rootNode->startFrame(_start);

        }
        if(_endFrameSpecified)
        {
            //ngs->nodeGraph()->endFrame(_end);


            rootNode->endFrame(_end);
        }

        if(!rootNode->run())
        {
            SWW_ERR("failed operation. " << _fileName.toStdString());
            return 99;
        }

        SWW_INF("complete operation. " << _fileName.toStdString());

        return 0;

    }
    else
    {

        Sww::Log::setGuiMode();

        //_mw = new MainWindow(rootNode);
        SwwGui::GuiGlobal::init(rootNode);
        _mw = SwwGui::GuiGlobal::getMainWindow();
        SwwOpenGL::GLGlobal::setMainWindow(_mw);

        QScreen *screen = QGuiApplication::primaryScreen();
        qreal dpiX = screen->logicalDotsPerInchX();
        qreal dpiY = screen->logicalDotsPerInchY();
        SwwGui::GuiGlobal::setCurrentLogicalDPI(dpiX,dpiY);

        if(!_fileName.isNull())
        {
            _mw->open(_fileName);
        }

        //#define SWW_SECOND_SCREEN
#ifdef SWW_SECOND_SCREEN
        QRect screenres = QApplication::desktop()->screenGeometry(1/*screenNumber*/);
        _mw->move(QPoint(screenres.x(), screenres.y()));
        _mw->resize(screenres.width(), screenres.height());
#endif

        _mw->showMaximized();

        return a.exec();
    }
}

