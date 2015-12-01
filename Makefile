DIR=.
BIN_DIR=$(DIR)/bin
SRC_DIR=$(DIR)
INCLUDE_DIR=$(DIR)/
OBJ_DIR=$(DIR)/obj
LIB_DIR=$(DIR)/lib
LIB=libchash.a

EXTENSION=cc
OBJS=$(patsubst $(SRC_DIR)/%.$(EXTENSION), $(OBJ_DIR)/%.o,$(wildcard $(SRC_DIR)/*.$(EXTENSION)))
DEPS=$(patsubst $(OBJ_DIR)/%.o, $(DEPS_DIR)/%.d, $(OBJS))

INCLUDE= 
		
CC=g++
AR= ar rcu
CFLAGS=-Wall -Werror -g 
#LDFLAGS= -L ./lib -lcr -pthread
LDFLAGS= -L ./lib -lchash -ldl

all:$(OBJS)
	$(AR) $(LIB_DIR)/$(LIB) $(OBJS)
	$(CC) test.cc -I./src $(CFLAGS) $(INCLUDE) -o test $(LDFLAGS)

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.$(EXTENSION) 
	$(CC) $< -o $@ -c $(CFLAGS) $(INCLUDE) 

rebuild:
	make clean
	make

clean:
	rm -rf $(OBJS) $(BIN_DIR)/* $(LIB_DIR)/*

