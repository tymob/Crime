/*
 * C++ console application written to analyze Chicago crime data.
 *
 * Author: Brent Campbell
 * U. of Illinois, Chicago
 * CS341, Fall 2015
 *
 */


#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>

using namespace std;

// input files:

#define crimesCSV "Crimes-2014-2015.csv" // small sample crimes data
#define codesCSV "IUCR-codes.csv"
#define communitiesCSV "Communities.csv"


// class to tally crimes by year and IUCR
class byYear {
    // fields
    int year;
    int count = 0;
    
    // public methods
    public:
    void incCount() {
        count += 1;
    }
    
    // setter
    void setYear(int x) {
        year = x;
    }
    
    // getters
    int getYear() {
        return year;
    }
    
    int getCount() {
        return count;
    }
};

// crime class used to store iucr, which community, and the year of each crime
class crime {
    // fields
    private:
    string iucr;
    int community;
    int year;
    
    // public methods
    public:
    // constructor
    crime() {
        iucr = "NA";
        community = 0;
        year = 0;
    }
    
    // setters
    void setIUCR(string str) {
        iucr = str;
    }
    
    void setComm(int x) {
        community = x;
    }
    
    void setYear(int x) {
        year = x;
    }
    
    // getters
    string getIUCR() {
        return iucr;
    }
    
    int getComm() {
        return community;
    }
    
    int getYear() {
        return year;
    }
};

// community class used to store each community's number, name, and total number of crimes
// and homicides that occured there
class community {
    
    // fields
    private:
    int number;
    int crimesTally;
    string name;
    int homicide;
    vector<crime> allCrimes;
    
    // public methods
    public:
    void homicideTally() {
        homicide++;
    }
    
    void tally() {
        crimesTally++;
    }
    
    // constructor
    community() {
        crimesTally = 0;
        homicide = 0;
    }
    
    // setters
    void setNum(int num) {
        number = num;
    }
    
    void setName(string nameFile) {
        name = nameFile;
    }
    
    void fillCrimesVec(crime x) {
        allCrimes.push_back(x);
    }
    
    // getters
    int getNum() {
        return number;
    }
    
    string getName() {
        return name;
    }
    
    int getCrimes() {
        return crimesTally;
    }
    
    int getHomicides() {
        return homicide;
    }
    
    vector<crime> getCrimeVec() {
        return allCrimes;
    }
};


// IUCR class used to store each IUCR's number, primary description (secondary if available) and tally of times committed (and by year)
class iucr {
    // fields
    private:
    string number;
    string priDescrip;
    string secDescrip;
    int totalTally;
    vector<byYear> byYearVec;
    vector<int> byComm;
    
    // public methods
    public:
    void tally() {
        totalTally += 1;
    }
    
    void addToCommVec(int x) {
        byComm[x - 1] += 1;
    }
    
    // constructor
    iucr() {
        totalTally = 0;
        byComm.assign(77, 0);
    }
    
    // setters
    void setNum(string num) {
        number = num;
    }
    
    void setPriDescrip(string str) {
        priDescrip = str;
    }
    
    void setSecDescrip(string str) {
        secDescrip = str;
    }
    
    // getters
    vector<int> getVec() {
        return byComm;
    }
    
    string getNum() {
        return number;
    }
    
    string getPriDescrip() {
        return priDescrip;
    }
    
    string getSecDescrip() {
        return secDescrip;
    }
    
    int getTotalCrimes() {
        return totalTally;
    }
    
    vector<byYear> getYearVec() {
        return byYearVec;
    }
};


// function that analyzes all the data
void analyzeCrimes(vector<iucr>& codes, vector<crime>& crimes, vector<community>& comms) {
    for (auto x : crimes) {
        
        // 1) tally IUCR counts
        // 2) tally crimes by code by community
        // 3) tally crimes by year by code
        
        // tally IUCR counts
        vector<iucr>::iterator it;
        it = find_if(codes.begin(), codes.end(), [&](iucr c1) {return c1.getNum() == x.getIUCR(); });
        if (it != codes.end()) {
            it->tally();
            it->addToCommVec(x.getComm());
            
            // TO-DO: tally each IUCR code by year
            /*
             vector<byYear>::iterator i;
             i = find_if((it->getYearVec()).begin(), (it->getYearVec()).end(), [&](byYear t) {return t.getYear() == x.getYear(); });
             if (i == (it->getYearVec()).end()) {
             byYear temp;
             temp.setYear(x.getYear());
             temp.incCount();
             (it->getYearVec()).push_back(temp);
             }
             else {
             i->incCount();
             }
             */
        }
        
        // tally crimes by community
        vector<community>::iterator it2;
        it2 = find_if(comms.begin(), comms.end(), [&](community c1) {return c1.getNum() == x.getComm(); });
        if (it2 != comms.end())
        it2->tally();
        
        // tally homicides by community
        vector<community>::iterator it3;
        it3 = find_if(comms.begin(), comms.end(), [&](community c1)
                      {
                          return c1.getNum() == x.getComm() && (x.getIUCR() == "0110" || x.getIUCR() == "0130"
                                                                || x.getIUCR() == "0141" || x.getIUCR() == "0142");
                      }
                      );
        if (it3 != comms.end())
        it3->homicideTally();
    }
}

