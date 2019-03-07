#ifndef HASH_TABLE
#define HASH_TABLE

#include "../block/block.hpp"
#include <string.h>

template<class T>
class hast_table{
private:
  block<T>** table;
  block<T>* last;
  unsigned int max_sz;
  const unsigned int block_bytes;

  unsigned int hash_function(T* item);
  unsigned int hash_function(char* str);
public:
  hast_table(unsigned int sz, unsigned int max_block_bytes);
  ~hast_table();

  T* find(char* id);
  void insert(T* new_item);

  void print_debug();
};

#endif
