PROJECT = enumcom

CC = gcc
CXX = g++
STRIP = strip
RM = rm -f
WINDRES = windres

CPPFLAGS =
LDFLAGS = -static
LDLIBS = -lsetupapi -lhid
WINDRESFLAGS =

SRCS = EnumSerial.cpp main.cpp
OBJS = $(subst .cpp,.o,$(SRCS))

RSRC = $(PROJECT).rc
RES = $(subst .rc,.res,$(RSRC))

TARGET = $(PROJECT)

ifdef LIBRARY
	TARGET := $(TARGET).dll
	CPPFLAGS += -DLIBRARY
	WINDRESFLAGS += -DLIBRARY
	LDFLAGS += -shared -Wl,--add-stdcall-alias
else
	TARGET := $(TARGET).exe
endif

ifeq ($(ARCH),32)
	LDFLAGS += -m32
endif

all: $(TARGET)

$(TARGET): $(OBJS) $(RES)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS) $(RES) $(LDLIBS)
	$(STRIP) $(TARGET)

$(RES) : $(RSRC)
	$(WINDRES) $(WINDRESFLAGS) $< -O coff -o $@

clean:
	$(RM) $(OBJS) $(RES)

distclean: clean
	$(RM) $(TARGET)
