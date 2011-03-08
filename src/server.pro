include(./Kingfisher.pri)
include(./server/server.pri)

DEPENDPATH += .
INCLUDEPATH += .
OBJECTS_DIR = ../obj/server

HEADERS += $$SERVER_HDRS
SOURCES += $$SERVER_SOURCES
