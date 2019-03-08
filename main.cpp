#include <iostream>

#include "./wallet_struct/wallet_struct.hpp"
#include "./user_block_item/user_block_item.hpp"
#include "./hash_table/hash_table.hpp"

using namespace std;

int main(){

  hash_table<user_block_item> table(1,40);

  user_block_item *block_item=new user_block_item();
  block_item->wallet =new wallet_struct("stratos");
  table.insert(block_item);

  for(int i=0;i<6;i++){
    user_block_item *block_item2=new user_block_item();
    char tmp[51];
    sprintf(tmp, "a%d",i);
    block_item2->wallet =new wallet_struct(tmp);
    table.insert(block_item2);
  }


  table.print_debug();

  table.find("a3")->print_debug();

  return 0;
}
