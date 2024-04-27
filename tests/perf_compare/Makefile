CC := g++
CFLAGS := -std=c++17 -pedantic -Wall -O2 -fno-rtti
LDFLAGS := -L../../lib -lmltl
INCLUDES := -I../../include -IMLTL_interpreter

OBJ = evaluate_mltl.o utils.o benchmark.o

TARGET := benchmark

.PHONY: all clean

all: $(TARGET)

%.o: %.cc
	$(CXX) -c -o $@ $< $(CFLAGS) $(INCLUDES)

%.o: MLTL_interpreter/%.cpp
	$(CXX) -c -o $@ $< $(CFLAGS) $(INCLUDES)

$(TARGET): $(OBJ)
	$(CXX) $(CFLAGS) -o $@ $^ $(INCLUDES) $(LDFLAGS)

clean:
	rm -f $(TARGET) *.o

