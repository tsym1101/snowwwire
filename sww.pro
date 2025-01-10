TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS +=\
    Log\
    Util\
    Core\
    Geom\
#    SwwKnob\
#    SwwNode\
#    SwwOpenGL\
#    SwwGui\
#    plugins\
#    SwwApp

Core.depends = Log
Util.depends = Log
#SwwKnob.depends = SwwUtil SwwCore SwwGeom
#SwwNode.depends = SwwUtil SwwCore SwwGeom
#SwwOpenGL.depends = SwwUtil SwwCore SwwGeom SwwNode
#SwwGui.depends = SwwCore SwwGeom SwwKnob SwwOpenGL
#plugins.depends = SwwUtil SwwCore SwwGeom SwwKnob SwwGui SwwNode SwwOpenGL
#SwwApp.depends = SwwUtil SwwCore SwwGeom SwwKnob SwwGui SwwNode SwwOpenGL plugins

QMAKE_CXXFLAGS += /FS

HEADERS += \
    SwwNs.h
