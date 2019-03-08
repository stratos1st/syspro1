#ifndef BITCOIN_STRUCT
#define BITCOIN_STRUCT

#include "../tree_node/tree_node.hpp"
#include <string.h>

class tree_node;

class bitcoin_struct{
public:
  char coin_id[51];
  unsigned int succ_trans;
  tree_node *root;
  const unsigned int value;

  bitcoin_struct(char* id, unsigned int val);
  ~bitcoin_struct();

  char* get_id();

  void print_debug();
};

#endif
