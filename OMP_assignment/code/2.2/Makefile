CC      = gcc
CFLAGS  = -Wall -Wextra -g -fopenmp


#---------- EXECUTABLE ---------------
TATGET = main


#---------- SOURCE FILES ---------------
SRC = ./src/functions.c ./src/main.c


all: $(TATGET)

$(TATGET): $(SRC) ./include/functions.h ./include/timer.h
	$(CC) $(CFLAGS) $(SRC) -o $(TATGET) 

#---------- COMMANDS ---------------
run: $(TATGET)
	./$(TATGET)  20000 30 10 4

#---------- CLEAN ---------------
clean:
	rm -f $(TATGET) 