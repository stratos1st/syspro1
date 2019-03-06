CC= g++
CGLAG= -g -Wall

main.out: main.o tree_node.o wallet_struct.o
	$(CC) $(CFLAG) -o main.out main.o tree_node.o wallet_struct.o

main.o: main.cpp
	$(CC) -c main.cpp

tree_node.o: ./tree_node/tree_node.cpp ./tree_node/tree_node.hpp
	$(CC) -c ./tree_node/tree_node.cpp

wallet_struct.o: ./wallet_struct/wallet_struct.cpp ./wallet_struct/wallet_struct.hpp
	$(CC) -c ./wallet_struct/wallet_struct.cpp

.PHONY: clean
clean:
	rm -f main.out main.o tree_node.o wallet_struct.o
