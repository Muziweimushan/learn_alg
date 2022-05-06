
CC := gcc
CXX := g++

DEBUG ?= true

TARGET := test.out

DIR_ROOT := $(realpath .)
DIR_BUILD := build
DIR_OUTPUT := $(DIR_ROOT)/$(DIR_BUILD)
DIR_SRC := $(DIR_ROOT)
DIR_INC := $(DIR_ROOT)

TARGET := $(DIR_OUTPUT)/$(TARGET)

CHECK_OUTPUT_DIR := $(wildcard $(DIR_OUTPUT))

SRCS_C := $(wildcard $(DIR_SRC)/*.c)
SRCS_CXX := $(wildcard $(DIR_SRC)/*.cpp)

OBJS :=
OBJS += $(SRCS_C:.c=.o)
OBJS += $(SRCS_CXX:.cpp=.o)

OBJS := $(patsubst $(DIR_SRC)/%, $(DIR_OUTPUT)/%, $(OBJS))
DEPS := $(OBJS:.o=.dep)

vpath %.h $(DIR_INC)
vpath %.c $(DIR_SRC)
vpath %.cpp $(DIR_SRC)

COMMON_COMPILE_FLAGS := -W{all,extra,pedantic,no-unused-function,shadow,error}

ifeq ("$(DEBUG)", "true")
COMMON_COMPILE_FLAGS += -g3 -O0
else
COMMON_COMPILE_FLAGS += -g0 -O2
endif

CFLAGS := $(COMMON_COMPILE_FLAGS)
CXXFLAGS := $(COMMON_COMPILE_FLAGS)

LDFLAGS :=


all: $(TARGET)

ifneq ("$(MAKECMDGOALS)", "clean")
include $(DEPS)
endif

.PHONY : all clean
#test: | $(DIR_OUTPUT) $(OBJS)
#	@echo "$(DEPS)"

$(TARGET) : $(OBJS)
	@echo "linking target $(TARGET) ..."
	@$(CXX) $(OBJS) $(LDFLAGS) -o $@

$(DIR_OUTPUT):
	@echo "creating directory $@ ..."
	@mkdir -p $@


ifeq ("$(wildcard $(DIR_OUTPUT))", "")
$(DIR_OUTPUT)/%.dep: $(DIR_OUTPUT) %.c
else
$(DIR_OUTPUT)/%.dep: %.c
endif
	@echo "Creating $@ ..."
	@set -e; \
	$(CC) $(CFLAGS) -MM -E $(filter %.c, $^) | sed 's,\(.*\)\.o[ :]*,$(DIR_OUTPUT)/\1.o $@ : ,g' > $@


ifeq ("$(wildcard $(DIR_OUTPUT))", "")
$(DIR_OUTPUT)/%.dep: $(DIR_OUTPUT) %.cpp
else
$(DIR_OUTPUT)/%.dep: %.cpp
endif
	@echo "Creating $@ ..."
	@set -e; \
	$(CC) $(CFLAGS) -MM -E $(filter %.cpp, $^) | sed 's,\(.*\)\.o[ :]*,$(DIR_OUTPUT)/\1.o $@ : ,g' > $@


$(DIR_OUTPUT)/%.o : %.c
	@echo "Creating $@ ..."
	@$(CC) $(CFLAGS) -o $@ -c $(filter %.c, $^)

$(DIR_OUTPUT)/%.o : %.cpp
	@echo "Creating $@ ..."
	$(CXX) $(CXXFLAGS) -o $@ -c $(filter %.cpp, $^)

clean :
	@rm -rf $(DIR_OUTPUT)

