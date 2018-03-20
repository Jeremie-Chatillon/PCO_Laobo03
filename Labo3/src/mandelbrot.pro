QT += widgets

HEADERS       = mandelbrotwidget.h \
                renderthread.h \
    delegationthread.h
SOURCES       = main.cpp \
                mandelbrotwidget.cpp \
                renderthread.cpp \
    delegationthread.cpp

unix:!mac:!vxworks:!integrity:LIBS += -lm

# install
target.path = $$[QT_INSTALL_EXAMPLES]/threads/mandelbrot
INSTALLS += target
