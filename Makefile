root=..
default: all
include $(root)/Makefile.common
include Makefile.conf

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

OBJECTS = $(patsubst %.c, $(CFG)/%.o, $(SOURCES) )

$(TARGET): $(OBJECTS)
	@$(AR) rsc $@ $(OBJECTS)

.PHONY: clean 
clean:
	@rm -f $(OBJECTS) $(TARGET)

all: build-dirs $(TARGET)
