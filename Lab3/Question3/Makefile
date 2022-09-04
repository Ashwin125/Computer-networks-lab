CC = g++
STD = -std=c++17

all:
	$(CC) $(STD) server_A.cpp -o server_A
	$(CC) $(STD) server_B.cpp -o server_B
	$(CC) $(STD) client.cpp -o client

genfile:
	$(CC) $(STD) random_file_generator.cpp -o random_file_generator
	./random_file_generator
	$(RM) random_file_generator

clean:
	$(RM) target.txt
	$(RM) server_A
	$(RM) server_B
	$(RM) client
