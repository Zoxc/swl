include ../Makefile.common
include Makefile.features

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

.PHONY: build-dir 
build-dir:
	@mkdir -p $(CFG)

$(TARGET): $(OBJECTS)
	@$(AR) rsc $@ $(OBJECTS)

.PHONY: clean 
clean:
	@rm -f $(OBJECTS) $(TARGET)

all: build-dir $(TARGET)
