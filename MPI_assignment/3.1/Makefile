# Ορισμοί Μεταβλητών
CC = mpicc
CFLAGS = -Wall -g
INC_DIR = include
INCLUDES = -I$(INC_DIR)

TARGET = test_main
BUILD_DIR = gitignore

SRC = src/mpi_main.c src/mpi_multiplying_polynominals.c

# Μετατροπή των μονοπατιών src σε μονοπάτια build
# Από src/file.c σε gitignore/file.o
OBJ = $(patsubst src/%.c, $(BUILD_DIR)/%.o, $(SRC))

# Πλήρης λίστα headers για να γίνεται recompilation αν αλλάξουν
HEADERS = $(wildcard $(INC_DIR)/*.h)

.PHONY: all clean run valgrind

# -----------------
# Κανόνες Δόμησης
# -----------------

all: $(BUILD_DIR) $(TARGET)

# Δημιουργία φακέλου αν δεν υπάρχει
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Σύνδεση (Linking)
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

$(BUILD_DIR)/%.o: src/%.c $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# -----------------
# Βοηθητικοί Κανόνες
# -----------------

run: all
	mpiexec -f ../machines -n 4 ./$(TARGET) 10000 

valgrind: all
	valgrind --leak-check=full --track-origins=yes ./$(TARGET) 100 

clean:
	rm -rf $(BUILD_DIR) 
	rm -f $(TARGET) 3.1.txt