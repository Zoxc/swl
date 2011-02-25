default: all

include Makefile.autoconf
-include Makefile.conf

CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
AR = $(CROSS_COMPILE)ar

BUILD_DIRS += .
CFLAGS += -pipe -Wall
CXXFLAGS += -Wno-invalid-offsetof -std=gnu++98

ifeq ($(CFG),release)
CFLAGS += -O3 -s
else
CFLAGS += -g
CFG=debug
endif

ifeq ($(SWL_BACKEND_XLIB),1)
LIBS += X11
endif

ifeq ($(SWL_CONTEXT_EGL),1)
LIBS += EGL
endif

ifeq ($(SWL_CONTEXT_WGL),1)
LIBS += gdi32
endif

CPPFLAGS += -Iinclude
LDFLAGS += $(patsubst %, -l%, $(LIBS))

.PHONY: build-dirs
build-dirs:
	@mkdir -p "$(CFG)"

$(CFG)/%.o: src/%.c
	@echo Building $< for $(CFG)
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

TARGET = $(CFG)/libswl$(LIB_EXT)

SOURCES = $(NULL)

ifeq ($(SWL_BACKEND_WINAPI),1)
SOURCES += swl-winapi
endif

ifeq ($(SWL_BACKEND_XLIB),1)
SOURCES += swl-xlib
endif

ifeq ($(SWL_CONTEXT_EGL),1)
SOURCES += swl-egl
endif

ifeq ($(SWL_CONTEXT_WGL),1)
SOURCES += swl-wgl
endif

HEADERS = $(patsubst %, include/%.h, $(SOURCES) )
HEADERS += include/swl-features.h

SOURCES += swl
OBJECTS = $(patsubst %, $(CFG)/%.o, $(SOURCES) )

$(TARGET): build-dirs $(OBJECTS)
	@$(AR) rsc $@ $(OBJECTS)

.PHONY: clean 
clean:
	@rm -f $(OBJECTS) $(TARGET) include/swl-features.h Makefile.autoconf

.PHONY: install 
install: $(TARGET)
	@mkdir -p $(INSTALL_PREFIX)/include $(INSTALL_PREFIX)/lib
	@cp -f $(CFG)/libswl$(LIB_EXT) $(INSTALL_PREFIX)/lib
	@cp -f $(HEADERS) $(INSTALL_PREFIX)/include

all: $(TARGET)

