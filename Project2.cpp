//----------------------------------------------------
//                     Project 2
//                   Andrew Pauley
//                    Section 401
//----------------------------------------------------

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

using namespace std;

const int MAX_TRANS = 9; // max number of transactions in partial array
const int MAX_ACCTS = 9; // max number of transactions in partial array
const int notFound = -1; // returned by search(), etc
const double MAX_BALANCE = 99999.99; // account balance max (for doDeposit() )
const string adminAcct = "admin"; // current administrator account number
const string adminPin = "9999"; // current administrator pin number

const int mntOptFirst = 1; // first option on Maintenance Menu
const int mntOptLast = 8; // last option on Maintenance Menu
const int custOptFirst = 1; // first option on Customer Menu
const int custOptLast = 4; // last option on Customer Menu

struct account { string number; string name; string PIN; double bal; };

struct transaction { char code; string number; double amount; double bal; };

//--------------------------------------------------------
//                      displayLogo
//--------------------------------------------------------

void displayLogo() {
	cout << "    /  /  +-----------------------------------------+  /   /  /    /  /\n" <<
			"   /  /   |                                         | /   /  /    /  /\n" <<
			"  /  +----+                 APP BANK                +----+  /    /  /\n" <<
			" /    |                                                  | /    /  /\n" <<
			"+----+                    Andrew Pauley                  +----+   /\n" <<
			"|                                                             |  /\n" <<
			"|                                                             | /\n"
			"+-------------------------------------------------------------+\n";
}

//--------------------------------------------------------
//                       startup
//--------------------------------------------------------

bool startup(account acct[], int & num) {
	ifstream f;
	f.open("acctsIn.txt");
	if (f.fail()) {
		cout << "fail";
		return false;
	}
	else {
		f >> num;
		for (int i = 0; i < num; i++) {
			f >> acct[i].number >> acct[i].bal >> acct[i].PIN;
			if (f.peek() == ' ') f.ignore();
			getline(f, acct[i].name);
			//figure out how to get customer name with space
		}
	}
	f.close();
	return true;
}

//--------------------------------------------------------
//                       search
//--------------------------------------------------------

int search(account acct[], int num, string acctNum) {
	int found = -1;
	for (int i = 0; i < num; i++) {
		if (acct[i].number == acctNum) {
			found = i;
		}
	}
	return found;
}

//--------------------------------------------------------
//                       addTrans
//--------------------------------------------------------

void addTrans(char code, string acctNum, double amount, double newBal, transaction tran[], int & num) {
	if (num < MAX_TRANS) {
		tran[num].code = code;
		tran[num].number = acctNum;
		tran[num].amount = amount;
		tran[num].bal = newBal;
		num++;
	}
	else {
		cout << "No more transactions can be added!";
	}
}

//--------------------------------------------------------
//                    getMaintOption
//--------------------------------------------------------

int getMaintOption() {
	int ans;
	cout << 
		"------------------- MAINTENANCE MENU -------------------\n" <<
		"1. Add new account    5. Print cuurent accounts         \n" <<
		"2. Remove account     6. Print current transactions     \n" <<
		"3. Shut down ATM      7. Sort Accounts by Account Number\n" <<
		"4. Exit maintenance   8. Sort Account by Name           \n" <<
		"Enter option: ";
	cin >> ans;
	while (ans < mntOptFirst || ans > mntOptLast) {
		cout << "Invalid entry." << endl << "Enter option (1-8):";
		cin >> ans;
	}
	return ans;
}

//--------------------------------------------------------
//                     askAcctData
//--------------------------------------------------------

void askAcctData(account & acct) {
	cout << "Enter Account Number: ";
	cin >> acct.number;
	cout << "Enter Account Name: ";
	cin >> acct.name;
	if (cin.peek() == ' ') cin.ignore();
	getline(cin, acct.name);
	cout << "Enter PIN: ";
	cin >> acct.PIN;
	cout << "Enter Balance: ";
	cin >> acct.bal;
}

//--------------------------------------------------------
//                        addAcct
//--------------------------------------------------------

void addAcct(account accts[], int & aNum, transaction trans[], int tNum) {
	if (aNum >= MAX_ACCTS) {
		cout << "No room to add an account." << endl;
		return;
	}
	if (tNum >= MAX_TRANS) {
		cout << "No more transactions may be added" << endl;
		return;
	}
	askAcctData(accts[aNum]);
	cout << "New account added!" << endl;
	addTrans('A', accts[aNum].number, 0, 0, trans, tNum);
	aNum++;

}

