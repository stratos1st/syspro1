#ifndef BLOCK
#define BLOCK

#include "../user_block_item/user_block_item.hpp"
#include "../bitcoin_struct/bitcoin_struct.hpp"
#include "../transaction_struct/transaction_struct.hpp"
#include <string.h>

template <class T>
class block{
private:
  T** table;//table of pointes to T
  unsigned int curr_pos,max_sz;//curr_pos is the current free table possition, max_sz is the size of the table
public:
  block* next;//pointer to the next block

  block(unsigned int byte_max_sz);
  ~block();//deletes block chain and table

  T* find_in_block(char* id);//returns item with correct id or nullptr
  bool insert_last(T* new_item);//inserts wallet to block, returns true if succesfull
  bool is_full();//returns true if block is full
  void delete_wallets();//deletes all wallets in this block

  void print_debug();
};

#endif
