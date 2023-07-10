PROJECT = lstcom

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
ifeq ($(OS),Windows_NT)
	TARGET := $(TARGET).exe
endif

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS)
	$(STRIP) $(TARGET)

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) $(TARGET)
