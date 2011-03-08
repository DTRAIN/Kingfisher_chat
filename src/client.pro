include(./Kingfisher.pri)
include(./client/client.pri)

DEPENDPATH += .
INCLUDEPATH += .
OBJECTS_DIR = ../obj/client

HEADERS += $$CLIENT_HDRS
SOURCES += $$CLIENT_SRCS
FORMS   += $$CLIENT_FORMS
