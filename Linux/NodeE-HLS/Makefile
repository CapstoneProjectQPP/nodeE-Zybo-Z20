###############################################################
# Standard Makefile template for all projects
# Copyright Danh Nguyen 2021
# Based on GNU C++ standard
#
# https://github.com/danny-cpp
###############################################################
PRJNAME := nodeE-hls.exe

# These following directories are assumed to exist; abide to GNU guideline.
CC	:= g++
SRCDIR	:= src
BLDDIR	:= build
TARGET	:= bin/$(PRJNAME)


### CONFIGURABLE SECTION !!!
#=============================================================================
# Do not remove -MP -MD flags. These are necessary for generating *.d files,
# which contains rules for headers.
# Change optimization flag (-OX) here. Add/Remove debug flag (-g) here.
CFLAGS := -std=c++14 -MP -MD -pthread -pipe
DEBUG := $(CFLAGS) -Wall -O0 -g -D_GLIBCXX_DEBUG -DDEBUG_MODE
RELEASE := $(CFLAGS) -w -O3 -march=native -D_TRACK_STATS
EXTREME := $(CFLAGS) -Ofast -frename-registers -fopenmp -D_GLIBCXX_PARALLEL -march=native # Don't even think about it
# Turn this to false to see command echo to shell
SUPPR_ECHO := true

# Parallel compilation. By default, will spawn NPROCS processes, which will be
# determined by /proc/cpuinfo for Linux and WSL.
NPROCS = $(shell grep -c 'processor' /proc/cpuinfo)
# Using the following command instead for MacOS
### NPROCS = $(shell sysctl hw.ncpu  | grep -o '[0-9]\+')
# If error occurs manually reduce NPROCS to 1.
MAKEFLAGS += -j$(NPROCS)

# CONFIGURE RELEASE/DEBUG MODE HERE
MODE := $(DEBUG)
#=============================================================================


SRC		:= $(shell find $(SRCDIR) -type f -name *.cpp)
OBJS	:= $(patsubst $(SRCDIR)/%, $(BLDDIR)/%, $(SRC:.cpp=.o))
INCLUDE	:= -I include

.PHONY: all clean


all: $(TARGET)


$(TARGET): $(OBJS)
	@echo "\n##__Linking objs"
	@echo "Total process spawned for compilation: $(NPROCS)"
	@$(CC) $(MODE) -o $@ $^ || (echo "Linking failed"; exit 20)
-include $(BLDDIR)/*.d

# This section already include automatic dependency tracking by using -include
# directive above. Do not add anything in the rules
$(BLDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BLDDIR) || $(SUPPR_ECHO)
	$(shell rsync -a --include='*/' --exclude='*' $(SRCDIR)/ $(BLDDIR))
	@echo "\n##__Compiling translation unit" $@
	@$(CC) $(INCLUDE) $(MODE) -c -o $@ $< || (echo "Compilation failed"; exit 10)


clean:
	rm -fr $(BLDDIR)
	rm -fr bin/*.exe
	rm -rf $(PRJNAME)
	rm -rf $(BLDDIR_T)