//--------------------------------------------------------
//                       removeAcct
//--------------------------------------------------------

void removeAcct(account acct[], int & aNum, transaction trans[], int tNum) {
	string ans;
	if (tNum >= MAX_TRANS) {
		cout << "No more transactions may be added!" << endl;
		return;
	}
	else {
		cout << "Enter account number to remove: ";
		cin >> ans;
		if (search(acct, aNum, ans) == -1) {
			cout << "No account with account number " << ans << " found!" << endl;
		}
		else {
			addTrans('R', ans, acct[search(acct, aNum, ans)].bal, 0, trans, tNum);
			for (int i = search(acct, aNum, ans); i < aNum-1; i++) {
				acct[i] = acct[i + 1];
			}
			aNum--;
			cout << "Account removed!" << endl;
		}
	}

}

//--------------------------------------------------------
//                       printAccts
//--------------------------------------------------------

void printAccts(account accts[], int num) {
	cout << "--------------------------------------------------------------\n" <<
			"Account Num  Balance  PIN  Name                               \n" <<
			"------------ -------- ---- -----------------------------------\n";
	for (int i = 0; i < num; i++) {
		cout << setw(12) << left << accts[i].number << setw(9) << right << fixed << setprecision(2) << accts[i].bal << setw(5) << accts[i].PIN << ' ' << setw(30) << left << accts[i].name << endl;
	}
	cout << "--------------------------------------------------------------\n";
	cout << "Number of Accounts = " << num << endl;
}

//--------------------------------------------------------
//                       printTrans
//--------------------------------------------------------

void printTrans(transaction trans[], int num) {
	cout << "-------------------------------------------------------------\n" <<
		"Account Num  Trans Trans Amt Balance      \n" <<
		"------------ ----- --------- --------     \n";
	for (int i = 0; i < num; i++) {
		cout << setw(12) << left << trans[i].number << setw(1) << trans[i].code << setw(9) << right << trans[i].amount << setw(9) << trans[i].bal;
	}
	cout << "-------------------------------------------------------------\n";
	cout << "Number of Transactions = " << num << endl;
}

//--------------------------------------------------------
//                       sortByName
//--------------------------------------------------------

void sortByName(account accts[], int num) {
	for (int i = 0; i < num - 1; i++) {
		for (int j = i + 1; j < num; j++) {
			if (accts[i].name > accts[j].name) {
				account t = accts[i];
				accts[i] = accts[j];
				accts[j] = t;

			}
		}
	}
}

//--------------------------------------------------------
//                       sortByNum
//--------------------------------------------------------

void sortByNum(account accts[], int num) {
	for (int i = 0; i < num - 1; i++) {
		for (int j = i + 1; j < num; j++) {
			if (accts[i].number > accts[j].number) {
				account t = accts[i];
				accts[i] = accts[j];
				accts[j] = t;
			}
		}
	}
}

//--------------------------------------------------------
//                       doMaintenance
//--------------------------------------------------------

bool doMaintenance(account accts[], int& aNum, transaction trans[], int tNum) {
	int option = getMaintOption();
	while (!(option == 3 || option == 4)) {
		if (option == 1) { //add new account
			addAcct(accts, aNum, trans, tNum);
		}
		else if (option == 2) { //remove account
			removeAcct(accts, aNum, trans, tNum);
		}
		else if (option == 5) { // print current accounts
			printAccts(accts, aNum);
		}
		else if (option == 6) { // print current transactions
			printTrans(trans, tNum);
		}
		else if (option == 7) { // sort by number
			sortByNum(accts, aNum);
		}
		else if (option == 8) { // sort by name
			sortByName(accts, aNum);
			
		}
		option = getMaintOption();
	}
	if (option == 3) {
		return true;
	}
	else {
		return false;
	}
}

//--------------------------------------------------------
//                      getCustOption
//--------------------------------------------------------

int getCustOption() {
	int ans;
	cout << "---- Transaction Menu ----\n" <<
		"1. Deposit Funds\n" <<
		"2. Withdraw Funds\n" <<
		"3. Balance Inquiry\n" <<
		"4. Cancel transaction\n";
	cout << "Enter option: ";
	cin >> ans;
	while (ans < custOptFirst || ans > custOptLast) {
		cout << "Invalid option. (1 - 4)";
		cin >> ans;
	}
	return ans;
}

//--------------------------------------------------------
//                       doWithdrawal
//--------------------------------------------------------

