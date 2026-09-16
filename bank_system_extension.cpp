#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<iomanip>

using namespace std;
const string clientsFileName = "Clients.txt";

void showMainMenue();

enum enMainMenueOptions
{
	eListClients = 1, eAddClients = 2,
	eDeleteClient = 3, eUpdateClient = 4,
	eFindClient = 5, eShowTansactionsMenue = 6, eExit = 7
};

enum enTransactionsMenueOptions
{
    eDeposit = 1, eWithDraw = 2, eTotalBalances = 3,
    eShowMainMenue = 4 	
};

	struct stClient
{
	string accountNumber = "";
	string pinCode = "";
	string name = "";
	string phone = "";
	double accountBalance = 0.000000;
	bool markedForDelete = false;
};


short readMainMenueOption()
{
	short choose = 0;
	do {
		cout << "\nChoose what do you want to do? [1 to 7]? ";
		cin >> choose;
	} while ((choose < 1) || (choose > 7));
	return choose;
}

string readAccountNumber()
{
	string accountNumber = "";
	cout << "Please enter account number? ";
	getline(cin >> ws, accountNumber);
	return accountNumber;
}

vector<string> splitString(string line, string seperator = "#//#")
{
	vector<string> vClient;
	string word = "";
	int pos = 0;
	while ((pos = line.find(seperator)) != std::string::npos)
	{
		word = line.substr(0, pos);
		if (word != "")
			vClient.push_back(word);
		line.erase(0, pos + seperator.length());
	}
	vClient.push_back(line);
	return vClient;
}

string convertRecordToLine(const stClient& Client, string seperator = "#//#")
{
	string sClient = "";
	sClient += Client.accountNumber + seperator;
	sClient += Client.pinCode + seperator;
	sClient += Client.name + seperator;
	sClient += Client.phone + seperator;
	sClient += to_string(Client.accountBalance);
	return sClient;
}

stClient convertClientDataToRecord(const vector<string>& vClient)
{
	stClient Client;
	Client.accountNumber = vClient[0];
	Client.pinCode = vClient[1];
	Client.name = vClient[2];
	Client.phone = vClient[3];
	Client.accountBalance = stod(vClient[4]);
	return Client;
}

vector<stClient> loadClientsDataFromFile(const string& fileName, string seperator = "#//#")
{
	vector<stClient> vClients;

	fstream MyFile;
	MyFile.open(fileName, ios::in);//read mode
	if (MyFile.is_open())
	{
		vector<string> vClient;
		stClient Client;
		string line = "";
		while (getline(MyFile, line))
		{
			vClient = splitString(line);
			Client = convertClientDataToRecord(vClient);
			vClients.push_back(Client);
		}
	}
	MyFile.close();
	return vClients;
}

void saveClientsDataToFile(const string& fileName, vector<stClient>& vClients)
{
	fstream MyFile;
	MyFile.open(fileName, ios::out);//write mode
	if (MyFile.is_open())
	{
		for (stClient c : vClients)
		{
			if (c.markedForDelete == false)
				MyFile << convertRecordToLine(c) << endl;
		}
		MyFile.close();
	}
}

void printClientCard(const stClient& Client)
{
	cout << "\nThe following are the client info :";
	cout << "\n_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ __ _ _ _ _ _ _  _ __ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _\n";
	cout << "Account number:" << Client.accountNumber << "\n";
	cout << "Pin code  :" << Client.pinCode << "\n";
	cout << "Name      :" << Client.name << "\n";
	cout << "Phone     :" << Client.phone << "\n";
	cout << "Account Balance:" << Client.accountBalance << "\n";
	cout << "\n_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ __ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _\n";
}

void goBackToMainMenue()
{
	cout << "Press any key to go back to Main Menue...";
	system("pause>0");
	showMainMenue();
}

void showAllClientsScreen()
{
	vector<stClient> vClients = loadClientsDataFromFile(clientsFileName);
	cout << "\n\t\t\t\t\t" << "Clients list (" << vClients.size() << ") " << "client(s).\n";
	cout << "\n_______________________________________________________________________________________________\n";
	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(10) << "Pin code";
	cout << "| " << left << setw(40) << "Client name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n_______________________________________________________________________________________________\n";
	if (vClients.size() == 0)
		cout << "\t\t\t\tNo clients available in the system!";
	else
	{
		for (stClient& client : vClients)
		{
			cout << "| " << left << setw(15) << client.accountNumber;
			cout << "| " << left << setw(10) << client.pinCode;
			cout << "| " << left << setw(40) << client.name;
			cout << "| " << left << setw(12) << client.phone;
			cout << "| " << left << setw(12) << client.accountBalance;
			cout << endl;
		}
	}
	cout << "\n\n_______________________________________________________________________________________________\n";
}

