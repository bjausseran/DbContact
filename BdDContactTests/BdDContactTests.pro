QT += testlib
QT -= gui
QT += sql concurrent

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_contacttest.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../BdDContactLibrary/release/ -lBdDContactLibrary
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../BdDContactLibrary/debug/ -lBdDContactLibrary
else:unix: LIBS += -L$$OUT_PWD/../BdDContactLibrary/ -lBdDContactLibrary

INCLUDEPATH += $$PWD/../BdDContactLibrary
DEPENDPATH += $$PWD/../BdDContactLibrary

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../BdDContactLibrary/release/ -lBdDContactLibrary
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../BdDContactLibrary/debug/ -lBdDContactLibrary
else:unix: LIBS += -L$$OUT_PWD/../BdDContactLibrary/ -lBdDContactLibrary

INCLUDEPATH += $$PWD/../BdDContactLibrary
DEPENDPATH += $$PWD/../BdDContactLibrary

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../BdDContactLibrary/release/ -lBdDContactLibrary
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../BdDContactLibrary/debug/ -lBdDContactLibrary
else:unix: LIBS += -L$$OUT_PWD/../BdDContactLibrary/ -lBdDContactLibrary

INCLUDEPATH += $$PWD/../BdDContactLibrary
DEPENDPATH += $$PWD/../BdDContactLibrary
