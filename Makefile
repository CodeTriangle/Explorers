CC := em++

SRCDIR := src
BUNDLEDIR := Explorers
INCDIR := include
ASSETDIR := assets
TARGET := explorers/index.html

VERSION := 1.1.1

$(TARGET): $(SRCDIR)/main.cpp
	mkdir -p explorers
	$(CC) $^ -s USE_SDL=2 -o $(TARGET) --preload-file $(ASSETDIR)

server:
	python2 -m SimpleHTTPServer

clean:
	rm -r $(TARGET)

bundle:
	mkdir -p $(BUNDLEDIR)
	cp $(TARGET) LICENSE -t $(BUNDLEDIR)
	cp -r assets -t $(BUNDLEDIR)
	tar -czvf $(BUNDLEDIR)-$(VERSION).tar.gz $(BUNDLEDIR)
