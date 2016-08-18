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
  LIBS += -lextensions -lganttmodel
} else:CONFIG(debug, debug|release) {
  LIBS += -lextensionsd -lganttmodeld
  unix {

  } else:win32 {

  }
}


INCLUDEPATH += "$${DEP_PATH_HEADERS}/extensions"
INCLUDEPATH += "$${DEP_PATH_HEADERS}/ganttmodel"



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
    mygantt_headerview.cpp \
    mygantt_treedelegate.cpp \
    mygantt_intervalslider.cpp \
    mygantt_slider.cpp \
    ganttplayersettings.cpp \
    mygantt_playercontrol.cpp \
    playercontrolbutton.cpp

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
    mygantt_headerview.h \
    mygantt_treedelegate.h \
    mygantt_intervalslider.h \
    mygantt_slider.h \
    ganttplayersettings.h \
    mygantt_playercontrol.h \
    playercontrolbutton.h

FORMS    += ganttwidget.ui \
    ganttplayersettings.ui \
    mygantt_playercontrol.ui

DISTFILES += \
    Buttons 64x64/back_off_64.png \
    Buttons 64x64/back_on_64.png \
    Buttons 64x64/back_step_off_64.png \
    Buttons 64x64/back_step_on_64.png \
    Buttons 64x64/begin_off_64.png \
    Buttons 64x64/begin_on_64.png \
    Buttons 64x64/end_off_64.png \
    Buttons 64x64/end_on_64.png \
    Buttons 64x64/forward_off_64.png \
    Buttons 64x64/forward_on_64.png \
    Buttons 64x64/forward_step_off_64.png \
    Buttons 64x64/forward_step_on_64.png \
    Buttons 64x64/next_off_64.png \
    Buttons 64x64/next_on_64.png \
    Buttons 64x64/pause_off_64.png \
    Buttons 64x64/pause_on_64.png \
    Buttons 64x64/prev_off_64.png \
    Buttons 64x64/prev_on_64.png \
    Buttons 64x64/rec_off_64.png \
    Buttons 64x64/rec_on_64.png \
    Buttons 64x64/reg_speed_on_64.png \
    Buttons 64x64/reg_step_on_64.png \
    Buttons 64x64/stop_off_64.png \
    Buttons 64x64/stop_on_64.png

RESOURCES += \
    images.qrc
