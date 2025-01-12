TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS +=\
    Log\
    Util\
    Core\
    Geom\
    Knob\
#    Node\
    OpenGL\
    Gui\
#    plugins\
    App

Core.depends = Log
Util.depends = Log
Knob.depends =  Core Geom
#Node.depends =  Core Geom
OpenGL.depends = Core Geom
Gui.depends = Util Core Geom Knob OpenGL
#plugins.depends = Core Geom Knob Gui Node OpenGL
App.depends = Core Geom Knob Gui OpenGL #plugins

QMAKE_CXXFLAGS += /FS

HEADERS += \
    Ns.h
