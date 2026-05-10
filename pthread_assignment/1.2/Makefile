CC      = gcc
CFLAGS  = -Wall -Wextra -g 

#---------- EXECUTABLE ---------------
MUTEX_TARGET = main_mutex
RWLOCK_TARGET = main_rwlock
ATOMIC_TARGET  = main_atomic

#---------- SOURCE FILES ---------------
MUTEX_SRC = ./src/increase_mutex.c
RWLOCK_SRC = ./src/increase_read_write.c
ATOMIC_SRC  = ./src/increase_atomic_operations.c


all: $(MUTEX_TARGET) $(RWLOCK_TARGET) $(ATOMIC_TARGET)

$(MUTEX_TARGET): $(MUTEX_SRC)
	$(CC) $(CFLAGS) $(MUTEX_SRC) -o $(MUTEX_TARGET) -lpthread

$(RWLOCK_TARGET): $(RWLOCK_SRC)
	$(CC) $(CFLAGS) $(RWLOCK_SRC) -o $(RWLOCK_TARGET) -lpthread

$(ATOMIC_TARGET): $(ATOMIC_SRC)
	$(CC) $(CFLAGS) $(ATOMIC_SRC) -o $(ATOMIC_TARGET) -lpthread

#---------- COMMANDS ---------------
run_mutex: $(MUTEX_TARGET)
	./$(MUTEX_TARGET) 10

run_rwlock: $(RWLOCK_TARGET)
	./$(RWLOCK_TARGET) 10

run_atomic: $(ATOMIC_TARGET)
	./$(ATOMIC_TARGET) 10

valgrind_mutex: $(MUTEX_TARGET)
	valgrind ./$(MUTEX_TARGET) 10

valgrind_rwlock: $(RWLOCK_TARGET)
	valgrind ./$(RWLOCK_TARGET) 10
	
valgrind_atomic: $(ATOMIC_TARGET)
	valgrind ./$(ATOMIC_TARGET) 10

#---------- CLEAN ---------------
clean:
	rm -f $(MUTEX_TARGET) $(RWLOCK_TARGET) $(ATOMIC_TARGET)