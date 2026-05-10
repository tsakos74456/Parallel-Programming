CC      = mpicc
CFLAGS  = -Wall -Wextra 


#---------- EXECUTABLE ---------------
TATGET = matrix_mul


#---------- SOURCE FILES ---------------
SRC = ./src/functions.c ./src/main.c


all: $(TATGET)

$(TATGET): $(SRC) ./include/functions.h ./include/timer.h
	$(CC) $(CFLAGS) $(SRC) -o $(TATGET) 

#---------- COMMANDS ---------------
run: $(TATGET)
	mpiexec -f ../machines -n 4 ./$(TATGET)  2000 30 10

#---------- CLEAN ---------------
clean:
	rm -f $(TATGET) 