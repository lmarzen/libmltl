CC := g++
CFLAGS := -std=c++17 -pedantic -Wall -fno-rtti
LDFLAGS := -L../../lib -lmltl -flto
INCLUDES := -I../../include

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

TARGET := regression
RESULTS := results.txt

.PHONY: all clean test reference

all: $(TARGET)

$(TARGET): regression.cc
	$(CXX) $(CFLAGS) -o $@ $^ $(INCLUDES) $(LDFLAGS)

# run regression tests (compares to reference file results.txt)
test: $(TARGET)
	gzip -dkf $(RESULTS).gz
	./$(TARGET) -r $(RESULTS)

# run regression tests (saves results to reference file)
reference: $(TARGET)
	./$(TARGET) -o $(RESULTS)
	gzip -k9 $(RESULTS)

clean:
	rm -f $(TARGET) $(RESULTS) gmon.out
