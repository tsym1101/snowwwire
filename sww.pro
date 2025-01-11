TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS +=\
    Log\
    Util\
    Core\
    Geom\
    Knob\
    Node\
    OpenGL\
#    Gui\
#    plugins\
#    App

Core.depends = Log
Util.depends = Log
Knob.depends = Util Core Geom
Node.depends = Util Core Geom
OpenGL.depends = Util Core Geom Node
Gui.depends = Core Geom Knob OpenGL
#plugins.depends = Util Core Geom Knob Gui Node OpenGL
App.depends = Util Core Geom Knob Gui Node OpenGL #plugins

QMAKE_CXXFLAGS += /FS

HEADERS += \
    Ns.h
