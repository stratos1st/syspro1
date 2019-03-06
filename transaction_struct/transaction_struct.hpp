#ifndef TRANSACTION_STRUCT
#define TRANSACTION_STRUCT

#include <ctime>

class wallet_struct;

typedef struct{
  wallet_struct *sender, *recver;
  char trans_id[51];
  struct tm *date;
  unsigned int money;
} transaction_struct;

#endif
