#ifndef WALLET_STRUCT
#define WALLET_STRUCT

#include "../list_structs/wallet_node_struct.hpp"
#include "../bitcoin_struct/bitcoin_struct.hpp"
#include "../transaction_struct/transaction_struct.hpp"
#include <string.h>

class transaction_struct;
class wallet_node_struct;

class wallet_struct{
private:
  char wallet_id[51];
  wallet_node_struct *start;//pointer to list of bitcoin parts owned
public:
  unsigned int send_money_total, recv_money_total,total_money;//total money send, receved, owned by user

  wallet_struct(char *usrid);
  ~wallet_struct();//deletes list

  void add_initial_coin(bitcoin_struct *coin);//adds a coin to the wallet
  //should be called like recv_money(send_money(transaction_struct *trans))
  wallet_node_struct* send_money(transaction_struct *trans, bool increase_flag=true);//handles the hole transaction and returns recver list_node
  //increase_flag is not being used. it is suposed to be used if every transaction counts as one even if it is broken into smaller ones
  void recv_money(wallet_node_struct *new_wallet_list_node);//inserts wallet list node and updates money values
  unsigned int get_possible_transaction_money();//returns returns start->leaf_node->get_curr_money()
  bool is_transaction_possible(transaction_struct *trans);//returns true if wallet has enought total money
  char* get_id();

  void print_debug();
};

#endif
