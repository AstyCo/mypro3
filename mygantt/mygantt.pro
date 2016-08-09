#-------------------------------------------------
#
# Project created by QtCreator 2016-07-31T20:10:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mygantt
TEMPLATE = app


# пути к зависимым библиотекам
unix {
  DEP_PATH_LIBS = /usr/lib64
  DEP_PATH_HEADERS = /usr/include
} else:win32 {
  DEP_PATH_LIBS = C:/lib64
  DEP_PATH_HEADERS = C:/include
}

LIBS += -L$${DEP_PATH_LIBS}

CONFIG(release, debug|release) {
  LIBS += -lextensions
} else:CONFIG(debug, debug|release) {
  LIBS += -lextensionsd
  unix {

  } else:win32 {

  }
}


INCLUDEPATH += "$${DEP_PATH_HEADERS}/extensions"


SOURCES += main.cpp\
    mygantt_header.cpp \
    mygantt_scene.cpp \
    mygantt_widget.cpp \
    qgraphicsrectitemwithtext.cpp \
    mygantt_item.cpp \
    mygantt_treemodel.cpp \
    mygantt_infoleaf.cpp \
    mygantt_infonode.cpp \
    mygantt_infoitem.cpp \
    mygantt_treeview.cpp \
    mygantt_graphicsview.cpp \
    fromballisticextension.cpp \
    mygantt_slider.cpp \
    mygantt_headerview.cpp \
    mygantt_treedelegate.cpp

HEADERS  += \
    mygantt_globalvalues.h \
    mygantt_header.h \
    mygantt_scene.h \
    mygantt_widget.h \
    qgraphicsrectitemwithtext.h \
    mygantt_item.h \
    mygantt_treemodel.h \
    mygantt_infonode.h \
    mygantt_infoleaf.h \
    mygantt_infoitem.h \
    mygantt_graphicsview.h \
    mygantt_treeview.h \
    fromballisticextension.h \
    mygantt_slider.h \
    mygantt_headerview.h \
    mygantt_treedelegate.h

FORMS    += ganttwidget.ui
