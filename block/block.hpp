#ifndef BLOCK
#define BLOCK

#include "../wallet_struct/wallet_struct.hpp"
#include "../tree_node/tree_node.hpp"
#include "../transaction_struct/transaction_struct.hpp"
#include <string.h>

template <class T>
class block{
private:
  T** table;
  unsigned int curr_pos,max_sz;
public:
  block(unsigned int byte_max_sz);
  ~block();

  T* find_in_block(char* id);
  bool insert_last(T* new_item);
  bool is_full();

  void print_debug();
};

#endif
