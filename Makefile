root=..
default: all
include $(root)/Makefile.common
include Makefile.shared

TARGET = $(CFG)/libswl.a

SOURCES = \
	swl.c \
	$(NULL)

ifeq ($(SWL_BACKEND_WINAPI),1)
SOURCES += swl-winapi.c
endif

ifeq ($(SWL_BACKEND_XLIB),1)
SOURCES += swl-xlib.c
endif

ifeq ($(SWL_CONTEXT_EGL),1)
SOURCES += swl-egl.c
endif

ifeq ($(SWL_CONTEXT_WGL),1)
SOURCES += swl-wgl.c
endif

OBJECTS = $(patsubst %.c, $(CFG)/%.o, $(SOURCES) )

$(TARGET): $(OBJECTS)
	@$(AR) rsc $@ $(OBJECTS)

.PHONY: clean 
clean:
	@rm -f $(OBJECTS) $(TARGET)

all: build-dirs $(TARGET)