void addDataLineToFile(const string& fileName, string line)
{
	vector<stClient> vClients = loadClientsDataFromFile(fileName);
	fstream MyFile;
	MyFile.open(fileName, ios::out);//write mode
	if (MyFile.is_open())
	{
		for (stClient& c : vClients)
		{
			MyFile << convertRecordToLine(c) << endl;
		}
		MyFile << line << endl;
	}
	MyFile.close();
}

bool clientExistsByAccountNumber(const string& accountNumber, const string& fileName)
{
	vector<stClient> vClients;
	fstream MyFile;
	MyFile.open(fileName, ios::in);//read mode

	if (MyFile.is_open())
	{
		string line = "";
		stClient Client;
		vector<string> vClient;
		while (getline(MyFile, line))
		{
			vClient = splitString(line);
			Client = convertClientDataToRecord(vClient);
			if (Client.accountNumber == accountNumber)
			{
				MyFile.close();
				return true;
			}
			vClients.push_back(Client);
		}
		MyFile.close();
	}
	return false;
}

stClient readNewClient()
{
	stClient Client;
	cout << "Enter account number? ";
	//usage std::ws will extract all the white spaces character
	getline(cin >> ws, Client.accountNumber);
	while (clientExistsByAccountNumber(Client.accountNumber, clientsFileName))
	{
		cout << " Client with account number [" << Client.accountNumber << "] already exists,Enter another account number? ";
		getline(cin >> ws, Client.accountNumber);
	}
	cout << "\nEnter pinCode: ";
	getline(cin, Client.pinCode);
	cout << "\nEnter name: ";
	getline(cin, Client.name);
	cout << "\nEnter phone: ";
	getline(cin, Client.phone);
	cout << "\nEnter accountBalance: ";
	cin >> Client.accountBalance;
	return Client;
}

void addNewClient()
{
	stClient Client;
	Client = readNewClient();
	addDataLineToFile(clientsFileName, convertRecordToLine(Client));
}

void addNewClients()
{
	char addMore = 'Y';
	do {
		cout << "Adding new client:\n\n";

		addNewClient();
		cout << "\nClient added successfully,Do you want to add more clients? ";
		cin >> addMore;
	} while (toupper(addMore) == 'Y');
}

void showAddClientsScreen()
{
	cout << "\n_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ __ _ _ _ _ _ _  _  _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _\n";
	cout << "\n\t\t\t\t\tAdd clients screen.\n";
	cout << "\n_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ __ _ _ _ _ _ __ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _\n";
	addNewClients();
}

bool findClientByAccountNumber(const string& accountNumber, vector<stClient>& vClients, stClient& Client)
{
	for (stClient& c : vClients)
	{
		if (c.accountNumber == accountNumber)
		{
			Client = c;
			return true;
		}
	}
	return false;
}

bool markDeleteClientByAccountNumber(const string& accountNumber, vector<stClient>& vClients)
{
	for (stClient& c : vClients)
	{
		if (c.accountNumber == accountNumber)
		{
			c.markedForDelete = true;
			return true;
		}
	}
	return false;
}

void  deleteClientByAccountNumber(const string& accountNumber, vector<stClient>& vClients)
{
	stClient Client;
	char answer = 'n';
	if (!findClientByAccountNumber(accountNumber, vClients, Client))
	{
		cout << "\n\nClient with account number (" << accountNumber << ") is Not Found!";
	}
	else
	{
		printClientCard(Client);

		cout << "\n\nAre you sure you want delete this client? y/n? ";
		cin >> answer;
		if ((answer == 'Y') || (answer == 'y'))
		{
			markDeleteClientByAccountNumber(accountNumber, vClients);
			saveClientsDataToFile(clientsFileName, vClients);

			//Refresh clients	
			vClients = loadClientsDataFromFile(clientsFileName);

			cout << "\n\nClient deleted successfully. \n";
		}
	}
}

void showDeleteClientScreen()
{
	cout << "\n_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ __ _ _ _ _ _ _  _ _  _ _ _  _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _\n";
	cout << "\n\t\t\t\t\tDelete client screen.\n";
	cout << "\n_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ __ _ _ _ _ _ _  _ _  _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _\n";
	vector<stClient> vClients = loadClientsDataFromFile(clientsFileName);
	string accountNumber = readAccountNumber();
	deleteClientByAccountNumber(accountNumber, vClients);
}

