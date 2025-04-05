PROJECT = enumcom

CC = gcc
CXX = g++
STRIP = strip
RM = rm -f

CPPFLAGS =
LDFLAGS = -static
LDLIBS = -lsetupapi -lhid

SRCS = EnumSerial.cpp main.cpp
OBJS = $(subst .cpp,.o,$(SRCS))

TARGET = $(PROJECT)

ifdef LIBRARY
	TARGET := $(TARGET).dll
	CPPFLAGS += -DLIBRARY
	LDFLAGS += -shared -Wl,--add-stdcall-alias
else
	TARGET := $(TARGET).exe
endif

ifeq ($(ARCH),32)
	LDFLAGS += -m32
endif

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS)
	$(STRIP) $(TARGET)

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) $(TARGET)
