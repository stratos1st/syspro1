#ifndef WALLET_STRUCT
#define WALLET_STRUCT

#include "../list_structs/wallet_node_struct.hpp"
#include "../bitcoin_struct/bitcoin_struct.hpp"
#include "../transaction_struct/transaction_struct.hpp"
#include <string.h>

class transaction_struct;

class wallet_struct{
private:
  char wallet_id[51];
  wallet_node_struct *start;
public:
  unsigned int send_money_total, recv_money_total,total_money;

  wallet_struct(char *usrid);
  ~wallet_struct();

  void add_initial_coin(bitcoin_struct *coin);
  wallet_node_struct* send_money(transaction_struct *trans, bool increase_flag=true);
  void recv_money(wallet_node_struct *new_wallet_list_node);
  unsigned int get_possible_transaction_money();
  bool is_transaction_possible(transaction_struct *trans);
  char* get_id();

  void print_debug();
};

#endif
