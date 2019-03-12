#ifndef BLOCK
#define BLOCK

#include "../user_block_item/user_block_item.hpp"
#include "../bitcoin_struct/bitcoin_struct.hpp"
#include "../transaction_struct/transaction_struct.hpp"
#include <string.h>

template <class T>
class block{
private:
  T** table;
  unsigned int curr_pos,max_sz;
public:
  block* next;

  block(unsigned int byte_max_sz);
  ~block();

  T* find_in_block(char* id);
  bool insert_last(T* new_item);
  bool is_full();
  void delete_wallets();

  void print_debug();
};

#endif
