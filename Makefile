# Compiler and flags
CC := gcc
CFLAGS := -g

# Directories
SRCDIR := src
OBJDIR := obj

# Source and object files
SRC := $(wildcard $(SRCDIR)/*.c)
OBJ := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))

# Targets
TARGET := my_program

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<


clean:
	rm -f $(OBJDIR)/*.o $(TARGET)

.PHONY: all clean