CC := g++
CFLAGS := -std=c++17 -pedantic -Wall -fno-rtti
LDFLAGS := -L../../lib -lmltl -flto
INCLUDES := -I../../include -IMLTL_interpreter

ifeq ($(DEBUG), 1)
  CFLAGS += -DDEBUG -g -O0
else
  CFLAGS += -DNDEBUG -O2
endif
ifeq ($(PROFILE), 1)
  CFLAGS += -pg
  LDFLAGS += -pg
else
  PROFILE := 0
endif

OBJ = evaluate_mltl.o utils.o benchmark.o

TARGET := benchmark

.PHONY: all clean

all: $(TARGET)

%.o: %.cc
	$(CXX) -c -o $@ $< $(CFLAGS) $(INCLUDES)

%.o: MLTL_interpreter/%.cpp
	$(CXX) -c -o $@ $< $(CFLAGS) $(INCLUDES) -Wno-sign-compare

$(TARGET): $(OBJ)
	$(CXX) $(CFLAGS) -o $@ $^ $(INCLUDES) $(LDFLAGS)

clean:
	rm -f $(TARGET) *.o gmon.out

