TEMPLATE = \
	lib

CONFIG += \
	plugin

TARGET = \
	import_image

INCLUDEPATH += \
	../vlog

HEADERS += \
	plugin.h \
	plugin_data.h \
	../vlog/interface/import_plugin.h \
	../vlog/interface/import_data.h

SOURCES += \
	plugin.cpp \
	plugin_data.cpp \
	../vlog/interface/import_plugin.cpp \
	../vlog/interface/import_data.cpp
