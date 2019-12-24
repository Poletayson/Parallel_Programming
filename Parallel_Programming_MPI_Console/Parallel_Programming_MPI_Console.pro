#-------------------------------------------------
#
# Project created by QtCreator 2018-09-11T16:00:52
#
#-------------------------------------------------

QT       += core

INCLUDEPATH+="C:\Program Files (x86)\Microsoft SDKs\MPI\Include"

LIBS += "C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x86\msmpi.lib"

#INCLUDEPATH+="C:\Program Files (x86)\MPICH2\include"

#LIBS += "C:\Program Files (x86)\MPICH2\lib\mpi.lib"
#"C:\Program Files (x86)\OpenMPI_1.6.2\lib\libmpi.lib" \
#        "C:\Program Files (x86)\OpenMPI_1.6.2\lib\libmpi_cxx.lib" \
#        "C:\Program Files (x86)\OpenMPI_1.6.2\lib\libopen-pal.lib" \
#        "C:\Program Files (x86)\OpenMPI_1.6.2\lib\libopen-rte.lib"

QMAKE_LIBS+=-static -lgomp -lpthread

QMAKE_CXXFLAGS+=-msse3 -fopenmp

QMAKE_LFLAGS += -fopenmp


#QMAKE_CXX = mpicxx
#QMAKE_CXX_RELEASE = $$QMAKE_CXX
#QMAKE_CXX_DEBUG = $$QMAKE_CXX
#QMAKE_LINK = $$QMAKE_CXX
#QMAKE_CC = mpicc


#QMAKE_CXXFLAGS+=-U_WIN32

TARGET = Parallel_Programming_MPI_Console
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11 console

SOURCES += \
        main.cpp \
    graphic.cpp

HEADERS += \
    graphic.h


FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
