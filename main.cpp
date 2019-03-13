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

hash_table<user_block_item> *ht_sender;//sender hash table
hash_table<user_block_item> *ht_recver;//recever hash table
hash_table<bitcoin_struct> *ht_bitcoin;//bitcoin hash table
hash_table<transaction_struct> *ht_transactions;//transactions hash table
//(used to find if transaction id is unique in O(1))

unsigned int BITCOIN_VALUE, trans_id_append;//BITCOIN_VALUE initial bitcoin value, counter used in get_valid_transaction_id
char curr_trans_id[51];//the last produced transaction id used in get_valid_transaction_id
time_t latest_time;//latest transaction time (used to reject transactions)

int init_urs(char* file_name);//reads from file and initialises users and bitcoins
int make_transaction(transaction_struct *trans);//takes transaction with valid sender,resever,date,id
//makes the transaction (breaks it into parts if needed). returns 1 if user doents have enought money.returns 0 if succesfull
int init_transactions(char* file_name);//reads from file and makes all transactions
void get_valid_transaction_id();//updates curr_trans_id to the next valid program generated transacion id

// TODO valgrind check
int main(int argc, char *argv[]){
//---------------------------------------------initialise some globals------------------------------------------
  strcpy(curr_trans_id,"");
  struct tm date={0};
  strptime("01-01-1970 00:01", "%d-%m-%Y %H:%M", &date);
  date.tm_isdst = -1;
  latest_time = mktime(&date);
  trans_id_append=0;
  char bitCoinBalancesFile[100],transactionsFile[100];
  unsigned int bytes_per_bucket=0, num_of_buckets_sender=0, num_of_buckets_recver=0,
    num_of_buckets_bitcoin=0, num_of_buckets_transaction=0;

//---------------------------------------------parsing command line argumets------------------------------------------
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
  num_of_buckets_bitcoin=num_of_buckets_sender*5;
  num_of_buckets_transaction=num_of_buckets_sender*10;

  //check for command argument errors
  bool extra=false;//command argument error flag
  if(num_of_buckets_sender<=0 || num_of_buckets_recver<=0 || bytes_per_bucket<30)
    extra=true;
  // optind is for the extra arguments
  for(; optind < argc; optind++){
    printf("!extra arguments: %s\n", argv[optind]);
    extra=true;
  }
  if(extra==true){//exit if arguments are not correct
    cerr << "not correct arguments" << '\n';
    return 2;
  }

  //initialise hash tables
  ht_sender=new hash_table<user_block_item>(num_of_buckets_sender, bytes_per_bucket);
  ht_recver=new hash_table<user_block_item>(num_of_buckets_recver, bytes_per_bucket);
  ht_bitcoin=new hash_table<bitcoin_struct>(num_of_buckets_bitcoin, bytes_per_bucket);
  ht_transactions=new hash_table<transaction_struct>(num_of_buckets_transaction, bytes_per_bucket);

//---------------------------------------------read from files------------------------------------------
  if(init_urs(bitCoinBalancesFile)!=0){//exit if file is not correct format
    cerr << "main bitCoinBalancesFile.txt is not correct" << '\n';
    //TODO free everything
    return 1;
  }
  cout<<"read from file "<<bitCoinBalancesFile<<endl;
  if(init_transactions(transactionsFile)!=0){//exit if file is not correct format
    cerr << "main transactionsFile.txt is not correct" << '\n';
    //TODO free everything
    return 1;
  }
  cout<<"read from file "<<transactionsFile<<endl;

//---------------------------------------------wait for command------------------------------------------
  char *buffer=NULL, *option, *tmp, *id1, *id2;
  size_t len=0;
  unsigned int money;
  while (1) {
    getline(&buffer, &len, stdin);
    buffer[strlen(buffer)-1]='\0';//delete '\n'
    if(strlen(buffer)==0){//if command is 'enter'
      cout<<"!not a valid commad\n";
      continue;
    }

    //get command (first word)
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
      struct tm date={0};
      time_t t;
      if(tmp!=NULL){//time is given
        strptime(tmp, "%d-%m-%Y %H:%M", &date);
        date.tm_isdst = -1;
        t = mktime(&date);
      }
      else{//get current time
        time(&t);
        date = *localtime (&t);
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
      char *buffer2=NULL;
      //get sender or file name
      id1 = strtok(NULL, " ");
      //get recver or NULL
      id2 = strtok(NULL, " ");

      FILE *input_from;
      transaction_struct *new_trans;

      if(id2==NULL){//reqd from file
        input_from=fopen(id1, "r");
        if(input_from==NULL){
          cout<<"can not open transactions file "<<id1<<endl;
          continue;
        }
      }
      else{//read from stdin
        input_from=stdin;
        //make the first transaction
        //chech if urs id exists
        if(ht_sender->find(id1)==nullptr){
          cerr << "main usr1 does not exisxt "<<id1 << '\n';
          return 2;
        }
        if(ht_sender->find(id2)==nullptr){
          cerr << "main usr2 does not exisxt "<<id2 << '\n';
          continue;
        }
        //get money
        char *question_mark;//checking if ; exists at the end
        tmp = strtok(NULL, " ");
        money=strtol(tmp, &question_mark, 10);
        //get time
        tmp = strtok(NULL, "");
        struct tm date={0};
        time_t t;
        if(tmp!=NULL){//time is given
          strptime(tmp, "%d-%m-%Y %H:%M", &date);
          date.tm_isdst = -1;
          t = mktime(&date);
          if(tmp[strlen(tmp)-1]!=';'){
            cerr << "main no ; at the end of line !!first transaction can not be done requestTransactions cancelling" << '\n';
            continue;
          }
        }
        else{//get current time
          if(strcmp(question_mark,";")!=0){//cancel transaction if ; does not exist
            cerr << "main no ; at the end of line !!first transaction can not be done requestTransactions cancelling" << '\n';
            continue;
          }
          time(&t);
          date = *localtime (&t);
        }
        if(difftime(t,latest_time)>0)//if time is acceptable
          latest_time=t;
        else{
          std::cerr << "!wrong time, transaction cancelled" << '\n';
          continue;
        }

        //make new transaction
        get_valid_transaction_id();
        new_trans=new transaction_struct(curr_trans_id);
        new_trans->money=money;
        new_trans->sender=ht_sender->find(id1)->wallet;
        new_trans->recver=ht_sender->find(id2)->wallet;
        new_trans->date_tm=date;
        new_trans->date=t;

        if(make_transaction(new_trans)!=0)
          delete new_trans;
      }

      //read from input_from loop
      while(getline(&buffer2, &len, input_from) != -1) {//read line
         buffer2[strlen(buffer2)-1]='\0';//delete '\n'
         if(strcmp(buffer2,"exitTransactions")==0)//exit requestTransactions mode
            break;

         get_valid_transaction_id();//get valid transaction id
         new_trans=new transaction_struct(curr_trans_id);//make new transaction
         //get sender id
         tmp= strtok(buffer2, " ");
         if(ht_sender->find(tmp)==nullptr){//check if sender_id exists
           cerr << "main sender does not exisxt "<<tmp << '\n';
           delete new_trans;
           continue;
         }
         new_trans->sender=ht_sender->find(tmp)->wallet;
         //get recver id
         tmp= strtok(NULL, " ");
         if(ht_sender->find(tmp)==nullptr){//check if recver_id exists
           cerr << "main recver does not exisxt "<<tmp << '\n';
           delete new_trans;
           continue;
         }
         new_trans->recver=ht_sender->find(tmp)->wallet;
         //get money
         tmp=strtok(NULL, " ");
         char *question_mark;
         new_trans->money=strtol(tmp, &question_mark, 10);
         //get time
         tmp=strtok(NULL,"");
         struct tm date={0};
         time_t t;
         if(tmp!=NULL){//time is given
           strptime(tmp, "%d-%m-%Y %H:%M", &date);
           date.tm_isdst = -1;
           t = mktime(&date);
           if(tmp[strlen(tmp)-1]!=';'){
             cerr << "main no ; at the end of line " << '\n';
             delete new_trans;
             continue;
           }
         }
         else{//get current time
           if(strcmp(question_mark,";")!=0){//cancel transaction if ; does not exist
             cerr << "main no ; at the end of line " << '\n';
             delete new_trans;
             continue;
           }
           time(&t);
           date = *localtime (&t);
         }
         if(difftime(t,latest_time)>=0)//if time is acceptable !!LES OR EQUAL BECAUSE reading from file is verry fast!!
           latest_time=t;
         else{
           std::cerr << "!wrong time, transaction cancelled" << '\n';
           continue;
         }
         new_trans->date_tm=date;
         new_trans->date=t;

         //make transaction
         make_transaction(new_trans);
      }
      if(buffer2!=NULL)
        free(buffer2);
      if(input_from!=stdin)
        fclose(input_from);
    }
    else if(strcmp(option,"findEarnings")==0){
      //get usr
      id1 = strtok(NULL, " ");
      if(ht_recver->find(id1)==nullptr){
        cerr<<"main user does not exist "<<id1<<endl;
        continue;
      }
      //get time1.1
      id2=new char[100];
      tmp=strtok(NULL, " ");
      time_t t1={0},t2={0};
      if(tmp!=NULL){//time1 exists
        strcpy(id2, tmp);
        strcat(id2, " ");
        //get time1.2
        tmp=strtok(NULL, " ");
        if(tmp==NULL){
          delete id2;
          cout<<"wrong time1, command canceled\n";
          continue;
        }
        strcat(id2, tmp);
        //get time2
        tmp=strtok(NULL, "");
        if(tmp==NULL){
          delete id2;
          cout<<"expected time2, command canceled\n";
          continue;
        }
        struct tm date1={0},date2={0};
        strptime(id2, "%d-%m-%Y %H:%M", &date1);
        date1.tm_isdst = -1;
        strptime(tmp, "%d-%m-%Y %H:%M", &date2);
        date2.tm_isdst = -1;
        t1 = mktime(&date1);
        t2 = mktime(&date2);
      }
      delete id2;

      cout<<"user "<<id1<<" has recved a total of "<<
        ht_recver->find(id1)->wallet->recv_money_total<<" money\n";
        ht_recver->find(id1)->print_list(t1,t2);
    }
    else if(strcmp(option,"findPayments")==0){
      //get usr
      id1 = strtok(NULL, " ");
      if(ht_recver->find(id1)==nullptr){
        cerr<<"main user does not exist "<<id1<<endl;
        continue;
      }
      //get time1.1
      id2=new char[100];
      tmp=strtok(NULL, " ");
      time_t t1={0},t2={0};
      if(tmp!=NULL){//time1 exists
        strcpy(id2, tmp);
        strcat(id2, " ");
        //get time1.2
        tmp=strtok(NULL, " ");
        if(tmp==NULL){
          delete id2;
          cout<<"wrong time1, command canceled\n";
          continue;
        }
        strcat(id2, tmp);
        //get time2
        tmp=strtok(NULL, "");
        if(tmp==NULL){
          delete id2;
          cout<<"expected time2, command canceled\n";
          continue;
        }
        struct tm date1={0},date2={0};
        strptime(id2, "%d-%m-%Y %H:%M", &date1);
        date1.tm_isdst = -1;
        strptime(tmp, "%d-%m-%Y %H:%M", &date2);
        date2.tm_isdst = -1;
        t1 = mktime(&date1);
        t2 = mktime(&date2);
      }
      delete id2;

      cout<<"user "<<id1<<" has recved a total of "<<
        ht_sender->find(id1)->wallet->recv_money_total<<" money\n";
        ht_sender->find(id1)->print_list(t1,t2);
    }
    else if(strcmp(option,"walletStatus")==0){
      //get usr_id
      id1 = strtok(NULL, " ");
      if(ht_sender->find(id1)==nullptr){
        cerr<<"main user does not exist "<<id1<<endl;
        continue;
      }
      cout<<"total money "<<ht_sender->find(id1)->wallet->total_money<<endl;
    }
    else if(strcmp(option,"bitCoinStatus")==0){
      //get coin id
      id1 = strtok(NULL, " ");
      if(ht_bitcoin->find(id1)==nullptr){
        cerr<<"main bitcoin does not exist "<<id1<<endl;
        continue;
      }
      cout<<id1<<" "<<ht_bitcoin->find(id1)->succ_trans<<
        " "<<ht_bitcoin->find(id1)->unspent_money()<<endl;
    }
    else if(strcmp(option,"traceCoin")==0){
      //get coin id
      id1 = strtok(NULL, " ");
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
     continue;
    }
  }
  free(buffer);

//---------------------------------------------delete everything------------------------------------------
  ht_sender->delete_wallets();
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
    while (getline(&buffer, &len, fp) != -1){//read line
       buffer[strlen(buffer)-1]='\0';//delete '\n'

       //take usr_id
       usr_id = strtok(buffer, " ");
       if(ht_sender->find(usr_id)!=nullptr){//check if usr exists
         cerr << "init_urs user already exisxts "<<usr_id << '\n';
         return 2;
       }
       //insert new user
       user_block_item *new_usr_item=new user_block_item(),*new_usr_item2=new user_block_item();
       new_usr_item->wallet=new wallet_struct(usr_id);
       new_usr_item2->wallet=new_usr_item->wallet;
       ht_sender->insert(new_usr_item);
       ht_recver->insert(new_usr_item2);

       //take coin_ids, insert new coins and asign them to usr_id
       tmp=usr_id;
       while( tmp != NULL ){
          tmp = strtok(NULL, " ");
          if(tmp==NULL)
            break;
          if(ht_bitcoin->find(tmp)!=nullptr){//check if coin exists
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
  else{//if it needs more than one nodes
    unsigned int rest_money=trans->money;
    char tmp[51],tmp1[20];
    unsigned int i=0;//counting parts (for trans id)

    do{//wile not enought money have been given
      //make new transaction id
      strcpy(tmp, trans->get_id());
      sprintf(tmp1,"part%d",i);
      strcat(tmp, tmp1);
      //make new transaction
      transaction_struct *new_tras=new transaction_struct(tmp);
      new_tras->sender=trans->sender;
      new_tras->recver=trans->recver;
      if(new_tras->sender->get_possible_transaction_money()>=rest_money)//if node has more money than needed
        new_tras->money=rest_money;
      else//if node has less money than needed
        new_tras->money=trans->sender->get_possible_transaction_money();
      new_tras->date=trans->date;
      new_tras->date_tm=trans->date_tm;

      //make transaction
      ht_transactions->insert(new_tras);
      new_tras->recver->recv_money(new_tras->sender->send_money(new_tras));
      ht_sender->find(new_tras->sender->get_id())->insert_first(new_tras);
      ht_recver->find(new_tras->recver->get_id())->insert_first(new_tras);
      //update rest_money
      rest_money-=new_tras->money;
      i++;
      cout<<"transaction "<<new_tras->trans_id<<" done\n";
    }while(rest_money>0);
    delete trans;//because we didnt use that transaction, we alocated new parts
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
    struct tm date={0};
    time_t t;

    while (getline(&buffer, &len, fp) != -1) {//read line
       buffer[strlen(buffer)-1]='\0';//delete '\n'

       //get trans id
       tmp = strtok(buffer, " ");
       if(ht_transactions->find(tmp)!=nullptr){//check if trans_id exists
         cerr << "init_transactions trans_id already exisxts "<<tmp << '\n';
         return 1;
       }
       if(strlen(curr_trans_id)<strlen(tmp))//save the longest transation id (ised in get_valid_transaction_id)
          strcpy(curr_trans_id,tmp);
       //make new transaction
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
       strptime(tmp, "%d-%m-%Y %H:%M", &date);
       date.tm_isdst = -1;
       t = mktime(&date);
       new_trans->date_tm=date;
       new_trans->date=t;
       if(difftime(t,latest_time)>0)//save the latest transaction time (used to validate transactions)
         latest_time=t;
       //make transaction
       if(make_transaction(new_trans)!=0)
         delete new_trans;
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
  sprintf(tmp, "a%d", trans_id_append);//append a%d
  curr_trans_id[strlen(curr_trans_id)-strlen(tmp)]='\0';//delete previous a%d part
  strcat(curr_trans_id,tmp);//append new a%d part
  trans_id_append++;
}
