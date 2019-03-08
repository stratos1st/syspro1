#ifndef TRANSACTION_STRUCT
#define TRANSACTION_STRUCT

#include <ctime>
#include <string.h>
#include <iostream>

class wallet_struct;

class transaction_struct{
public:
  wallet_struct *sender, *recver;
  char trans_id[51];
  struct tm *date;
  unsigned int money;

  transaction_struct(char* id);
  ~transaction_struct();

  char* get_id();

  void print_debug();
};

#endif
