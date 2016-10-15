CXX = g++
#CXXFLAGS = -O2 -g -Wall -std=c++11

# Strict compiler options
CXXFLAGS += -Werror -Wformat-security -Wignored-qualifiers -Winit-self \
		-Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith \
		-Wtype-limits -Wempty-body -Wlogical-op \
		-Wmissing-field-initializers -Wctor-dtor-privacy \
		-Wnon-virtual-dtor -Wstrict-null-sentinel \
		-Woverloaded-virtual -Wsign-promo -std=gnu++0x

LDFLAGS = -lconfig++ -ldl

HDR=Server.h Client.h

SRCSRV=main_s.cpp Server.cpp 
OBJSSRV = $(SRCSRV:.cpp=.o) 

SRCCLT=main_c.cpp Client.cpp
OBJSCLT=$(SRCCLT:.cpp=.o)

# Default target (make without specified target).
.DEFAULT_GOAL := all

%.o: %.c $(HDR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

server: $(OBJSSRV)
	$(CXX) $(CXXFLAGS) -o server $(OBJSSRV)

client: $(OBJSCLT)
	$(CXX) $(CXXFLAGS) -o client $(OBJSCLT)

test: test.py server client
	python test.py

all: server client

clean:
	rm client server $(OBJSSRV) $(OBJSCLT)
