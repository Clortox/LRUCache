###############################################################################
# Tyler Perkins
# 7-21-21
# Makefile
#

CC = g++

FLAGS = -pipe

CFLAGS  = -Wall
CFLAGS += -Ofast
#CFLAGS += -g
CFLAGS += -pg

LIBRARIES = 

SRC = $(shell find . -name '*.cpp')
HPP = $(shell find . -name '*.hpp')
OBJ = $(subst .cpp,.o,$(SRC))
BIN = ./bin

TARGET = lrucache-test.out

all : $(BIN)/$(TARGET)

$(BIN)/$(TARGET) : $(OBJ)
	@echo LD $(TARGET)
	@$(CC) $(FLAGS) $(CFLAGS) -o $(BIN)/$(TARGET) $(OBJ) $(LIBRARIES)

.cpp.o :
	@echo CC $<
	@$(CC) $(FLAGS) $(CFLAGS) $(LIBRARIES) -c $< -o $@ -D_LRU_DEBUG_

clean :
	find . -type f -name '*.o' -delete
	rm -rf $(TARGET) performance.prof gmon.out

sanity : $(BIN)/$(TARGET)
	$(BIN)/$(TARGET)

profile: sanity
	gprof $(BIN)/$(TARGET) > performance.prof
