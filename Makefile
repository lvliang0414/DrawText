

CFLAGS:= -g  -I include -I include/freetype2
LDFLAGS:= -lpthread -lfreetype

SRCS = DrawText.cc \
		TextRender.cc \
		BmpFunctions.cc \
		easylogging++.cc \
		Functions.cc

CXX = g++ -std=c++11

all: drawtext

drawtext: 
	$(CXX) $(CFLAGS)  $(SRCS) $(LDFLAGS) -o bin/$@

clean:
	rm bin/*

install:
	$(STRIP) bin/*
