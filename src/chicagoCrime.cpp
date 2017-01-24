/*
	C++ program to analyze Chicago crime data.
	
	Jonathan Galvan
	U. of Illinois, Chicago
	CS341, Fall 2015
	Homework 3

*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>

// Formated the text files into txt extension
#define sourceOne "Crimes.txt"
#define sourceTwo "IUCR-codes.txt"
#define sourceThree "Communities.txt"

using namespace std;

class Crimes {
	public:
		string uicrID;
		string community;
		string year;

		Crimes(string getuicr,string getcommunity,string getyear) {
			uicrID = getuicr;
			community = getcommunity;
			year = getyear;
		}

};

class IUCR {
	public:
		string iucrCode;
		string pDescription;
		string sDescription;

		IUCR(string getcode, string getpDescription, string getsDescription) {
			iucrCode = getcode;
			pDescription = getpDescription;
			sDescription = getsDescription;
		}

};

class TopCrime{
	public:
		string iucrReference;
		int counter = 1;

		TopCrime(string getiucrReference) {
			iucrReference = getiucrReference;
			
		}

};

class Communities{
	public:
		string number;
		string name;
		
		Communities(string getnumber, string getname) {
			number = getnumber;
			name = getname;
		}
};


Crimes ParseCrimes(string line) {
	// NEED TO COME UP WITH CASES THAT HANDLE empty COLUMNS for BIG FILE

	istringstream strstream(line);
	string iucrstr, communitystr, yearstr, tmpstr;

	getline(strstream, tmpstr, ',');
	getline(strstream, iucrstr, ',');
	getline(strstream, tmpstr, ',');
	getline(strstream, tmpstr, ',');
	getline(strstream, tmpstr, ',');
	getline(strstream, tmpstr, ',');
	getline(strstream, tmpstr, ',');
	getline(strstream, communitystr, ',');

	// Checks to see if the community string is empty, if so add a default string
	if (communitystr.compare("") == 0) {
		communitystr = "99";
	}


	getline(strstream, yearstr);

	return Crimes(iucrstr, communitystr, yearstr); 
}

IUCR ParseIUCR(string line) {

	istringstream strstream(line);
	string iucrstr, primarystr, secondarystr;

	getline(strstream, iucrstr, ',');
	getline(strstream, primarystr, ',');
	getline(strstream, secondarystr);

	return IUCR(iucrstr, primarystr, secondarystr);
}

Communities ParseCommunities(string line) {

	istringstream strstream(line);
	string numberstr, namestr;

	getline(strstream, numberstr, ',');
	getline(strstream, namestr);

	return Communities(numberstr, namestr);
}

/* Function that determines if the iucr value is unique. If found in rank, add one to counter and exit
   If not unique, then add it to the unique vector.
*/
void check(string iucrCheck, vector<TopCrime>& rank) {
	
	// Checks to see if rank vector is empty. If so, we can assume it is a unique iucr value.
	if (rank.empty()) {
		TopCrime s(iucrCheck);
		rank.push_back(s);
	}
	
	else {

		int found = 0;

		// Goes through each index in the rank vector to see if the unique iucr exists
		for (auto iter = rank.begin(); iter != rank.end(); iter++) {
		
			// Compares the crime iucr string with unique iucr strings from rank vector, iucr is already stored
			if ( (*iter).iucrReference.compare(iucrCheck) == 0) {
				(*iter).counter++; // increments the count of the unique iucr
				found = 1; // Found a unique instance
				return; // breaks from the from the for loop 
			}

		}
		
		// If unique iucr does not exist in the rank then we add it to the vector
		if (found == 0) {
			TopCrime s(iucrCheck);
			rank.push_back(s);
		}

	}
}

void checkCommunity(string iucrCheck, vector<TopCrime>& communityRank) {

	// Checks to see if communityrank vector is empty. If so, we can assume it is a unique iucr value.
	if (communityRank.empty()) {
		TopCrime s(iucrCheck);
		communityRank.push_back(s);
	}

	else {

		int found = 0;

		// Goes through each index in the communityrank vector to see if the unique iucr exists
		for (auto iter = communityRank.begin(); iter != communityRank.end(); iter++) {

			// Compares the crime iucr string with unique iucr strings from community rank vector, iucr is already stored
			if ((*iter).iucrReference.compare(iucrCheck) == 0) {
				(*iter).counter++; // increments the count of the unique iucr
				found = 1; // Found a unique instance
				return; // breaks from the from the for loop 
			}

		}

		// If unique iucr does not exist in the communityrank then we add it to the vector
		if (found == 0) {
			TopCrime s(iucrCheck);
			communityRank.push_back(s);
		}

	}
}

