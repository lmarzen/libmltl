CXX = g++
CFLAGS = -std=c++17 -Wall -fno-rtti
LDFLAGS =

ifeq ($(DEBUG), 1)
  CFLAGS += -DDEBUG -g -O0
else
  CFLAGS += -DNDEBUG -O3
endif

SRC_PATH := src
SRC_PYBIND_PATH := $(SRC_PATH)/pybind
LIB_PATH := lib
OBJ_PATH := obj

# src files & obj files
SRC := $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,.cc)))
SRC_PYBIND := $(foreach x, $(SRC_PYBIND_PATH), $(wildcard $(addprefix $(x)/*,.cc)))
OBJ := $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))

STATIC_LIB = $(LIB_PATH)/libmltl.a
DYNAMIC_PYLIB = $(LIB_PATH)/libmltl$(shell python3-config --extension-suffix)

.PHONY: default all clean cpp python

default: all
all: cpp python
cpp: $(STATIC_LIB)
python: $(DYNAMIC_PYLIB)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cc
	@mkdir -p $(OBJ_PATH)
	$(CXX) $(CFLAGS) -c $< -o $@

$(STATIC_LIB): $(OBJ)
	@mkdir -p $(LIB_PATH)
	ar rcs $@ $^

$(DYNAMIC_PYLIB): $(SRC_PYBIND) $(SRC)
	@mkdir -p $(LIB_PATH)
	$(CXX) -std=c++17 -shared -fPIC \
		$(shell python3-config --cflags --ldflags) \
		-o $@ $^

clean:
	rm -rf $(LIB_PATH) $(OBJ_PATH)



