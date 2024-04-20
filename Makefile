CXX := g++
CFLAGS := -std=c++17 -pedantic -Wall -fno-rtti
LDFLAGS :=
INCLUDES := -Iinclude
ifeq ($(DEBUG), 1)
  CFLAGS += -DDEBUG -g -O0
else
  CFLAGS += -DNDEBUG -O3
endif

SRC_PATH := src
SRC_PYBIND_PATH := $(SRC_PATH)/pybind
LIB_PATH := lib
OBJ_PATH := obj
INC_PATH := include

# src files & obj files
SRC := $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,.cc)))
SRC_PYBIND := $(foreach x, $(SRC_PYBIND_PATH), $(wildcard $(addprefix $(x)/*,.cc)))
OBJ := $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))
HEADERS := $(foreach x, $(INC_PATH), $(wildcard $(addprefix $(x)/*,.hh)))

STATIC_LIB := $(LIB_PATH)/libmltl.a
DYNAMIC_PYLIB := $(LIB_PATH)/libmltl$(shell python3-config --extension-suffix)

.PHONY: default all clean cpp python examples install uninstall

default: cpp python
all: cpp python examples
cpp: $(STATIC_LIB)
python: $(DYNAMIC_PYLIB)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cc $(HEADERS) Makefile
	@mkdir -p $(OBJ_PATH)
	$(CXX) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(STATIC_LIB): $(OBJ)
	@mkdir -p $(LIB_PATH)
	ar rcs $@ $^

$(DYNAMIC_PYLIB): $(SRC_PYBIND) $(SRC) $(HEADERS) Makefile
	@mkdir -p $(LIB_PATH)
	$(CXX) -std=c++17 -shared -fPIC $(INCLUDES) \
		$(shell python3-config --cflags --ldflags) \
		-o $@ $(SRC_PYBIND) $(SRC)

examples:
	$(MAKE) -C $@

clean_examples:
	$(MAKE) -C examples clean

# if user didn't set install prefix with `make install PREFIX=` use default
ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

install: $(STATIC_LIB) $(DYNAMIC_PYLIB)
	install -d $(PREFIX)/lib/libmltl
	install -m 644 $(STATIC_LIB) $(PREFIX)/lib/libmltl/
	install -m 755 $(DYNAMIC_PYLIB) $(PREFIX)/lib/libmltl/
	install -d $(PREFIX)/include/libmltl
	install -m 755 $(INC_PATH)/* $(PREFIX)/include/libmltl/

uninstall:
	rm -rf $(PREFIX)/lib/libmltl
	rm -rf $(PREFIX)/include/libmltl

clean:
	rm -rf $(LIB_PATH) $(OBJ_PATH)



