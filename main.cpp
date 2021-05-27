#include<iostream>
#include<fstream>
#include<cstdlib>
#include<vector>
#include<map>
using namespace std;
#define MIN_BALANCE 500
class InsufficientFunds{};
class Account {
private:
    long accountNumber;
    string firstName;
    string lastName;
    float balance;
    static long nextAccountNumber;
public:
    Account(){}
    Account(string fname, string lname, float balance);
    long getAccountNumber(){return accountNumber;}
    string getFirstName(){return firstName;}
    string getLastName(){return lastName;}
    float getBalance(){return balance;}

    void Deposit(float amount);
    void Withdraw(float amount);
    static void setLastAccountNumber(long accountNumber);
    static long getLastAccountNumber();
    friend ofstream & operator<<(ofstream &ofs,Account &acc);
    friend ifstream & operator>>(ifstream &ifs,Account &acc);
    friend ostream & operator<<(ostream &os,Account &acc);
};

long Account::nextAccountNumber=0;

class Bank {
private:
    map<long, Account> accounts;
public:
    Bank();
    Account openAccount(string fname, string lname, float balance);
    Account balanceEnquiry(long accountNumber);
    Account Deposit(long accountNumber, float amount);
    Account Withdraw(long accountNumber, float amount);
    void closeAccount(long accountNumber);
    void showAllAccounts();
    ~Bank();
};

int main() {
    Bank b;
    Account acc;

    int choice;
    string fname;
    string lname;
    long accountNumber;
    float balance;
    float amount;
    cout << "***Bank System***" << endl;
    do
    {
        cout<<"\n\tSelect one option below ";
        cout<<"\n\t1 Open an Account";
        cout<<"\n\t2 Balance Enquiry";
        cout<<"\n\t3 Deposit";
        cout<<"\n\t4 Withdrawal";
        cout<<"\n\t5 Close an Account";
        cout<<"\n\t6 Show All Accounts";
        cout<<"\n\t7 Quit";
        cout<<"\nEnter your choice: ";
        cin>>choice;
        switch(choice)
        {
            case 1:
                cout<<"Enter First Name: ";
                cin>>fname;
                cout<<"Enter Last Name: ";
                cin>>lname;
                cout<<"Enter the initial Balance: ";
                cin>>balance;
                acc=b.openAccount(fname,lname,balance);
                cout<<endl<<"Account has been created! "<<endl;
                cout<<acc;
                break;
            case 2:
                cout<<"Enter Account Number:";
                cin>>accountNumber;
                acc=b.balanceEnquiry(accountNumber);
                cout<<endl<<"Your Account Details"<<endl;
                cout<<acc;
                break;
            case 3:
                cout<<"Enter Account Number: ";
                cin>>accountNumber;
                cout<<"Enter Balance: ";
                cin>>amount;
                acc=b.Deposit(accountNumber, amount);
                cout<<endl<<"Amount is Deposited"<<endl;
                cout<<acc;
                break;
            case 4:
                cout<<"Enter Account Number:";
                cin>>accountNumber;
                cout<<"Enter Balance:";
                cin>>amount;
                acc=b.Withdraw(accountNumber, amount);
                cout<<endl<<"Amount Withdrawn"<<endl;
                cout<<acc;
                break;
            case 5:
                cout<<"Enter Account Number:";
                cin>>accountNumber;
                b.closeAccount(accountNumber);
                cout<<endl<<"Account is Closed"<<endl;
                cout<<acc;
            case 6:
                b.showAllAccounts();
                break;
            case 7: break;
            default: cout<<"\nEnter correct choice";
                exit(0);
        }
    }while(choice!=7);
    return 0;
}

// Sets account number, first name, last name and initial bank balance.
Account::Account(string fname, string lname, float balance) {
    nextAccountNumber++;
    accountNumber=nextAccountNumber;
    firstName = fname;
    lastName = lname;
    this->balance=balance;
}

// Deposits to bank balance
void Account::Deposit(float amount) {
    balance += amount;
}

// Checks if balance - amount is less than 500 and throws exception, else, balance -= amount
void Account::Withdraw(float amount) {
    if(balance - amount < MIN_BALANCE) {
        throw InsufficientFunds();
    }
    balance -= amount;
}

// Stores last account number
void Account::setLastAccountNumber(long accountNumber) {
    nextAccountNumber = accountNumber;
}

long Account::getLastAccountNumber() {
    return nextAccountNumber;
}


// Writes data to file
ofstream & operator << (ofstream &ofs, Account &acc) {
    ofs << acc.accountNumber << endl;
    ofs << acc.firstName << endl;
    ofs << acc.lastName << endl;
    ofs << acc.balance << endl;
    return ofs;
}

// Reads data from file
ifstream & operator >> (ifstream &ifs,Account &acc) {
    ifs>>acc.accountNumber;
    ifs>>acc.firstName;
    ifs>>acc.lastName;
    ifs>>acc.balance;
    return ifs;
}

// Represents the data
ostream & operator << (ostream &os, Account &acc) {
    os << "First name: " << acc.getFirstName() << endl;
    os << "Last name: " << acc.getLastName() << endl;
    os << "Account Number: " << acc.getAccountNumber() << endl;
    os << "Balance: " << acc.getBalance() << endl;
    return os;
}

Bank::Bank() {
    Account account;

    // Open file if exists
    ifstream infile; infile.open("Bank.data");

    // If file doesn't exist, return.
    if(!infile)
    {
        return;
    }

    // While end of file not reached
    while(!infile.eof())
    {
        infile>>account;
        accounts.insert(pair<long,Account>(account.getAccountNumber(),account));
    }
    Account::setLastAccountNumber(account.getAccountNumber());
    infile.close();
}

// Open Account data insertion
Account Bank::openAccount(string fname,string lname,float balance)
{
    ofstream outfile;
    Account account(fname,lname,balance);
    accounts.insert(pair<long,Account>(account.getAccountNumber(),account));

    outfile.open("Bank.data", ios::trunc);

    map<long,Account>::iterator itr;
    for(itr=accounts.begin();itr!=accounts.end();itr++)
    {
        outfile<<itr->second;
    }
    outfile.close();
    return account;
}

// Return balance
Account Bank::balanceEnquiry(long accountNumber)
{
    map<long,Account>::iterator itr=accounts.find(accountNumber);
    return itr->second;
}

// Deposit into balance
Account Bank::Deposit(long accountNumber,float amount) {
    map<long,Account>::iterator itr=accounts.find(accountNumber);
    itr->second.Deposit(amount);
    return itr->second;
}

// Withdraw balance
Account Bank::Withdraw(long accountNumber,float amount) {
    map<long,Account>::iterator itr=accounts.find(accountNumber);
    itr->second.Withdraw(amount);
    return itr->second;
}

// Deletes all data from account dependant on accountNumber
void Bank::closeAccount(long accountNumber)
{
    map<long,Account>::iterator itr=accounts.find(accountNumber);
    cout<<"Account Deleted"<<itr->second;
    accounts.erase(accountNumber);
}

// Displays all accounts
void Bank::showAllAccounts()
{
    map<long,Account>::iterator itr;
    for(itr=accounts.begin();itr!=accounts.end();itr++)
    {
        cout<<"Account "<<itr->first<<endl<<itr->second<<endl;
    }
}

// Exits
Bank::~Bank() {
    ofstream outfile;
    outfile.open("Bank.data", ios::trunc);

    map<long, Account>::iterator itr;
    for (itr = accounts.begin(); itr != accounts.end(); itr++) {
        outfile << itr->second;
    }
    outfile.close();
}