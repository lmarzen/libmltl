CC := g++
CFLAGS := -std=c++17 -pedantic -Wall -O3 -fno-rtti
LDFLAGS := -L../../lib -lmltl
INCLUDES := -I../../include

TARGET := regression

.PHONY: all clean

all: $(TARGET)

$(TARGET): regression.cc
	$(CXX) $(CFLAGS) -o $@ $^ $(INCLUDES) $(LDFLAGS)

clean:
	rm -f $(TARGET)

