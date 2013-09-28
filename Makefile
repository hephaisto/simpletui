CPPFLAGS=-Wall -std=c++0x
CPP=g++
LD=g++	#ensures stdc++ is also linked
VERSION=1
SONAME=libsimpletui.so

INSTALL_LIB=/usr/local/lib
INSTALL_INC=/usr/local/include


all: $(SONAME)


simpletui.o: simpletui.cpp simpletui.hpp
	$(CPP) $(CPPFLAGS) -fPIC -c $< -o $@

$(SONAME).$(VERSION): simpletui.o
	$(LD) -shared -Wl,-soname,$@ -o $@ $< -lcdk

$(SONAME): $(SONAME).$(VERSION)
	ln -s $< $@

test_local: simpletui_test_local
	LD_LIBRARY_PATH=. ./simpletui_test_local

test: simpletui_test
	./simpletui_test

simpletui_test_local: simpletui_test.o $(SONAME)
	$(LD) -o $@ $< -L. -lsimpletui

simpletui_test: simpletui_test.o $(SONAME)
	$(LD) -o $@ $< -L$(INSTALL_LIB)/ -lsimpletui

simpletui_test.o: simpletui_test.cpp
	$(CPP) $(CPPFLAGS) -o $@ -c $<

install:
	cp $(SONAME).$(VERSION) $(INSTALL_LIB)/
	ln -s $(SONAME).$(VERSION) $(INSTALL_LIB)/$(SONAME)
	cp simpletui.hpp $(INSTALL_INC)/
	lcdonfig

clean:
	rm *.o $(SONAME) $(SONAME).$(VERSION) simpletui_test
