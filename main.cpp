#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
int make_transaction(transaction_struct *trans);
int init_transactions(char* file_name);
void get_valid_transaction_id();

unsigned int BITCOIN_VALUE, trans_id_append;
char curr_trans_id[51];
time_t latest_time;

// TODO fix byte size
// TODO valgrind check
int main(int argc, char *argv[]){
  strcpy(curr_trans_id,"");
  struct tm *date=new struct tm();
  strptime("01-01-1970 00:01", "%d-%m-%Y %H:%M", date);
  date->tm_isdst = -1;
  latest_time = mktime(date);
  delete date;
  trans_id_append=0;
  char bitCoinBalancesFile[100],transactionsFile[100];
  unsigned int bytes_per_bucket, num_of_buckets_sender, num_of_buckets_recver,
    num_of_buckets_bitcoin=10, num_of_buckets_transaction=10;

  //parsing command line argumets
  int opt;
  while((opt = getopt(argc, argv, "a:t:v:s:r:b:")) != -1) {
    switch(opt){
      case 'a':
        strcpy(bitCoinBalancesFile,optarg);
        //cout<<"1"<<optarg<<"1\n";
        break;
      case 't':
        strcpy(transactionsFile,optarg);
        //cout<<"2"<<optarg<<"2\n";
        break;
      case 'v':
        BITCOIN_VALUE = strtol(optarg, NULL, 10);
        //cout<<BITCOIN_VALUE<<"\n";
        break;
      case 's':
        num_of_buckets_sender = strtol(optarg, NULL, 10);
        //cout<<num_of_buckets_sender<<"\n";
        break;
      case 'r':
        num_of_buckets_recver = strtol(optarg, NULL, 10);
        //cout<<num_of_buckets_recver<<"\n";
        break;
      case 'b':
        bytes_per_bucket = strtol(optarg, NULL, 10);
        //cout<<bytes_per_bucket<<"\n";
        break;
    }
  }
  // TODO check for correctness of arguments
  // optind is for the extra arguments
  bool extra=false;
  for(; optind < argc; optind++){
    printf("!extra arguments: %s\n", argv[optind]);
    extra=true;
  }
  if(extra==true){
    cerr << "not correct arguments" << '\n';
    return 2;
  }

  //initialise hash tables
  ht_sender=new hash_table<user_block_item>(num_of_buckets_sender, bytes_per_bucket);
  ht_recver=new hash_table<user_block_item>(num_of_buckets_recver, bytes_per_bucket);
  ht_bitcoin=new hash_table<bitcoin_struct>(num_of_buckets_bitcoin, bytes_per_bucket);
  ht_transactions=new hash_table<transaction_struct>(num_of_buckets_transaction, bytes_per_bucket);

  //read usr and bitcoin from file
  if(init_urs(bitCoinBalancesFile)!=0){
    cerr << "main bitCoinBalancesFile.txt is not correct" << '\n';
    return 1;
  }
  cout<<"read from file "<<bitCoinBalancesFile<<endl;

  if(init_transactions(transactionsFile)!=0){
    cerr << "main transactionsFile.txt is not correct" << '\n';
    return 1;
  }
  cout<<"read from file "<<transactionsFile<<endl;


  char *buffer=NULL, *option, *tmp, *id1, *id2;
  size_t len=0;
  unsigned int money;
  while (1) {
    getline(&buffer, &len, stdin);
    buffer[strlen(buffer)-1]='\0';

    //get first word
    option = strtok(buffer, " ");

    if(strcmp(option,"requestTransaction")==0){
      //get sender
      id1 = strtok(NULL, " ");
      if(ht_sender->find(id1)==nullptr){
        cerr << "main usr1 does not exisxt "<<id1 << '\n';
        return 2;
      }
      //get recver
      id2 = strtok(NULL, " ");
      if(ht_sender->find(id2)==nullptr){
        cerr << "main usr2 does not exisxt "<<id2 << '\n';
        continue;
      }
      //get money
      tmp = strtok(NULL, " ");
      money=strtol(tmp, NULL, 10);
      //get time
      tmp = strtok(NULL, "");
      struct tm *date;
      time_t t;
      if(tmp!=NULL){//time is given
        date=new struct tm();
        strptime(tmp, "%d-%m-%Y %H:%M", date);
        date->tm_isdst = -1;
        t = mktime(date);
      }
      else{//get current time
        time(&t);
        date = localtime (&t);
      }
      if(difftime(t,latest_time)>0)//if time is acceptable
        latest_time=t;
      else{
        std::cerr << "!wrong time, transaction cancelled" << '\n';
        continue;
      }

      //make new transaction
      get_valid_transaction_id();
      transaction_struct *new_trans=new transaction_struct(curr_trans_id);
      new_trans->money=money;
      new_trans->sender=ht_sender->find(id1)->wallet;
      new_trans->recver=ht_sender->find(id2)->wallet;
      new_trans->date_tm=date;
      new_trans->date=t;

      if(make_transaction(new_trans)!=0)
        delete new_trans;

    }
    else if(strcmp(option,"requestTransactions")==0){
      // TODO requestTransactions
      //get sender or file name
      id1 = strtok(NULL, " ");
      //get recver or NULL
      id2 = strtok(NULL, " ");

      FILE *out;

      if(id2==NULL){
        out=fopen(id1, "r");
        if(out==NULL){
          cout<<"cant open transactions file "<<id1<<endl;
          continue;
        }
      }
      else{
        out=stdout;
        if(ht_sender->find(id1)==nullptr){
          cerr << "main usr1 does not exisxt "<<id1 << '\n';
          return 2;
        }

        if(ht_sender->find(id2)==nullptr){
          cerr << "main usr2 does not exisxt "<<id2 << '\n';
          continue;
        }
        //get money
        tmp = strtok(NULL, " ");
        money=strtol(tmp, NULL, 10);
        //get time
        tmp = strtok(NULL, "");
        struct tm *date;
        time_t t;
        if(tmp!=NULL){//time is given
          date=new struct tm();
          strptime(tmp, "%d-%m-%Y %H:%M;", date);
          date->tm_isdst = -1;
          t = mktime(date);
        }
        else{//get current time
          time(&t);
          date = localtime (&t);
        }
        if(difftime(t,latest_time)>0)//if time is acceptable
          latest_time=t;
        else{
          std::cerr << "!wrong time, transaction cancelled" << '\n';
          continue;
        }

        //make new transaction
        get_valid_transaction_id();
        transaction_struct *new_trans=new transaction_struct(curr_trans_id);
        new_trans->money=money;
        new_trans->sender=ht_sender->find(id1)->wallet;
        new_trans->recver=ht_sender->find(id2)->wallet;
        new_trans->date_tm=date;
        new_trans->date=t;

        if(make_transaction(new_trans)!=0)
          delete new_trans;
      }

      init_transactions(out);

    }
    else if(strcmp(option,"findEarnings")==0){
      id1 = strtok(NULL, " ");
      //printf( "%s\n", id1 );
      if(ht_recver->find(id1)==nullptr){
        cerr<<"main user does not exist "<<id1<<endl;
        continue;
      }
      cout<<"user "<<id1<<" has recved a total of "<<
        ht_recver->find(id1)->wallet->recv_money_total<<" money\n";
        ht_recver->find(id1)->print_list();
    }
    else if(strcmp(option,"findPayments")==0){// TODO date
      id1 = strtok(NULL, " ");
      //printf( "%s\n", id1 );
      if(ht_sender->find(id1)==nullptr){
        cerr<<"main user does not exist "<<id1<<endl;
        continue;
      }
      cout<<"user "<<id1<<" has sent a total of "<<
        ht_sender->find(id1)->wallet->send_money_total<<" money\n";
        ht_sender->find(id1)->print_list();
    }
    else if(strcmp(option,"walletStatus")==0){// TODO date
      id1 = strtok(NULL, " ");
      //printf( "%s\n", id1 );
      if(ht_sender->find(id1)==nullptr){
        cerr<<"main user does not exist "<<id1<<endl;
        continue;
      }
      cout<<"total money "<<ht_sender->find(id1)->wallet->total_money<<endl;
    }
    else if(strcmp(option,"bitCoinStatus")==0){
      id1 = strtok(NULL, " ");
      //printf( "%s\n", id1 );
      if(ht_bitcoin->find(id1)==nullptr){
        cerr<<"main bitcoin does not exist "<<id1<<endl;
        continue;
      }
      cout<<id1<<" "<<ht_bitcoin->find(id1)->succ_trans<<
        " "<<ht_bitcoin->find(id1)->unspent_money()<<endl;
    }
    else if(strcmp(option,"traceCoin")==0){
      id1 = strtok(NULL, " ");
      //printf( "%s\n", id1 );
      if(ht_bitcoin->find(id1)==nullptr){
        cerr<<"main bitcoin does not exist "<<id1<<endl;
        continue;
      }
      ht_bitcoin->find(id1)->print_history();
    }
    else if(strcmp(option,"exit")==0){
    cout<<"exiting\n";
     break;
    }
    else{
     cout<<"!not a valid commad\n";
    }
  }

  // char a[51];
  // do{
  //   cin>>a;
  //   if(ht_sender->find(a)!=nullptr)
  //     ht_sender->find(a)->print_debug();
  // }while(strcmp(a,"aaa")!=0);

  delete ht_sender;
  delete ht_recver;
  delete ht_bitcoin;
  delete ht_transactions;

  return 0;
}

