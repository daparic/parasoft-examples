CC=g++
INCLUDE_FLAGS=-Iinclude
DEBUG_FLAGS=
CFLAGS=-g
OBJ_DIR=obj

OBJ = $(OBJ_DIR)/NullPointer.o \
	$(OBJ_DIR)/DivisionByZero.o \
	$(OBJ_DIR)/MemoryLeak.o \
	$(OBJ_DIR)/DeadLock.o

.PHONY = clean all
	
all : $(OBJ_DIR) $(OBJ)

$(OBJ_DIR) :
	mkdir $(OBJ_DIR)

$(OBJ_DIR)/%.o : %.cpp
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) -o $@ -c $^ 

clean:
	rm -rf $(OBJ_DIR)
