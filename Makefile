CC := g++

SRCDIR := src
BUILDDIR := build
INCDIR := include
TARGET := bin/game

LIB := -lallegro -lallegro_image

$(TARGET): $(SRCDIR)/main.cpp
	mkdir -p $(BUILDDIR)
	$(CC) $^ -I $(INCDIR) -o $@ $(LIB) -g $(extra)

clean:
	rm -r $(TARGET)
