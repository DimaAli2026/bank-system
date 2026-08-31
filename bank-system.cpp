#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<iomanip>

using namespace std;

const string clientsFileName="Clients.txt";


enum enCardOperation{showData=1,addData=2,deleteData=3,updateData=4,findData=5,exitSystem=6};

struct stClient
{
	string accountNumber="";
	string pinCode="";
	string name="";
	string phone="";
	double accountBalance=0.000000;
	bool markedForDelete=false;
};
	
enCardOperation readNumber()
{
	short num=0;
	do{
		    cout<<"\n\nChoose what do you want to do? [1 to 6]?";
		    cin>>num;
		}while(num<1 || num>6);
  return (enCardOperation)num;
}

string readAccountNumber()
{
   	string accountNumber="";
   	cout<<"Please enter account number? ";
    getline(cin>>ws,accountNumber);
    return accountNumber;
}
       
void printMainMenueScreen()
{	cout<<"\n===========================================\n\n";
cout<<setw(28)<<"Main menue screen"<<setw(10)<<endl;
cout<<"\n===========================================\n\n";
cout<<setw(32)<<"[1] Show clients list."<<setw(10)<<endl;
cout<<setw(29)<<"[2] Add new client."<<setw(10)<<endl;
cout<<setw(28)<<"[3] Delete client."<<setw(10)<<endl;
cout<<setw(33)<<"[4] Update client info."<<setw(10)<<endl;
cout<<setw(26)<<"[5] Find client."<<setw(10)<<endl;
cout<<setw(19)<<"[6] Exit."<<setw(10)<<endl;
cout<<"\n===========================================\n\n";

}

vector<string> splitString(string line,string seperator="#//#")
{
	vector<string> vClientData;
	string word="";
	short pos=0;
	while((pos=line.find(seperator))!=std::string::npos)
	{
		word=line.substr(0,pos);
		if(word!="")
	      vClientData.push_back(word);
	  line.erase(0,pos+seperator.length());
  }
  vClientData.push_back(line);
  return vClientData;
} 

string convertClientDataToString(const stClient& Client,string seperator="#//#")   
{
	string sClientData="";
	sClientData+=Client.accountNumber+seperator;
	sClientData+=Client.pinCode+seperator;
	sClientData+=Client.name+seperator;
	sClientData+=Client.phone+seperator;
	sClientData+=to_string(Client.accountBalance);
	return sClientData;
}	

stClient convertClientDataToRecord(const vector<string>& vClient)
{
	stClient Client;
	Client.accountNumber=vClient[0];
	Client.pinCode=vClient[1];
	Client.name=vClient[2];
	Client.phone=vClient[3];
	Client.accountBalance=stod(vClient[4]);
	return Client;
}	
		
vector<stClient> loadClientsDataFromFile(const string& fileName)
{
	vector<stClient> vClients;
	stClient Client;
	vector<string> vClientData;
	fstream MyFile;
	MyFile.open(fileName,ios::in);//read mode
	
	if(MyFile.is_open())
	{
	   string line="";	
       while(getline(MyFile,line))
    	{
    	      vClientData=splitString(line);	    		
    	      Client=convertClientDataToRecord(vClientData);
    	      vClients.push_back(Client);
    	}
    }
    MyFile.close();
    return vClients;
}

void saveClientsDataToFile(const string& fileName,vector<stClient>& vClients)
{
	fstream MyFile;
	MyFile.open(fileName,ios::out);
	
	if(MyFile.is_open())
	{
        for(stClient& c:vClients)
        {
        	if(c.markedForDelete==false)
           	MyFile<<convertClientDataToString(c)<<endl;
        }
  }
  MyFile.close();
}          