void displayTopCrime(const vector<TopCrime>& rank, const vector<IUCR>& iucrstorage) {

	int limit = 1;

	for (auto iter = rank.begin(); iter != rank.end(); iter++) {

		if (limit == 11) {
			break;
		}
		else{

			// Searches for the rank uicr number in the IUCR storage vector structure
			auto i = find_if(iucrstorage.begin(), iucrstorage.end(), [&](IUCR a)
			{ return !a.iucrCode.compare((*iter).iucrReference); });

			cout << limit << "." << "\t" << (*iter).iucrReference << "\t" << (*iter).counter << "\t" << (*i).pDescription << " " << "(" << (*i).sDescription << ")" << endl;
			
		}
		limit++;
	}

}

void displayCommunity(string x, const vector<Communities>& communityStorage, const vector<Crimes>& crimeStorage, vector<TopCrime>& communityRank, const vector<IUCR> iucrstorage) {

	// Finds the name of the community 
	auto i = find_if(communityStorage.begin(), communityStorage.end(), [&](Communities a)
	{return !x.compare(a.number); });

	cout << ">> " << (*i).name << " <<" << endl;
	
	// Counts the number of times a crime has occured in the inputed community number
	int totalCount = count_if(crimeStorage.begin(), crimeStorage.end(), [&](Crimes b)
	{return !x.compare(b.community); });

	cout << ">> Total:" << "\t" << totalCount << endl;

	// Calculate the crime percent corresponding to the community number 
	double percent = ( (double)totalCount / crimeStorage.size()) * 100;
	cout << ">> Percentage:  " << percent << "%" << endl;

	// Calculate the number of homicides 
	int countHomicides = count_if(crimeStorage.begin(), crimeStorage.end(), [&](Crimes c)
	{ return !x.compare(c.community) && (!c.uicrID.compare("0110") || !c.uicrID.compare("0130") || !c.uicrID.compare("0141") || !c.uicrID.compare("0142")); });

	cout << ">> Homicides:   " << countHomicides << endl;
	cout << ">> Top-5 Crimes <<" << endl;
	cout << "Rank" << "\t" << "IUCR" << "\t" << "Number" << "\t" << "Description" << endl;

	// Creates the communityRank list 
	for (auto a = crimeStorage.begin(); a != crimeStorage.end(); a++) {

		// If the inputed community matches the community number in crimes then we insert into the communityRank vector 
		if ( (*a).community.compare(x) == 0) {
			checkCommunity((*a).uicrID,communityRank);
		}
	}

	// Sort community Rank by highest to lowest 
	sort(communityRank.begin(), communityRank.end(), [](TopCrime x, TopCrime y)
	{return x.counter > y.counter; });

	int limit = 1;

	for (auto iter = communityRank.begin(); iter != communityRank.end();iter++) {

		if (limit == 6) {
			break;
		}
	
		else {

			// Searches for the rank uicr number in the IUCR storage vector structure
			auto i = find_if(iucrstorage.begin(), iucrstorage.end(), [&](IUCR a)
			{ return !a.iucrCode.compare((*iter).iucrReference); });

			cout << limit << "." << "\t" << (*iter).iucrReference << "\t" << (*iter).counter << "\t" << (*i).pDescription << " " << "(" << (*i).sDescription << ")" << endl;
		}

		limit++;
	}

	//Reset the size of the community back to zero to reuse it
	communityRank.clear();

}

void displayIUCR(string x,const vector<IUCR>& iucrStorage, const vector<TopCrime> rank, const vector<Crimes> crimeStorage) {

	auto i = find_if(iucrStorage.begin(), iucrStorage.end(), [&](IUCR a)
	{return !x.compare(a.iucrCode); });

	cout << ">> " << (*i).pDescription << " (" << (*i).sDescription << ")" << endl;

	auto e = find_if(rank.begin(), rank.end(), [&](TopCrime a)
	{return !x.compare(a.iucrReference);});
	
	cout << ">> Total:" << "\t" << (*e).counter << endl;

	int totalCount = (*e).counter;

	// Calculate the crime percent corresponding to the community number 
	double percent = ((double)totalCount / crimeStorage.size()) * 100;
	cout << ">> Percentage:  " << percent << "%" << endl;

	cout << ">> By Year:" << endl;

	int countYearOne = count_if(crimeStorage.begin(), crimeStorage.end(), [&](Crimes a)
	{return !x.compare(a.uicrID) && !a.year.compare("2015"); });

	int countYearTwo = count_if(crimeStorage.begin(), crimeStorage.end(), [&](Crimes a)
	{return !x.compare(a.uicrID) && !a.year.compare("2014"); });

	cout << "\t" << "2015: " << countYearOne << endl;
	cout << "\t" << "2014: " << countYearTwo << endl;

}

