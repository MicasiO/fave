CC = gcc
TARGET = fave
SRC = main.c utils.c dir.c array.c action.c  
PREFIX = /usr/local

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

install: $(TARGET)
	@echo "Installing $(TARGET) to $(PREFIX)/bin..."
	@mkdir -p $(PREFIX)/bin
	@cp $(TARGET) $(PREFIX)/bin/$(TARGET)
	@chmod +x $(PREFIX)/bin/$(TARGET)
	@echo "Done"

uninstall:
	@echo "Removing $(TARGET) from $(PREFIX)/bin..."
	@rm -f $(PREFIX)/bin/$(TARGET)

clean:
	@echo "Cleaning up..."
	@rm -f $(TARGET)

.PHONY: all install uninstall clean
