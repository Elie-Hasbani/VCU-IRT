# --- Configuration ---
CXX      := g++
CXXFLAGS := -Wall -Wextra -O2 -std=c++17
TARGET_DIR := target
TARGET := $(TARGET_DIR)/a.out

# --- Détection automatique des fichiers .cpp ---
SRC := $(wildcard *.cpp)
OBJ := $(patsubst %.cpp,$(TARGET_DIR)/%.o,$(SRC))

# --- Règle principale ---
all: $(TARGET)

# --- Lien final ---
$(TARGET): $(OBJ)
	@if [ -z "$(OBJ)" ]; then \
		echo "❌ Aucun fichier .cpp trouvé dans le dossier courant."; \
		exit 1; \
	fi
	@mkdir -p $(TARGET_DIR)
	$(CXX) $(OBJ) -o $@
	@echo "✅ Build complete: $@"

# --- Compilation des .cpp en .o ---
$(TARGET_DIR)/%.o: %.cpp
	@mkdir -p $(TARGET_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Compiled $< -> $@"

# --- Nettoyage ---
clean:
	rm -rf $(TARGET_DIR)
	@echo "🧹 Cleaned."

# --- Exécution rapide ---
run: all
	@./$(TARGET)

.PHONY: all clean run
