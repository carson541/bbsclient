TEMPLATE	= app
CONFIG		= qt warn_on debug
HEADERS		= termwidget.h
SOURCES		= main.cpp termwidget.cpp
INCLUDEPATH	+= $(QPEDIR)/include
DEPENDPATH	+= $(QPEDIR)/include
LIBS            += -lqpe -L. -lbbs -lpthread
INTERFACES	=
TARGET		= bbsclient
