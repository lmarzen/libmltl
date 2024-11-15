CC := g++
CFLAGS := -std=c++17 -pedantic -Wall -fno-rtti
LDFLAGS := -L../lib -lmltl -flto
INCLUDES := -I../include

ifeq ($(DEBUG), 1)
  CFLAGS += -DDEBUG -g -O0
else
  CFLAGS += -DNDEBUG -O2
  DEBUG := 0
endif
ifeq ($(PROFILE), 1)
  CFLAGS += -pg
  LDFLAGS += -pg
else
  PROFILE := 0
endif

TARGET := example

.PHONY: all clean

all: $(TARGET)

$(TARGET): example.cc
	$(CXX) $(CFLAGS) -o $@ $^ $(INCLUDES) $(LDFLAGS)

clean:
	rm -f $(TARGET) gmon.out

