# syspro1.1
Efstratios Vamvourellis 1115201600014

To compile: make
to run:     ./main.out -a name -t name2 -v val -s val2 -r val3 -b val4
To clear:   make clean

name:   name of Bit Coin Balance File (string)
name2:  name of Transactions File (string)
val:    starting bitcoin value (unsigned int)
val2:   number of buckets in sender's hash table (unsigned int>0) (h1)
val3:   number of buckets in receiver's hash table (unsigned int>0) (h2)
val4:   bucket size in bytes (unsigned int>=30)

!ALL arguments must be given!
If the user gives more arguments or the arguments don't meet the requirements
the program will terminate. If any of the files doesn't meet the following requirements
the program terminates. 1 all id's (transaction, user, coin) must be different,
2 each transaction must be in the right format, 3 Bit Coin Balance File must be on
a specific format, 4 each transaction must be valid (must follow the rules of requestTransaction).
If a transaction from Transactions File can't be done due to lack of money an error
message will be printed, the transaction will be canceled and the program will
continue normally. For each successful transaction a message with the transactionID will appear.

The hash function used is trial. The size of blocks and number of buckets in
ht_bitcoin and ht_transactions is NOT chosen. These factors can reduce the time
efficiency of the program for large inputs.  

!! IMPORTANT differences !!
1 findPayments and findEarnings format is a little different.
2 If more than one bitcoins are used for a transaction this transaction will be
  broken into smaller ones. This means that even thought the user inputs one transaction,
  more than one might be recorded (with transaction id ID+part+N). This effects
  some of the program's functions. For example: "total number of transactions used"
  returned from bitCoinStatus can deffer from expected.
  To be more accurate, all that depends on the number of tree_nodes used in each
  transaction and not the number of different bitcoins.
3 transactionID is, like any other id value, a string (of size 51) and not an integer.
4 bitcoinID is, like any other id value, a string (of size 51) and not an integer.


ones the program starts it can take the following commands:

  requestTransaction senderID receiverID amount [date time]
    This makes a new transaction. [date time] is optional. If not given the
    transaction will be assigned the current time.
    The transaction will be canceled with an
    error message if one of the following is true: 1 either userID does not
    exist, 2 sender hasn't got enough money, 3 [date time] represents a time
    before the last transaction, 4 senderID and recverID are the same. Otherwise the transaction will be completed and
    the user will be informed with a message informing him with the id that was
    assigned to the transaction.

  requestTransactions senderID receiverID amount [date time];
    Typing this enters the user in requestTransactions mode. [date time] is
    optional. If not given the transaction will be assigned the current time.
    The transaction will be canceled with an
    error message if one of the following is true: 1 either userID does not
    exist, 2 sender hasn't got enough money, 3 [date time] represents a time
    before the last transaction, 4 the line does not end with a ;, , 5 senderID and recverID are the same.
    Otherwise the transaction will be completed and
    the user will be informed with a message informing him with the id that was
    assigned to the transaction. While in requestTransactions mode the user can
    give transactions int the following format: senderID receiverID amount [date time];
    To exit this mode use "exitTransactions".

  requestTransactions inputFile
    This command expects an input file with the following format:
      senderID receiverID amount [date time];
      senderID receiverID amount [date time];
      ....
      senderID receiverID amount [date time];
    All valid transactions within the file will be done. This command follows the
    previously mentioned rules on format and errors. Also it returns an error
    message if the file can't be opened.

  findEarnings walletID [date1 time1 date2 time2]
    !!Unlike the format that was requested this command can not just receive
    time1 time2 or date1 date2 as arguments!!
    This prints all the transactions where walletID is the receiver. [date1
    time1 date2 time2] is optional. If given it will print only the ones between
    date1time1 and date2time2. If walletID doesn't exist an error message will appear.

  findPayments walletID [date1 time1 date2 time2]
    !!Unlike the format that was requested this command can not just receive
    time1 time2 or date1 date2 as arguments!!
    This prints all the transactions where walletID is the sender. [date1 time1
    date2 time2] is optional. If given it will print only the ones between
    date1time1 and date2time2. If walletID doesn't exist an error message will appear.

  walletStatus walletID
    This prints the total money that walletID has. If walletID doesn't exist an
    error message will appear.

  bitCoinStatus bitcoinID
    This prints the bitcoinID, total number of transactions used, unspent value.
    If bitcoinID doesn't exist an error message will appear.

  traceCoin bitcoinID
    This prints all the transactions that bitcoinID was used for. If bitcoinID
    doesn't exist an error message will appear.

  exit
    This command terminates the program.
