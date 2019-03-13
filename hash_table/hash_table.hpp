#ifndef HASH_TABLE
#define HASH_TABLE

#include "../block/block.hpp"
#include <string.h>

template<class T>
class hash_table{
private:
  block<T>** table;//array of pointers to starting block
  block<T>** last;//array of pointers to last block
  unsigned int max_sz;//number of buckets
  const unsigned int block_bytes;//byte size of block

  unsigned int hash_function(T* item);//returns "unic number based on item->get_id"
  unsigned int hash_function(char* str);//returns "unic" number based on str
public:
  hash_table(unsigned int sz, unsigned int max_block_bytes);
  ~hash_table();//deletes all items in table

  T* find(char* id);// returns item with correct id or nullptr
  void insert(T* new_item);//inserts item in corect bucket and block !even if item exists!
  void delete_wallets();//deletes all wallets

  void print_debug();
};

#endif
