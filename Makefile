CC := g++

SRCDIR := src
BUILDDIR := build
INCDIR := include
TARGET := bin/game

LIB := -lallegro

$(TARGET): $(SRCDIR)/main.cpp
	$(CC) $^ -I $(INCDIR) -o $@ $(LIB) $(extra)
