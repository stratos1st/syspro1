#ifndef USER_BLOCK_ITEM
#define USER_BLOCK_ITEM

#include "../wallet_struct/wallet_struct.hpp"
#include "../list_structs/transactions_node_struct.hpp"
#include <ctime>

class user_block_item{
private:
  transactions_node_struct* start;//poiter to sender or recver transactions list

  bool is_empty();//returns true if list is empty
public:
  wallet_struct* wallet;//pointer to users wallet

  user_block_item();
  ~user_block_item();//deletes list !does NOT delete wallet!

  void insert_first(transaction_struct* new_transaction);//inserts transaction at start of list
  char* get_id();//returns wallet->usr_id
  void print_list(time_t time1={0}, time_t time2={0});//!called with both arguments or none! prints all transactins between time1 and time 2

  void print_debug();
};

#endif
