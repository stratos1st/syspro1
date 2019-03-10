#include "transaction_struct.hpp"

transaction_struct::transaction_struct(char* id){
  strcpy(trans_id,id);
}

transaction_struct::~transaction_struct(){

}

char* transaction_struct::get_id(){
  return trans_id;
}

void transaction_struct::print_transaction(){
  std::cout<<trans_id<<" "<<sender->get_id()<<" "<<recver->get_id()<<" "<<money<<std::endl;
  // TODO date
}

void transaction_struct::print_debug(){
  std::cout << "transaction_struct id= "<<trans_id<<" money= "<<money << '\n';
}
