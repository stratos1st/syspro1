CC= g++
CGLAG= -g -Wall

main.out: main.o tree_node.o wallet_struct.o block.o user_block_item.o
	$(CC) $(CFLAG) -o main.out main.o tree_node.o wallet_struct.o block.o user_block_item.o

main.o: main.cpp
	$(CC) -c main.cpp

tree_node.o: ./tree_node/tree_node.cpp ./tree_node/tree_node.hpp
	$(CC) -c ./tree_node/tree_node.cpp

wallet_struct.o: ./wallet_struct/wallet_struct.cpp ./wallet_struct/wallet_struct.hpp
	$(CC) -c ./wallet_struct/wallet_struct.cpp

block.o: ./block/block.cpp ./block/block.hpp
	$(CC) -c ./block/block.cpp

user_block_item.o: ./user_block_item/user_block_item.cpp ./user_block_item/user_block_item.hpp
	$(CC) -c ./user_block_item/user_block_item.cpp

.PHONY: clean
clean:
	rm -f main.out main.o tree_node.o wallet_struct.o block.o user_block_item.o
