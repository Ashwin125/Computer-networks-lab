CC = g++

all:
	$(CC) server_A.cpp -o server_A
	$(CC) server_B.cpp -o server_B
	$(CC) client.cpp -o client

genfile:
	$(CC) random_file_generator.cpp -o random_file_generator
	./random_file_generator
	$(RM) random_file_generator

clean:
	$(RM) server_A
	$(RM) server_B
	$(RM) client
