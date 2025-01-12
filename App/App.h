#ifndef SWWAPP_H
#define SWWAPP_H

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtGui/QApplication>
#else
#include <QtWidgets/QApplication>
#endif

#include <vector>

namespace Sww::Core {
class PluginInterface;
}

class App
{
public:
    App();
    int run(QApplication &a,int argc, char *argv[]);
private:
    bool loadPlugin();
    bool unloadPlugin();
    std::vector<Sww::Core::PluginInterface*> _interfaces;
    bool loadMod();
};

#endif // SWWAPP_H