void showClientsList()
{
     vector<stClient> vClients=loadClientsDataFromFile(clientsFileName);
     cout<<"_______________________________________________________________________________________________\n\n";
     cout<<setw(29)<<"Clients list ("<<vClients.size()<<") "<<"client(s)."<<setw(29);
     cout<<"\n_______________________________________________________________________________________________\n\n";
     cout<<left<<setw(20)<<"| Account Number";
     cout<<left<<setw(32)<<"| Pin code";   
     cout<<left<<setw(22)<<"| Client name";
     cout<<left<<setw(13)<<"| Phone";
     cout<<left<<setw(10)<<"| Balance";
     cout<<"\n_______________________________________________________________________________________________\n\n";
     for(const stClient& c:vClients)
     {
     	 cout<<"| "<<setw(18)<<left<<c.accountNumber;
     	 cout<<"| "<<setw(30)<<left<<c.pinCode;
    	 cout<<"| "<<setw(20)<<left<<c.name;
     	 cout<<"| "<<setw(11)<<left<<c.phone;
     	 cout<<"| "<<setw(10)<<left<<c.accountBalance;
     	 cout<<endl;
     }
     cout<<"\n\n_______________________________________________________________________________________________\n\n";

}	
 
bool findClientByAccountNumber(const string& accountNumber,stClient& Client,vector<stClient>& vClients)
{	
	for(stClient& c:vClients) 	    
	{
			if(c.accountNumber==accountNumber)
			 {
			 	Client=c;
			 	return true;
			 }
  }
    return false;
}

stClient readNewClient(const string& accountNumber)
{
	stClient Client;
	Client.accountNumber=accountNumber;
	cout<<"Enter pinCode? ";
	getline(cin>>ws,Client.pinCode);
	cout<<"Enter name? ";
	getline(cin,Client.name);
	cout<<"Enter phone? ";
	getline(cin,Client.phone);
	cout<<"Enter accountBalance? ";
	cin>>Client.accountBalance;
	return Client;
}
	
void addNewClient(vector<stClient>& vClients)
{ 
    string accountNumber;
    stClient Client;
    string sClientData;
    char answer='n';
    
    cout<<"\n\n_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ \n\n";
    cout<<setw(10)<<"Add new clients screen"<<setw(10);
    cout<<"\n_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ \n\n";
    cout<<"Adding New client:\n\n";			 	    
    accountNumber=readAccountNumber(); 
    while(findClientByAccountNumber(accountNumber,Client,vClients))
    {
    	cout<<"Client with account number ["<<accountNumber<<"] already exists,Enter another account number? ";
    	getline(cin>>ws,accountNumber);
    }
 do{
    	Client=readNewClient(accountNumber);
    	vClients.push_back(Client);
    	saveClientsDataToFile(clientsFileName,vClients);
    	vClients=loadClientsDataFromFile(clientsFileName);
    	cout<<"Client added successfully,do you want to add more clients? y/n? ";
    	cin>>answer;
    }while(toupper(answer)=='Y');
}     
 
 void showClientCard(const stClient& Client)
 {
 	 cout<<"The following are the client details:\n";
 	 cout<<"\n\n_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ \n";
 	 cout<<"Account number : "<<Client.accountNumber<<endl;
 	 cout<<"Pin code       : "<<Client.pinCode<<endl;
 	 cout<<"Name           : "<<Client.name<<endl;
 	 cout<<"Phone          : "<<Client.phone<<endl;
 	 cout<<"Account balance: "<<Client.accountBalance<<endl;
 	 cout<<"_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ \n";
} 	

