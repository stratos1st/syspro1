#include <iostream>

#include "./wallet_struct/wallet_struct.hpp"
#include "./transaction_struct/transaction_struct.hpp"
#include "./hash_table/hash_table.hpp"

using namespace std;

int main(){

  hash_table<transaction_struct> table(1,40);

  transaction_struct *block_item=new transaction_struct("stratos");
  table.insert(block_item);

  for(int i=0;i<6;i++){
    char tmp[51];
    sprintf(tmp, "a%d",i);
    transaction_struct *block_item2=new transaction_struct(tmp);
    table.insert(block_item2);
  }


  table.print_debug();

  table.find("a3")->print_debug();

  return 0;
}