void doWithdrawal(account & acct, transaction trans[], int num) {
	double amount;
	cout << "Current Balance: $" << acct.bal << endl;
	cout << "Enter withdrawal amount: ";
	cin >> amount;
	while (amount < 0 || amount > acct.bal) {
		cout << "Invalid amount. Enter a number 0.00 to " << acct.bal << ": ";
		cin >> amount;
	}
	acct.bal = acct.bal - amount;
	cout << "Your new balance is: " << acct.bal << endl;
	addTrans('W', acct.number, amount, acct.bal, trans, num);
}

//--------------------------------------------------------
//                       doDeposit
//--------------------------------------------------------

void doDeposit(account& acct, transaction trans[], int num) {
	double amount;
	cout << "Current Balance: $" << acct.bal << endl;
	cout << "Enter deposit amount: ";
	cin >> amount;
	while (amount < 0 || amount >(MAX_BALANCE - acct.bal)) {
		cout << "Invalid amount. Enter a number between 0.00 to " << (MAX_BALANCE - acct.bal) << ": ";
		cin >> amount;
	}
	acct.bal = acct.bal + amount;
	cout << "Your new balance is: " << acct.bal << endl;
	addTrans('D', acct.number, amount, acct.bal, trans, num);
}

//--------------------------------------------------------
//                       serveCustomer
//--------------------------------------------------------

void serveCustomer(account & acct, transaction trans[], int num) {
	int option = getCustOption();
	if (option == 1) {
		doDeposit(acct, trans, num);
	}
	else if (option == 2) {
		doWithdrawal(acct, trans, num);
	}
	else if (option == 3) {
		cout << "Your current balance is: " << acct.bal << endl;
		addTrans('I', acct.number, 0, acct.bal, trans, num);
	}
	else if (option == 4) {
		cout << "Transaction canceled!" << endl;
	}
}

//--------------------------------------------------------
//                       writeAccounts
//--------------------------------------------------------

void writeAccounts(account accts[], int num) {
	ofstream f;
	f.open("acctsOut.txt");
	if (f.fail()) {
		cout << "Unable to open output file acctsOut.txt!";
		return;
	}
	f << num << endl;
	for (int i = 0; i < num; i++) {
		f << accts[i].number << " " << accts[i].bal << " " << accts[i].PIN << " " << accts[i].name << endl;
	}
	f.close();
}

//--------------------------------------------------------
//                       writeTrans
//--------------------------------------------------------

void writeTrans(transaction trans[], int num) {
	ofstream f;
	f.open("transactions.txt");
	if (f.fail()) {
		cout << "Unable to open transactions.txt!";
		return;
	}
	f << num << endl;
	for (int i = 0; i < num; i++) {
		f << trans[i].code << " " << trans[i].number << " " << trans[i].amount << " " << trans[i].bal << endl;
	}
	f.close();
}

//--------------------------------------------------------
//                       doShutdown
//--------------------------------------------------------

void doShutdown(account accts[], int aNum, transaction trans[], int tNum) {
	writeAccounts(accts, aNum);
	writeTrans(trans, tNum);
}

//--------------------------------------------------------
//                       getAcct
//--------------------------------------------------------

int getAcct(account accts[], int num) {
	string numb, pinnum;
	displayLogo();
	cout << "Enter account number: ";
	cin >> numb;
	cout << "Enter PIN: ";
	cin >> pinnum;
	int index = search(accts, num, numb);
	if (numb == adminAcct) {
		if (pinnum == adminPin) {
			return -2;
		}
	}
	if (index == -1) {
		return -1;
	}
	else {
		if (pinnum == accts[index].PIN) {
			return index;
		}
		else {
			return -1;
		}
	}

}

int main() {
	account accountArray[MAX_ACCTS];
	int numAcct = 0;
	transaction transArray[MAX_TRANS];
	int numTrans = 0;
	if (!startup(accountArray, numAcct)) {
		cout << "ATM shut down" << endl;
		return 0;
	}
	int returned = getAcct(accountArray, numAcct);
	while (returned != -3) {
		if (returned == -2) {
			bool done = doMaintenance(accountArray, numAcct, transArray, numTrans);
			if (done == true) {
				doShutdown(accountArray, numAcct, transArray, numTrans);
				returned = -3;
			}
		}
		if (numTrans >= MAX_TRANS) {
			cout << "Sorry, this ATM is unavailable. Please check back later." << endl;
			return 0;
		}
		if (returned == -1) {
			cout << "Invalid account number and/or pin!" << endl;
		}
		else if (returned >= 0) {
			serveCustomer(accountArray[returned], transArray, numTrans);
		}
		returned = getAcct(accountArray, numAcct);
		cout << "Testing: " << transArray[0].number;
	}
}