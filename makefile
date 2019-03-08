CC= g++
CGLAG= -g -Wall

main.out: main.o transaction_struct.o bitcoin_struct.o tree_node.o wallet_struct.o block.o user_block_item.o hash_table.o
	$(CC) $(CFLAG) -o main.out main.o transaction_struct.o bitcoin_struct.o tree_node.o wallet_struct.o block.o user_block_item.o hash_table.o

main.o: main.cpp
	$(CC) -c main.cpp

transaction_struct.o: ./transaction_struct/transaction_struct.cpp ./transaction_struct/transaction_struct.hpp
	$(CC) -c ./transaction_struct/transaction_struct.cpp

bitcoin_struct.o: ./bitcoin_struct/bitcoin_struct.cpp ./bitcoin_struct/bitcoin_struct.hpp
	$(CC) -c ./bitcoin_struct/bitcoin_struct.cpp

tree_node.o: ./tree_node/tree_node.cpp ./tree_node/tree_node.hpp
	$(CC) -c ./tree_node/tree_node.cpp

wallet_struct.o: ./wallet_struct/wallet_struct.cpp ./wallet_struct/wallet_struct.hpp
	$(CC) -c ./wallet_struct/wallet_struct.cpp

block.o: ./block/block.cpp ./block/block.hpp
	$(CC) -c ./block/block.cpp

user_block_item.o: ./user_block_item/user_block_item.cpp ./user_block_item/user_block_item.hpp
	$(CC) -c ./user_block_item/user_block_item.cpp

hash_table.o: ./hash_table/hash_table.cpp ./hash_table/hash_table.hpp
	$(CC) -c ./hash_table/hash_table.cpp

.PHONY: clean
clean:
	rm -f main.out main.o transaction_struct.o bitcoin_struct.o tree_node.o wallet_struct.o block.o user_block_item.o hash_table.o