stClient changeClientRecord(const string& accountNumber)
{
	stClient Client;
	Client.accountNumber = accountNumber;
	cout << "Enter pinCode: ";
	getline(cin >> ws, Client.pinCode);
	cout << "Enter name: ";
	getline(cin, Client.name);
	cout << "Enter phone :";
	getline(cin, Client.phone);
	cout << "Enter accountBalance :";
	cin >> Client.accountBalance;
	return Client;
}

bool updateClientByAccountNumber(const string& accountNumber, vector<stClient>& vClients)
{
	stClient Client;
	char answer = 'n';
	if (findClientByAccountNumber(accountNumber, vClients, Client))
	{

		printClientCard(Client);
		cout << "\n\nAre you sure you want update this client? y/n? ";
		cin >> answer;
		if (toupper(answer) == 'Y')
		{
			for (stClient& c : vClients)
			{
				if (c.accountNumber == accountNumber)
				{
					c = changeClientRecord(accountNumber);
					break;
				}
			}
			saveClientsDataToFile(clientsFileName, vClients);
			cout << "\n\nClient update successfully.\n\n";
			return true;
		}
	}
}

void showUpdateClientScreen()
{
	cout << "\n_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ __ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _\n";
	cout << "\n\t\t\t\t\tUpdate client info screen.\n";
	cout << "\n_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ ___ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _\n";
	stClient Client;
	string accountNumber = readAccountNumber();
	vector<stClient> vClients = loadClientsDataFromFile(clientsFileName);

	updateClientByAccountNumber(accountNumber, vClients);
}

void showFindClientScreen()
{
	cout << "\n_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ __ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _\n";
	cout << "\n\t\t\t\t\tFind client screen \n";
	cout << "\n_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ __ __ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _\n";
	vector<stClient> vClients = loadClientsDataFromFile(clientsFileName);
	string accountNumber = readAccountNumber();
	stClient Client;
	if (findClientByAccountNumber(accountNumber, vClients, Client))
		printClientCard(Client);
	else
		cout << "Client with account Number (" << accountNumber << ")is Not Found!\n";
}

void showEndScreen()
{
	cout << "\n_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _  _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _\n";
	cout << "\n\t\t\t\t\tProgram ends :-)\n";
	cout << "\n_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _\n";
}

double sumTotalBalances(vector<stClient>& vClients)
{
	double sum = 0;
	for (stClient& c : vClients)
	{
		sum += c.accountBalance;		
	}
	return sum;
}

void showTotalBalancesScreen()
{
	vector<stClient> vClients= loadClientsDataFromFile(clientsFileName);

	cout << "\t\t\t\t\tTotal balances screen \n";
	cout << "\n\n_______________________________________________________________________________________________\n";
	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(40) << "Client name";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n_______________________________________________________________________________________________\n";
	if(vClients.size() == 0)
		cout << "\t\t\t\tNo clients available in the system!";
	else
	{
		for (stClient& client : vClients)
		{
			cout << "| " << left << setw(15) << client.accountNumber;
			cout << "| " << left << setw(40) << client.name;
			cout << "| " << left << setw(12) << client.accountBalance;
			cout << endl;
		}
	}
	cout << "\n\n_______________________________________________________________________________________________\n";
	cout << "\t\t\t\t\tTotal balances = " << sumTotalBalances(vClients)<<"\n";
}

short readTransactionsMenueOption()
{
	short option = 0;
    do{
	    cout << "Choose what do you want to enter ? 1 to 4 ? ";
	    cin >> option;	   
      }while ((option < 1) || (option > 4));
	return option;
}

double readAmount()
{
	double amount;
	cout << "Please enter amount?";
	cin >> amount;
	return amount;
}

void depositAmountToAccountBalance(string& accountNumber,vector<stClient>& vClients)
{
	stClient Client;
	double amount;
	while(!findClientByAccountNumber(accountNumber,vClients, Client))
	{
		cout << "Client with account number [" << accountNumber << "] does not exists.\n";
		accountNumber = readAccountNumber();
	} 
	printClientCard(Client);
	amount = readAmount();
	for (stClient& c : vClients)
	{
		if (c.accountNumber == Client.accountNumber)
		{
			c.accountBalance += amount;
			cout << "Done successfully,New balance is " << c.accountBalance << "\n";
			
		}
	}
	saveClientsDataToFile(clientsFileName, vClients);
	
}

void showDepositScreen()
{
 	cout << "\n_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _  _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _\n";
	cout << "\n\t\t\t\t\tDeposit screen \n";
	cout << "\n_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _\n";
	string accountNumber = readAccountNumber();
	vector<stClient> vClients = loadClientsDataFromFile(clientsFileName);
	depositAmountToAccountBalance(accountNumber,vClients);
}

