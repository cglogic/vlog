include(./qtpropertybrowser/src/qtpropertybrowser.pri)

QT += \
	core \
	gui \
	xml

TARGET = \
	vlog

win32 {
    QMAKE_CXXFLAGS += \
	/openmp

    RC_FILE +=\
	win32.rc
}

unix {
    QMAKE_CXXFLAGS += \
        -fopenmp \
        -std=c++11

    LIBS += \
        -lgomp
}

CODECFORTR = \
	UTF-8

CODECFORSRC = \
	UTF-8

TRANSLATIONS += \
	russian.ts \
	ukrainian.ts

RESOURCES += \
	resources.qrc

#INCLUDEPATH += \
#	opencl/include

#LIBS += \
#	-L../../src/vlog/opencl/lib -lopencl

OTHER_FILES += \
	win32.rc \
	../../build/application.log \
	files/pixmap_waving.cl

SOURCES += \
	system/log.cpp \
	system/dialog.cpp \
	system/config_section.cpp \
	system/config.cpp \
	interface/import_plugin.cpp \
	interface/import_data.cpp \
	interface/export_plugin.cpp \
	interface/export_data.cpp \
	main.cpp \
	application.cpp \
	window.cpp \
	object.cpp \
	manager.cpp \
	preferences.cpp \
	project.cpp \
	project_new.cpp \
	view.cpp \
	tree.cpp \
	tree_item_delegate.cpp \
	scene/scene.cpp \
	scene/range.cpp \
	scene/point.cpp \
	scene/pixmap.cpp \
	scene/parameter.cpp \
	scene/curve.cpp \
	props/prop_manager.cpp \
	props/prop_item.cpp \
	scene/alglib/statistics.cpp \
	scene/alglib/specialfunctions.cpp \
	scene/alglib/solvers.cpp \
	scene/alglib/optimization.cpp \
	scene/alglib/linalg.cpp \
	scene/alglib/interpolation.cpp \
	scene/alglib/integration.cpp \
	scene/alglib/fasttransforms.cpp \
	scene/alglib/diffequations.cpp \
	scene/alglib/dataanalysis.cpp \
	scene/alglib/ap.cpp \
	scene/alglib/alglibmisc.cpp \
	scene/alglib/alglibinternal.cpp \
	about.cpp \
	preview/preview.cpp \
	focl/focl.cpp

HEADERS += \
	system/log.h \
	system/dialog.h \
	system/config_section.h \
	system/config.h \
	interface/import_plugin.h \
	interface/import_data.h \
	interface/export_plugin.h \
	interface/export_data.h \
	application.h \
	window.h \
	object.h \
	manager.h \
	preferences.h \
	project.h \
	project_new.h \
	view.h \
	tree.h \
	tree_item_delegate.h \
	scene/scene.h \
	scene/range.h \
	scene/point.h \
	scene/pixmap.h \
	scene/parameter.h \
	scene/curve.h \
	props/prop_item.h \
	props/prop_manager.h \
	scene/alglib/stdafx.h \
	scene/alglib/statistics.h \
	scene/alglib/specialfunctions.h \
	scene/alglib/solvers.h \
	scene/alglib/optimization.h \
	scene/alglib/linalg.h \
	scene/alglib/interpolation.h \
	scene/alglib/integration.h \
	scene/alglib/fasttransforms.h \
	scene/alglib/diffequations.h \
	scene/alglib/dataanalysis.h \
	scene/alglib/ap.h \
	scene/alglib/alglibmisc.h \
	scene/alglib/alglibinternal.h \
	about.h \
	preview/preview.h \
	focl/focl.h \
	config.h
