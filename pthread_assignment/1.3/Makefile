# Makefile για το project test_main

# Ορισμοί Μεταβλητών
CC = gcc
CFLAGS = -Wall -Wextra -pthread -g -lrt
INC_DIR = include
INCLUDES = -I$(INC_DIR)

TARGET = test_main

# Φάκελος Εξόδου για τα .o αρχεία
BUILD_DIR = gitignore

# Διαδρομές αρχείων πηγής (.c files)
SRC = src/main.c src/Mystruct.c

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

clean:
	
	rm -rf $(BUILD_DIR) 
	rm -f $(TARGET) 1.3.txt

run: $(BUILD_DIR) $(TARGET)
	./$(TARGET) 100000000

valgrind: $(TARGET)
	valgrind --leak-check=full --track-origins=yes ./$(TARGET) 10000000