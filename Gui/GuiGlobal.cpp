#include "GuiGlobal.h"
#include <qglobal.h>
//#include <QMainWindow>
#include "MainWindow.h";
#include "Core/RootNode.h"
//#include "DsGui/NodeItem.h"

namespace SwwGui {

#define SWW_PIXELS_FOR_DPI_DEFAULT 96.

MainWindow* GuiGlobal::_mw = 0;
int GuiGlobal::_dpiX = 0;
int GuiGlobal::_dpiY = 0;
//QUndoStack* GuiGlobal::_commandStack = 0;
//void GuiGlobal::setMainWindow(QMainWindow *mw){
//    _mw = mw;
//}

MainWindow *GuiGlobal::getMainWindow(){
    Q_ASSERT(_mw);
    return _mw;
}

void GuiGlobal::currentSceneModified(){
    _mw->setWindowModified(true);
}

void GuiGlobal::init(Sww::Core::RootNode *rootNode){
    _mw = new MainWindow(rootNode);
    //NodeItem::initFont();
}

void GuiGlobal::setCurrentLogicalDPI(double dpiX, double dpiY){
    _dpiX = dpiX;
    _dpiY = dpiY;
}

double
GuiGlobal::getLogicalDPIXRATIO()
{
    return _dpiX / SWW_PIXELS_FOR_DPI_DEFAULT;
}

double
GuiGlobal::getLogicalDPIYRATIO()
{
    return _dpiY / SWW_PIXELS_FOR_DPI_DEFAULT;
}

}
