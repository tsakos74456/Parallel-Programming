# Makefile για το project test_main

# Ορισμοί Μεταβλητών
CC = gcc
CFLAGS = -Wall -fopenmp  -g 
INC_DIR = include
INCLUDES = -I$(INC_DIR)

TARGET = test_main

# Φάκελος Εξόδου για τα .o αρχεία
BUILD_DIR = gitignore

# ΔΙΟΡΘΩΣΗ 1: Σωστά ονόματα αρχείων πηγής
SRC = src/omp_main.c src/omp_merge_functions.c

# Αρχεία αντικειμένου (.o files) με τη διαδρομή του φακέλου BUILD_DIR
OBJ_NAMES = $(patsubst src/%.c, %.o, $(SRC)) 
OBJ = $(addprefix $(BUILD_DIR)/, $(OBJ_NAMES)) 

# Ορισμός του VPATH 
VPATH = src

# Πλήρης λίστα όλων των αρχείων κεφαλίδας στον φάκελο include/
HEADERS = $(wildcard $(INC_DIR)/*.h)

# Προσθέτουμε τον BUILD_DIR στα phony targets
.PHONY: all clean run valgrind $(BUILD_DIR)

# -----------------
# Κανόνες Δόμησης
# -----------------

# 1. all: εξαρτάται από τη δημιουργία του φακέλου και του εκτελέσιμου
all: $(BUILD_DIR) $(TARGET)

# 2. Κανόνας για τη δημιουργία του φακέλου gitignore (αν δεν υπάρχει)
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# 3. Κανόνας για τη σύνδεση των αρχείων .o στον εκτελέσιμο στόχο
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ 

# Αναγνωρίζει στόχους της μορφής gitignore/%.o και τους δημιουργεί:
$(BUILD_DIR)/%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# -----------------
# Βοηθητικοί Κανόνες
# -----------------

run_serial : $(BUILD_DIR) $(TARGET)
	./$(TARGET) 10000000 serial 16

run_parallel: $(BUILD_DIR) $(TARGET)
	./$(TARGET) 10000000 parallel 16

valgrind_p: $(TARGET) 
	valgrind --leak-check=full --track-origins=yes ./$(TARGET) 100 parallel 1

valgrind_s: $(TARGET) 
	valgrind --leak-check=full --track-origins=yes ./$(TARGET) 100 serial 1

clean:
	rm -rf $(BUILD_DIR) 
	rm -f $(TARGET)  2.3.txt