void withDrawAmountFromAccountBalance(string& accountNumber, vector<stClient>& vClients)
{
	stClient Client;
	double amount;
	while (!findClientByAccountNumber(accountNumber, vClients, Client))
	{
		cout << "Client with account number [" << accountNumber << "] does not exists.\n";
		accountNumber = readAccountNumber();
	}
	printClientCard(Client);
	amount = readAmount();
	while (amount > Client.accountBalance)
	{
		cout << "Amount exceeds the balance,you can withDraw up to " << Client.accountBalance << "\n";
		amount = readAmount();
	}
	for (stClient& c : vClients)
	{
		if (c.accountNumber == Client.accountNumber)
		{
			c.accountBalance -= amount;
			cout << "Done successfully,New balance is " << c.accountBalance << "\n";
		}
	}
	saveClientsDataToFile(clientsFileName, vClients);
}

void showWithDrawScreen()
{
	cout << "\n_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _  _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _\n";
	cout << "\n\t\t\t\t\tWithDraw screen\n";
	cout << "\n_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _\n";
	string accountNumber = readAccountNumber();
	vector<stClient> vClients = loadClientsDataFromFile(clientsFileName);
	withDrawAmountFromAccountBalance(accountNumber, vClients);

}

void performTransactionsMenueOption(enTransactionsMenueOptions TransactionsOption)
{
    switch (TransactionsOption)
	{
    	case enTransactionsMenueOptions::eDeposit:
		{
			system("cls");
			showDepositScreen();
			goBackToMainMenue();
			break;
		}

		case enTransactionsMenueOptions::eWithDraw:
		{
			system("cls");
			showWithDrawScreen();
			goBackToMainMenue();
			break;
		}

		case enTransactionsMenueOptions::eTotalBalances:
		{
			system("cls");
			showTotalBalancesScreen();
			goBackToMainMenue();
			break;
		}

		case enTransactionsMenueOptions::eShowMainMenue:
		{
			system("cls");
			showMainMenue();
			break;
		}
    }

}

void showTransactionsMenueScreen()
{
	cout << "===========================================\n";
	cout << "\tTransactions menue screen\n";
	cout << "===========================================\n";
	cout << "\t[1] Deposit.\n";
	cout << "\t[2] WithDraw.\n";
	cout << "\t[3] Total Balances.\n";
	cout << "\t[4] Main menue. \n";
	cout << "===========================================\n";

	performTransactionsMenueOption(enTransactionsMenueOptions(readTransactionsMenueOption())); 

}

void performMainMenueOption(enMainMenueOptions MainMenueOption)
{
	switch (MainMenueOption)
	{
	   case enMainMenueOptions::eListClients:
	   {
  		system("cls");
	  	showAllClientsScreen();
	  	goBackToMainMenue();
	  	break;
	   }
	   case enMainMenueOptions::eAddClients:
	   {
  		system("cls");
  		showAddClientsScreen();
	  	goBackToMainMenue();
		  break;
	   }
	   case enMainMenueOptions::eDeleteClient:
	   {
	  	system("cls");
		  showDeleteClientScreen();
	  	goBackToMainMenue();
	  	break;
	   }
       case enMainMenueOptions::eUpdateClient:
	   {
	  	system("cls");
	  	showUpdateClientScreen();
	  	goBackToMainMenue();
	  	break;
	   }
	   case enMainMenueOptions::eFindClient:
	   {
	  	system("cls");
	  	showFindClientScreen();
	  	goBackToMainMenue();
	  	break;
	   }
	   case enMainMenueOptions::eShowTansactionsMenue:
	   {
	  	system("cls");
	  	showTransactionsMenueScreen();
  		goBackToMainMenue();
	  	break;
	   }
	   case enMainMenueOptions::eExit:
	   {
	  	system("cls");
	    showEndScreen();
	    break;
	   }
	}
}

void showMainMenue()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tMain menue screen\n";
	cout << "===========================================\n";
	cout << "\t[1] Show clients list.\n";
	cout << "\t[2] Add new client.\n";
	cout << "\t[3] Delete client.\n";
	cout << "\t[4] Update client info.\n";
	cout << "\t[5] Find client.\n";
	cout << "\t[6] Transactions.\n";
	cout << "\t[7] Exit.\n";
	cout << "===========================================\n";
	performMainMenueOption(enMainMenueOptions(readMainMenueOption()));
}

int main()
{
	showMainMenue();
	system("pause>0");
	return 0;
}

