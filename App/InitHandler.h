#ifndef INITHANDLER_H
#define INITHANDLER_H

#include <QString>
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtGui/QApplication>
#else
#include <QtWidgets/QApplication>
#endif

namespace SwwGui{
class NodeGraphScene;
}

namespace Sww::Core{
class RootNode;
}

class MainWindow;

class InitHandler
{
public:
    InitHandler();
    ~InitHandler();
    void parseArg(int argc, char *argv[]);
    int start(Sww::Core::RootNode* rootNode, QApplication &a);

private:
    bool _nogui;
    int _start,_end;
    QString _fileName;
    bool _startFrameSpecified,_endFrameSpecified;
    MainWindow* _mw;
};

#endif // INITHANDLER_H
