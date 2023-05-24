#Output files to make

CC = g++
CFLAGS = -c -Wall -Iheaders

OUT_HEADERS = main.o server.o logger.o parser.o path.o
HEADER_DIR = headers/
SRC_DIR = src/

output: $(OUT_HEADERS)
	$(CC) $(OUT_HEADERS) -o server

main.o: $(SRC_DIR)main.cpp
	$(CC) $(CFLAGS) $(SRC_DIR)main.cpp

logger.o: $(SRC_DIR)logger.cpp $(HEADER_DIR)logger.h
	$(CC) $(CFLAGS) $(SRC_DIR)logger.cpp

server.o: $(SRC_DIR)server.cpp $(HEADER_DIR)server.h
	$(CC) $(CFLAGS) $(SRC_DIR)server.cpp

parser.o: $(SRC_DIR)parser.cpp $(HEADER_DIR)parser.h 
	$(CC) $(CFLAGS) $(SRC_DIR)parser.cpp

path.o: $(SRC_DIR)path.cpp $(HEADER_DIR)path.h
	$(CC) $(CFLAGS) $(SRC_DIR)path.cpp
clean:
	rm *.o server