// print top 10 crimes
void printTop10(vector<iucr>& codes) {
    int count = 1;
    for (auto i = codes.begin(); i != codes.begin() + 10; ++i) {
        cout << count << ".\t" << i->getNum() << "\t" << i->getTotalCrimes()
        << "\t" << i->getPriDescrip() << " (" << i->getSecDescrip() << ")" << endl;
        count++;
    }
    cout << "\n\n";
}

// print top 5 crimes of selected community
void printTop5(vector<iucr>& vec, int x) {
    sort(vec.begin(), vec.end(), [&](iucr c1, iucr c2) {return c1.getVec().at(x-1) > c2.getVec().at(x-1); });
    
    int count = 1;
    for (auto i = vec.begin(); i != vec.begin() + 5; ++i) {
        cout << count << ".\t" << i->getNum() << "\t" << i->getVec().at(x-1) << "\t"
        << i->getPriDescrip() << " (" << i->getSecDescrip() << ")" << endl;
        count++;
    }
    cout << "\n";
}

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

int main() {
    
    // vector of community objects
    vector<community> communities;
    
    // vector of IUCR objects
    vector<iucr> iucrVec;
    
    // vector of crime objects
    vector<crime> crimes;
    
    // variable to hold user community selection
    int comm;
    
    // variable to hold user IUCR selection
    string code;
    
    
    
    // read in community data
    cout << ">> Reading community data...";
    ifstream communityFile(communitiesCSV);
    string line;
    
    while (getline(communityFile, line)) {
        if (line[0] == 'N') continue;
        istringstream strstream(line);
        string num, name;
        
        getline(strstream, num, ',');
        getline(strstream, name);
        
        community temp;
        temp.setName(name);
        temp.setNum(stoi(num));
        
        communities.push_back(temp);
    }
    cout << "done." << endl;
    
    // read in IUCR data
    cout << ">> Reading IUCR codes...";
    ifstream iucrFile(codesCSV);
    
    while (getline(iucrFile, line)) {
        if (line[0] == -17) continue;
        istringstream strstream(line);
        string num, pri, sec;
        
        getline(strstream, num, ',');
        getline(strstream, pri, ',');
        getline(strstream, sec);
        
        iucr temp;
        temp.setNum(num);
        temp.setPriDescrip(pri);
        temp.setSecDescrip(sec);
        
        iucrVec.push_back(temp);
    }
    cout << "done." << endl;
    
    // read in crime data
    cout << ">> Reading crime data...";
    ifstream crimesFile(crimesCSV);
    
    while (getline(crimesFile, line)) {
        if (line[0] == -17) continue;
        istringstream strstream(line);
        string date, code, arrest, domestic, beat, district, ward, comm, year;
        
        getline(strstream, date, ',');
        getline(strstream, code, ',');
        getline(strstream, arrest, ',');
        getline(strstream, domestic, ',');
        getline(strstream, beat, ',');
        getline(strstream, district, ',');
        getline(strstream, ward, ',');
        getline(strstream, comm, ',');
        getline(strstream, year);
        
        crime temp;
        temp.setIUCR(code);
        if (year != "")
        temp.setYear(stoi(year));
        if (comm != "")
        temp.setComm(stoi(comm));
        crimes.push_back(temp);
    }
    
    cout << "done.\n" << endl;
    
    // analyze data..
    cout << ">> Analyzing data. This make take a few minutes...";
    
    analyzeCrimes(iucrVec, crimes, communities);
    sort(iucrVec.begin(), iucrVec.end(), [&](iucr c1, iucr c2) {return c1.getTotalCrimes() > c2.getTotalCrimes(); });
    cout << "done.\n" << endl;
    
    // print required output/take user input:
    
    // Part 1: Print the Top 10 crimes and list what the crime is and its IUCR code
    cout << ">> Top-10 Crimes <<" << endl;
    cout << "Rank\tIUCR\tNumber\tDescription" << endl;
    printTop10(iucrVec);
    
    // Part 2: Give more crime details for a user selected community
    cout << ">> By Community <<\n" << endl;
    cout << "Please enter a community number [1..77], 0 to stop: ";
    
    // get community from the user
    cin >> comm;
    
    // check that the community number is valid
    while (comm < 0 || comm>77) {
        cout << "** Invalid community number, please try again..\n" << endl;
        cout << "Please enter a community number [1..77], 0 to stop: ";
        cin >> comm;
    }
    
    while (comm != 0) {
        // find user selected community
        vector<community>::iterator it;
        it = find_if(communities.begin(), communities.end(), [&](community c1) {return c1.getNum() == comm; });
        
        cout << ">> " << it->getName().erase(it->getName().size()-1) << " <<" << endl;
        cout << ">> Total:\t" << it->getCrimes() << endl;
        cout << ">> Percentage:\t" <<(double)it->getCrimes() / (double)crimes.size() * 100 << "%" << endl;
        cout << ">> Homocides:\t" << it->getHomicides() << endl;
        cout << ">> Top-5 Crimes <<" << endl;
        cout << "Rank\tIUCR\tNumber\tDescription" << endl;
        printTop5(iucrVec, comm);
        
        // repeat until user exits the community info section
        cout << "Please enter a community number [1..77], 0 to stop: ";
        cin >> comm;
        while (comm<0 || comm>77) {
            cout << "** Invalid community number, please try again..\n" << endl;
            cout << "Please enter a community number [1..77], 0 to stop: ";
            cin >> comm;
        }
    }
    
    cout << "\n" << endl;
    
    // Part 3: List more details about a user selected crime (given by the user entering the IUCR code)
    cout << ">> By IUCR Code <<\n" << endl;
    cout << "Please eneter an IUCR crime code [e.g. 0110 or 031A], 0000 to stop: ";
    
    // get IUCR code
    cin >> code;
    
    // exit on '0000'
    if (code == "0000") {
        cout << ">> Done! <<\n" << endl;
        exit(0);
    }
    
    vector<iucr>::iterator i;
    i = find_if(iucrVec.begin(), iucrVec.end(), [&](iucr c1) {return c1.getNum() == code; });
    
    // validate the given IUCR code
    while (i == iucrVec.end()) {
        cout << "** Invalid IUCR code, please try again..\n" << endl;
        cout << "Please eneter an IUCR crime code [e.g. 0110 or 031A], 0000 to stop: ";
        cin >> code;
        if (code == "0000") break;
        i = find_if(iucrVec.begin(), iucrVec.end(), [&](iucr c1) {return c1.getNum() == code; });
    }
    
    // display details of valid IUCR codes
    while (code != "0000") {
        cout << ">> " << i->getPriDescrip() << " (" << i->getSecDescrip() << ")" << endl;
        cout << ">> Total:\t" << i->getTotalCrimes() << endl;
        cout << ">> Percentage:\t" << ((double)i->getTotalCrimes() / (double)crimes.size())*100 << "%" << endl;
        cout << ">> By Year:" << endl;
        cout << "   Feature not yet implemented..\n" << endl;
        
        /*
         for (auto t : i->getYearVec()) {
         cout << "   " << t.getYear() << "\t" << t.getCount() << endl;
         }
         */
        
        // repeat until user exits this portion of the app
        cout << "Please eneter an IUCR crime code [e.g. 0110 or 031A], 0000 to stop: ";
        cin >> code;
        if (code == "0000") break;
        i = find_if(iucrVec.begin(), iucrVec.end(), [&](iucr c1) {return c1.getNum() == code; });
        while (i == iucrVec.end()) {
            cout << "** Invalid IUCR code, please try again..\n" << endl;
            cout << "Please eneter an IUCR crime code [e.g. 0110 or 031A], 0000 to stop: ";
            cin >> code;
            if (code == "0000") break;
            i = find_if(iucrVec.begin(), iucrVec.end(), [&](iucr c1) {return c1.getNum() == code; });
        }
    }
    
    cout << "\n\n>> DONE! <<\n" << endl;
    
    return 0;
}
