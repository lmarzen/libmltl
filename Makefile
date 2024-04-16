TARGET = main
CC = gcc
CXX = g++
CFLAGS = -Wall -O3 -fopenmp -DNDEBUG -fno-rtti
# CFLAGS = -Wall -g -fno-rtti
LDFLAGS =
OBJECTS = main.o libmltl.o

PYBIND_SRCS = libmltl_pybind.cc libmltl.cc
PYBIND_TARGET = libmltl$(shell python3-config --extension-suffix)

.PHONY: default all clean cpp python

all: $(TARGET) $(PYTHON_TARGET)
cpp: $(TARGET)
python: $(PYTHON_TARGET)

%.o: %.cc
	$(CXX) $(CFLAGS) -c $< -o $@
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@

$(PYTHON_TARGET): $(PYBIND_SRCS)
	$(CXX) -shared -fPIC \
		$(shell python3-config --cflags --ldflags) \
		-o $@ $(PYBIND_SRCS)

clean:
	rm -f $(TARGET) $(PYTHON_TARGET) *.o *.so vgcore.*

