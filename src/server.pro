include(./Kingfisher.pri)
include(./server/server.pri)
include(./network/network.pri)

DEPENDPATH += .
INCLUDEPATH += .
OBJECTS_DIR = ../obj

HEADERS += $$NETWORK_HDRS
SOURCES += $$NETWORK_SRCS $$SERVER_SRCS
