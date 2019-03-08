#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "./wallet_struct/wallet_struct.hpp"
#include "./user_block_item/user_block_item.hpp"
#include "./hash_table/hash_table.hpp"
#include "./bitcoin_struct/bitcoin_struct.hpp"
#include "./transaction_struct/transaction_struct.hpp"

using namespace std;

hash_table<user_block_item> *ht_sender;
hash_table<user_block_item> *ht_recver;
hash_table<bitcoin_struct> *ht_bitcoin;
hash_table<transaction_struct> *ht_transactions;

int init_urs(char* file_name);

int main(){
  unsigned int num_of_buckets=5, bytes_per_bucket=40;

  ht_sender=new hash_table<user_block_item>(num_of_buckets, bytes_per_bucket);
  ht_recver=new hash_table<user_block_item>(num_of_buckets, bytes_per_bucket);
  ht_bitcoin=new hash_table<bitcoin_struct>(num_of_buckets, bytes_per_bucket);
  ht_transactions=new hash_table<transaction_struct>(num_of_buckets, bytes_per_bucket);

  // user_block_item *item=new user_block_item();
  // item->wallet=new wallet_struct("stratos");
  // table.insert(item);
  //
  // for(int i=0;i<6;i++){
  //   char tmp[51];
  //   sprintf(tmp, "a%d",i);
  //   transaction_struct *block_item2=new transaction_struct(tmp);
  //   table.insert(block_item2);
  // }
  //
  // table.print_debug();
  //
  // table.find("a3")->print_debug();

  init_urs("bitCoinBalancesFile.txt");

  return 0;
}

int init_urs(char* file_name){
  char *buffer=NULL, *usr_id, *tmp;
  unsigned int coin_id;
  size_t len=0;

  //open file
  FILE *fp;
  fp=fopen(file_name, "r");
  if (fp != NULL){
    while (getline(&buffer, &len, fp) != -1) {//read line
       printf("%s", buffer);
       buffer[strlen(buffer)-1]='\0';

       //take usr_id
       usr_id = strtok(buffer, " ");
       printf( "%s\n", usr_id );
       //insert new user
       if(ht_sender->find(usr_id)!=nullptr || ht_recver->find(usr_id)!=nullptr){
         cerr << "init_urs user already exisxts "<<usr_id << '\n';
         return 1;
       }
       user_block_item *item=new user_block_item();
       item->wallet=new wallet_struct(usr_id);
       ht_sender->insert(item);
       ht_recver->insert(item);

       //take coin_id, insert new coins and asign them to usr_id
       tmp=usr_id;
       while( tmp != NULL ) {
          tmp = strtok(NULL, " ");
          if(tmp==NULL)
            break;
          coin_id=strtol(tmp, NULL, 10);
          printf("%d\n", coin_id);
          /////////////incert coin
       }

    }
    free(buffer);
    fclose(fp);
  }
  else {
    std::cerr << "init_urs Unable to open file"<<file_name<<"\n";
    return 1;
  }

  return 0;
}
