#ifndef TRANSACTION_STRUCT
#define TRANSACTION_STRUCT

#include <ctime>
#include <string.h>
#include <iostream>
#include "../wallet_struct/wallet_struct.hpp"

class wallet_struct;

class transaction_struct{
public:
  wallet_struct *sender, *recver;//pointer to sender, recver
  char trans_id[51];
  struct tm date_tm;//keeping date and time
  time_t date;//keeping date and time
  unsigned int money;//transaction money

  transaction_struct(char* id);
  ~transaction_struct();//does nothing

  char* get_id();//returns trans_id
  void print_transaction();//prints transaction with nc format

  void print_debug();
};

#endif
