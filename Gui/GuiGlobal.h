#ifndef SWWGUIGLOBAL_H
#define SWWGUIGLOBAL_H

#include "Gui/GuiAPI.h"

class MainWindow;
class QObject;

namespace Sww::Core {
class RootNode;
}

namespace SwwGui {

class  SWWGUISHARED_EXPORT GuiGlobal
{
public:
    static void init(Sww::Core::RootNode* rootNode);
    static MainWindow* getMainWindow();
    static void currentSceneModified();
    static void setCurrentLogicalDPI(double dpiX,double dpiY);
    static double getLogicalDPIXRATIO();
    static double getLogicalDPIYRATIO();

private:
    static MainWindow* _mw;
    static int _dpiX;
    static int _dpiY;

};

}

#endif // SWWGUIGLOBAL_H
