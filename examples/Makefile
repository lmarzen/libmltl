CC := g++
CFLAGS := -std=c++17 -pedantic -Wall -fno-rtti -static
LDFLAGS := -L../lib -lmltl
INCLUDES := -I../include

TARGET := example

.PHONY: all clean

all: $(TARGET)

$(TARGET): example.cc
	$(CXX) $(CFLAGS) -o $@ $^ $(INCLUDES) $(LDFLAGS)

clean:
	rm -f $(TARGET)

