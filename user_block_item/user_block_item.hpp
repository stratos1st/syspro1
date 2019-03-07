#ifndef USER_BLOCK_ITEM
#define USER_BLOCK_ITEM

#include "../wallet_struct/wallet_struct.hpp"
#include "../list_structs/transactions_node_struct.hpp"

class user_block_item{
private:
  transactions_node_struct* start;

  bool is_empty();
public:
  wallet_struct* wallet;

  user_block_item();
  ~user_block_item();

  void insert_first(transaction_struct* new_transaction);
  char* get_id();

  void print_debug();
};

#endif