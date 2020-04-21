CC = g++

CFLAGS +=-c -g -O0 -Wall -Itools/lib/rdkafka/include

LFLAGS = -lz -lpthread -lrt -lrdkafka++ -lrdkafka

SOURCES = main.cpp

OBJECTS = $(SOURCES:.cpp=.o)

EXECUTABLE = server-test

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)