int init_urs(char* file_name){
  char *buffer=NULL, *usr_id, *tmp;
  size_t len=0;

  //open file
  FILE *fp;
  fp=fopen(file_name, "r");
  if (fp != NULL){
    while (getline(&buffer, &len, fp) != -1) {//read line
      // printf("%s", buffer);
       buffer[strlen(buffer)-1]='\0';

       //take usr_id
       usr_id = strtok(buffer, " ");
       //printf( "%s\n", usr_id );
       //check if usr exists
       if(ht_sender->find(usr_id)!=nullptr || ht_recver->find(usr_id)!=nullptr){
         cerr << "init_urs user already exisxts "<<usr_id << '\n';
         return 2;
       }
       //insert new user
       user_block_item *new_usr_item=new user_block_item(),*new_usr_item2=new user_block_item();
       new_usr_item->wallet=new wallet_struct(usr_id);
       new_usr_item2->wallet=new_usr_item->wallet;
       ht_sender->insert(new_usr_item);
       ht_recver->insert(new_usr_item2);

       //take coin_id, insert new coins and asign them to usr_id
       tmp=usr_id;
       while( tmp != NULL ) {
          tmp = strtok(NULL, " ");
          if(tmp==NULL)
            break;
          //printf("%s\n",tmp);
          //this is for int coin_id
          //coin_id=strtol(tmp, NULL, 10);
          //printf("%d\n", coin_id);

          //check if coin exists
          if(ht_bitcoin->find(tmp)!=nullptr){
            cerr << "init_urs bitcoin already exisxts "<<tmp << '\n';
            return 2;
          }
          //insert new coin
          bitcoin_struct* new_coin_item=new bitcoin_struct(tmp, BITCOIN_VALUE);
          ht_bitcoin->insert(new_coin_item);
          //coin belongs to usr
          new_usr_item->wallet->add_initial_coin(new_coin_item);
       }

    }
    free(buffer);
    fclose(fp);
  }
  else {
    cerr << "init_urs Unable to open file"<<file_name<<"\n";
    return 1;
  }
  return 0;
}

