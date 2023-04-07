CC = gcc

all: reader_writer dining_philosophers

reader_writer: reader_writer.c
	$(CC) -o $@ $< -lpthread -lrt

dining_philosophers: dining_philosophers.c
	$(CC) -o $@ $< -lpthread
