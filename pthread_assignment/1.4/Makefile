CC      = gcc
CFLAGS  = -Wall -Wextra -g  


#---------- EXECUTABLE ---------------
BANK_TARGET = bank


#---------- SOURCE FILES ---------------
BANK_SRC = ./src/bank_main.c ./src/functions_bank_threads.c


all: $(BANK_TARGET)

$(BANK_TARGET): $(BANK_SRC) ./include/functions_bank_threads.h
	$(CC) $(CFLAGS) $(BANK_SRC) -o $(BANK_TARGET) -lpthread

#---------- COMMANDS ---------------
run_bank_coarse_rw: $(BANK_TARGET)
	./$(BANK_TARGET) 2000000 1 20 1 16

run_bank_coarse_mutex: $(BANK_TARGET)
	./$(BANK_TARGET) 2000000 1500000 30 0 16 

run_bank_fine_mutex: $(BANK_TARGET)
	./$(BANK_TARGET)  3000000 150000 10 2 16

run_bank_fine_rw: $(BANK_TARGET)
	./$(BANK_TARGET)  3000000 10000 50 3 16

#---------- CLEAN ---------------
clean:
	rm -f $(BANK_TARGET) 