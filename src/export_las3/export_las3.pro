TEMPLATE = \
	lib

CONFIG += \
	plugin

TARGET = \
	export_las3

INCLUDEPATH += \
	../vlog

HEADERS += \
	plugin.h \
	plugin_data.h \
	../vlog/interface/export_plugin.h \
	../vlog/interface/export_data.h

SOURCES += \
	plugin.cpp \
	plugin_data.cpp \
	../vlog/interface/export_plugin.cpp \
	../vlog/interface/export_data.cpp

RESOURCES += \
	resources.qrc
