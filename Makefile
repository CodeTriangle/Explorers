CC := g++

SRCDIR := src
BUNDLEDIR := Explorers
INCDIR := include
TARGET := bin/game

LIB := $(shell sdl2-config --cflags --libs)

VERSION := 1.1.0

$(TARGET): $(SRCDIR)/main.cpp
	mkdir -p bin
	$(CC) $^ -I $(INCDIR) -o $@ $(LIB) -g $(extra)

clean:
	rm -r $(TARGET)

bundle:
	mkdir -p $(BUNDLEDIR)
	cp $(TARGET) LICENSE -t $(BUNDLEDIR)
	cp -r assets -t $(BUNDLEDIR)
	tar -xzvf $(BUNDLEDIR)-$(VERSION).tar.gz $(BUNDLEDIR)