void findClientByAccountNumber(vector<stClient>& vClients)
{
	string accountNumber;
	stClient Client;
	cout<<"\n\n_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ \n\n";
  cout<<setw(10)<<"find clients screen"<<setw(10);
  cout<<"\n_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ \n\n";
	accountNumber=readAccountNumber();
  if(findClientByAccountNumber(accountNumber,Client,vClients))
     showClientCard(Client);
 else
     cout<<"\nClient with account number ["<<accountNumber<<"] is Not Found!\n";
    
}    

 bool isClientMarkedForDelete(const string& accountNumber,vector<stClient>& vClients)
 {
 	for(stClient& c:vClients)
 	{
 		if(c.accountNumber==accountNumber)
 		{
 			c.markedForDelete=true;
 			return true;
 		}
  }
  return false;
}
    			
 void deleteClientByAccountNumber(vector<stClient>& vClients)
 {
 	stClient Client;
 	string accountNumber;
 	char answer='n';
 	cout<<"\n\n_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ \n\n";
  cout<<setw(10)<<"Delete clients screen"<<setw(10);
  cout<<"\n_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ \n\n";
  accountNumber=readAccountNumber(); 
  while(!findClientByAccountNumber(accountNumber,Client,vClients))
  {
    cout<<"Client with account number ["<<accountNumber<<"] is Not Found,please enter account number is exist? ";
    getline(cin,accountNumber);
  }
  showClientCard(Client);
  cout<<"\n\nAre you sure you want delete this client? y/n? ";
  cin>>answer;
  if(toupper(answer)=='Y')
  {
 	  isClientMarkedForDelete(accountNumber,vClients);	
    saveClientsDataToFile(clientsFileName,vClients);
    vClients=loadClientsDataFromFile(clientsFileName);
    cout<<"\n\nClient deleted successfully.\n\n";
 	}     
 } 	      	     

void updateClientData(const string& accountNumber,vector<stClient>& vClients)
{
	for(stClient& c:vClients)
	{
		if(c.accountNumber==accountNumber)
		{
		   c=readNewClient(accountNumber);
		   break;
		}   
	}	   
}		   

 void updateClientByAccountNumber(vector<stClient>& vClients)
{
 	stClient Client;
 	string accountNumber;
 	char answer='n';
 	cout<<"\n\n_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ \n\n";
  cout<<setw(10)<<"Update clients screen"<<setw(10);
  cout<<"\n_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ \n\n";
  accountNumber=readAccountNumber(); 
  while(!findClientByAccountNumber(accountNumber,Client,vClients))
  {
  	cout<<"Client with account number ["<<accountNumber<<"] is Not Found,please enter account number is exist? ";
    getline(cin,accountNumber);
  }	
  showClientCard(Client);
  cout<<"\n\nAre you sure you want update this client? y/n? ";
  cin>>answer;
  if(toupper(answer)=='Y')
  {
 	  updateClientData(accountNumber,vClients);
 	  saveClientsDataToFile(clientsFileName,vClients);
    vClients=loadClientsDataFromFile(clientsFileName);
    cout<<"\n\nClient update successfully.\n\n";
 	}    
} 	    

void goBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>nul"); 
}

 void playBankSystem()
 {
    enCardOperation choose;
    do{
         system("cls");
 	       printMainMenueScreen();
 	       vector<stClient> vClients=loadClientsDataFromFile(clientsFileName);
 	       choose=readNumber();//read the choose of the user
 	       switch(choose)
 	        {
 	        	case enCardOperation::showData:
 	          	{
 	   	            system("cls");
 	   	            showClientsList();  
 	   	            goBackToMainMenue();
 	   	            break;
 	           	}   
 	        	case enCardOperation::addData:
 	          	{
 	   	           system("cls");
 	   	           addNewClient(vClients);
 	   	           goBackToMainMenue();
 	   	           break;
 	          	}
 	          case enCardOperation::deleteData:
 	        	 {
 	        	    system("cls");
 	   	          deleteClientByAccountNumber(vClients);
 	        	    goBackToMainMenue();
 	   	          break;
 	        	 }
 	          case enCardOperation::updateData:
 	        	 {
 	   	          system("cls");
 	   	          updateClientByAccountNumber(vClients);
 	   	          goBackToMainMenue();
 	   	          break;
 	        	 }
 	          case enCardOperation::findData:
 	   	       {
                system("cls");
                findClientByAccountNumber(vClients);
                goBackToMainMenue();
                break;
             }    
            default:
             {
                system("cls"); 
                cout<<"\n\n_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ \n\n";
                cout<<setw(10)<<"Program ends :-)"<<setw(10);
                cout<<"\n_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ \n\n";
                break;
             }
           }
        }while(choose!=enCardOperation::exitSystem);
}                  
 	   	       	     
int main()
{
	    playBankSystem();
	    return 0;
}