int make_transaction(transaction_struct *trans){
  if(!trans->sender->is_transaction_possible(trans)){//if it can not be done
    cout<<"not enought money, transaction "<<trans->trans_id<<" can not be done\n";
    return 1;
  }

  if(trans->sender->get_possible_transaction_money()>=trans->money){//if it can be done with only one node
    ht_transactions->insert(trans);
    trans->recver->recv_money(trans->sender->send_money(trans));
    ht_sender->find(trans->sender->get_id())->insert_first(trans);
    ht_recver->find(trans->recver->get_id())->insert_first(trans);
    cout<<"transaction "<<trans->trans_id<<" done\n";
  }
  else{
    unsigned int rest_money=trans->money;
    char tmp[51],tmp1[20];
    unsigned int i=0;
    do{
      strcpy(tmp, trans->get_id());
      sprintf(tmp1,"part%d",i);
      strcat(tmp, tmp1);
      transaction_struct *new_tras=new transaction_struct(tmp);
      new_tras->sender=trans->sender;
      new_tras->recver=trans->recver;
      if(new_tras->sender->get_possible_transaction_money()>=rest_money)
        new_tras->money=rest_money;
      else
        new_tras->money=trans->sender->get_possible_transaction_money();
      new_tras->date=trans->date;
      new_tras->date_tm=trans->date_tm;

      ht_transactions->insert(new_tras);
      new_tras->recver->recv_money(new_tras->sender->send_money(new_tras));
      ht_sender->find(new_tras->sender->get_id())->insert_first(new_tras);
      ht_recver->find(new_tras->recver->get_id())->insert_first(new_tras);
      rest_money-=new_tras->money;
      i++;
      cout<<"transaction "<<new_tras->trans_id<<" done\n";
    }while(rest_money>0);
    trans->date_tm=nullptr;
    delete trans;
  }

  return 0;
}

