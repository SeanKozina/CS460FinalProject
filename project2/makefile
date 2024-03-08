OBJS	= tokenizer.o CommentDeleter.o
SOURCE	= tokenizer.cpp CommentDeleter.cpp
HEADER	= 
OUT	= tokenizer
CC	 = g++
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

tokenizer.o: tokenizer.cpp
	$(CC) $(FLAGS) tokenizer.cpp 

CommentDeleter.o: CommentDeleter.cpp
	$(CC) $(FLAGS) CommentDeleter.cpp 


clean:
	rm -f $(OBJS) $(OUT)