#include "transaction_struct.hpp"

transaction_struct::transaction_struct(char* id){
  strcpy(trans_id,id);
  date_tm={0};
}

transaction_struct::~transaction_struct(){
}

char* transaction_struct::get_id(){
  return trans_id;
}

void transaction_struct::print_transaction(){
  char tmp[50];
  strftime(tmp,50, "%d-%m-%Y %H:%M", &date_tm);
  std::cout<<trans_id<<" "<<sender->get_id()<<" "<<recver->get_id()
    <<" "<<money<<" "<<tmp<<std::endl;
}

void transaction_struct::print_debug(){
  std::cout << "transaction_struct id= "<<trans_id<<" money= "<<money << '\n';
}