int init_transactions(char* file_name){
  char *buffer=NULL, *tmp;
  size_t len=0;

  //open file
  FILE *fp;
  fp=fopen(file_name, "r");
  if (fp != NULL){
    struct tm *date;
    time_t t;
    while (getline(&buffer, &len, fp) != -1) {//read line
       //printf("%s", buffer);
       buffer[strlen(buffer)-1]='\0';

       //make new transaction
       //get trans id
       tmp = strtok(buffer, " ");
       if(ht_transactions->find(tmp)!=nullptr){//check if trans_id exists
         cerr << "init_transactions trans_id already exisxts "<<tmp << '\n';
         return 1;
       }
       if(strlen(curr_trans_id)<strlen(tmp))
          strcpy(curr_trans_id,tmp);
       transaction_struct *new_trans=new transaction_struct(tmp);
       //get sender id
       tmp= strtok(NULL, " ");
       if(ht_sender->find(tmp)==nullptr){//check if sender_id exists
         cerr << "init_transactions sender does not exisxt "<<tmp << '\n';
         delete new_trans;
         return 2;
       }
       new_trans->sender=ht_sender->find(tmp)->wallet;
       //get recver id
       tmp= strtok(NULL, " ");
       if(ht_sender->find(tmp)==nullptr){//check if recver_id exists
         cerr << "init_transactions recver does not exisxt "<<tmp << '\n';
         delete new_trans;
         return 2;
       }
       new_trans->recver=ht_sender->find(tmp)->wallet;
       //get money
       tmp=strtok(NULL, " ");
       new_trans->money=strtol(tmp, NULL, 10);
       //get time
       tmp = strtok(NULL, "");
       date=new struct tm();
       strptime(tmp, "%d-%m-%Y %H:%M", date);
       date->tm_isdst = -1;
       t = mktime(date);
       new_trans->date_tm=date;
       new_trans->date=t;
       if(difftime(t,latest_time)>0)
         latest_time=t;

       make_transaction(new_trans);
    }
    free(buffer);
    fclose(fp);
  }
  else {
    cerr << "init_transactions unable to open file"<<file_name<<"\n";
    return 1;
  }

  return 0;
}

void get_valid_transaction_id(){
  char tmp[51];
  sprintf(tmp, "%d", trans_id_append);
  strcat(curr_trans_id,tmp);
  trans_id_append++;
}
