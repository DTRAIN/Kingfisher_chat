include(./Kingfisher.pri)
include(./client/client.pri)
include(./network/network.pri)

DEPENDPATH += .
INCLUDEPATH += .
OBJECTS_DIR = ../obj

HEADERS += $$CLIENT_HDRS $$NETWORK_HDRS
SOURCES += $$CLIENT_SRCS $$NETWORK_SRCS
FORMS   += $$CLIENT_FORMS