bool findIUCR(string x, const vector<TopCrime>& rank) {


	for (auto iter = rank.begin(); iter != rank.end(); iter++) {
		
		if (!x.compare((*iter).iucrReference)) {
			return true;
		}

	}
	return false;
}


int main() {

	ifstream ifstreamFile(sourceOne);
	ifstream ifstreamFileTwo(sourceTwo);
	ifstream ifstreamFileThree(sourceThree);

	string getInput = ""; // Default string
	string getSecondInput = ""; // Default second string
	string line; // Store each line after the first one
	string garbage; // will retrive uncessary first line of each file

	bool run = true;
	bool execute = true;

	vector<Crimes> crimeStorage; // Data Structure for Crimes
	vector<IUCR> iucrStorage; // Data Structure for IUCR reports
	vector<Communities> communitiesStorage; // Data structure for Community reports
	vector<TopCrime> rank; // Data Structure that stores unique counter for each iucr value
	vector<TopCrime> communityRank; // Data Strucure that stores unique crime counter for each community

	// Reads data from Crime and stores necessary info into crimestorage
	getline(ifstreamFile, garbage); // Skips the file line of crimes file
	
	while (getline(ifstreamFile,line) ) {
		crimeStorage.push_back(ParseCrimes(line));
	}

	// Creates the rank vector to determine overall rank of crime in Chicago
	for (auto iter = crimeStorage.begin(); iter != crimeStorage.end(); iter++) {
		check((*iter).uicrID,rank);
	}

	// Sorts the vector crime based from highest number of crimes to lowest 
	sort(rank.begin(), rank.end(), [](TopCrime x, TopCrime y)
	{return x.counter > y.counter; });

	// Reads data from IUCR file and stores it into iucrstorage
	getline(ifstreamFileTwo, garbage);

	while (getline(ifstreamFileTwo,line) ) {
		iucrStorage.push_back(ParseIUCR(line));
	}

	// Reads data from Communities files and stores it into the communitiesstorage
	getline(ifstreamFileThree, garbage);

	while (getline(ifstreamFileThree,line)) {
		communitiesStorage.push_back(ParseCommunities(line));
	}


	cout << ">> Reading crime data 'Crimes-2014-2015.csv'... " << "[" << crimeStorage.size() << "]" << endl;
	cout << ">> Reading IUCR codes 'IUCR-codes.csv' ... " << "[" << iucrStorage.size() << "]" << endl;
	cout << ">> Reading communities file 'Communities.csv' ..." << "[" << communitiesStorage.size() << "]" << endl;
	cout << endl;

	cout << ">> Top-10 Crimes <<" << endl;
	cout << "Rank" << "\t" << "IUCR" << "\t" << "Number" << "\t" << "Description" << endl;

	displayTopCrime(rank,iucrStorage);
	cout << endl;
	
	cout << ">> By Community  <<" << endl;
	cout << endl;
	
	// while loop for community display
	while (run == true) {
		
		int number;

		do {

			cout << "Please enter a community number [1..77], 0 to stop: ";
			cin >> getInput;

			number = stoi(getInput);

			if (number > 77 || number < 0) {
				cout << "** Invalid community number, please try again..." << endl;
				cout << endl;
			}


		} while (number > 77 || number < 0);

		if (getInput.compare("0") == 0) {
			run = false;
			continue;
		}

		displayCommunity(getInput, communitiesStorage, crimeStorage, communityRank, iucrStorage);
		cout << endl;
	}
	cout << endl;
	
	cout << ">> By IUCR Crime Code <<" << endl;
	cout << endl;

	
	// while loop for IUCR display
	while (execute == true) {

		do {

			cout << "Please enter an IUCR crime code [e.g 0110 or 031A], 0000 to stop: ";
			cin >> getSecondInput;

			if (getSecondInput.compare("0000") == 0) {
				break;
			}

			if (findIUCR(getSecondInput, rank) == false) {
				cout << "** Invalid IUCR code, please try again..." << endl;
				cout << endl;
			}

		} while (findIUCR(getSecondInput,rank) == false || exit == false);

		if (getSecondInput.compare("0000") == 0) {
			execute = false;
			continue;
		}

		displayIUCR(getSecondInput, iucrStorage, rank, crimeStorage);
	}

	cout << endl;
	cout << ">> Done! << " << endl;

	return 0;
}