CXX := g++
CFLAGS := -std=c++17 -pedantic -Wall -Wextra -fno-rtti
LDFLAGS := -flto
INCLUDES := -Iinclude

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
# for editors using clangd
COMPILE_FLAGS := compile_flags.txt

.PHONY: default all clean debug profile cpp python examples tests perf_compare install uninstall

default: $(COMPILE_FLAGS) cpp python
all: $(COMPILE_FLAGS) cpp python examples tests perf_compare
cpp: $(STATIC_LIB)
python: $(DYNAMIC_PYLIB)

debug:
	$(MAKE) DEBUG=1 --no-print-directory

profile:
	$(MAKE) PROFILE=1 --no-print-directory

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cc $(HEADERS) Makefile
	@mkdir -p $(OBJ_PATH)
	$(CXX) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(STATIC_LIB): $(OBJ)
	@mkdir -p $(LIB_PATH)
	ar rcs $@ $^

$(DYNAMIC_PYLIB): $(SRC_PYBIND) $(SRC) $(HEADERS) Makefile
	@mkdir -p $(LIB_PATH)
	$(CXX) -std=c++17 -shared -fPIC -DNDEBUG -O2 $(INCLUDES) \
		$(shell python3 -m pybind11 --includes) \
		-o $@ $(SRC_PYBIND) $(SRC)

examples: cpp python
	$(MAKE) -C examples DEBUG=$(DEBUG) PROFILE=$(PROFILE) --no-print-directory

tests: cpp python
	$(MAKE) -C tests/regression test DEBUG=$(DEBUG) PROFILE=$(PROFILE) --no-print-directory


perf_compare: cpp python
	$(MAKE) -C tests/perf_compare all DEBUG=$(DEBUG) PROFILE=$(PROFILE) --no-print-directory

FLAGS := $(CFLAGS) $(INCLUDES) $(LFLAGS) $(shell python3 -m pybind11 --includes)
$(COMPILE_FLAGS): Makefile
	@echo -n > $(COMPILE_FLAGS)
	@for flag in $(FLAGS); do \
	    echo "$$flag" >> $(COMPILE_FLAGS); \
	done

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
	rm -rf $(LIB_PATH) $(OBJ_PATH) $(COMPILE_FLAGS) gmon.out
	$(MAKE) -C examples clean --no-print-directory
	$(MAKE) -C tests/regression clean --no-print-directory
	$(MAKE) -C tests/perf_compare clean --no-print-directory
