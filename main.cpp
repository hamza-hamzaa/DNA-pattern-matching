#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include"Aho-Corasick.h"
#include"bruteforce.h"
#include"rabinkarp.h"
#include"SuffixArray.h"
using namespace std;



//helper function that load text from file into document as a string
string loadDNA(const string& filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        return "";
    }

    string dna = "";
    string line;

    while (getline(file, line)) {
        dna += line;
    }

    file.close();
    return dna;
}

//confirms file only contains DNA characters A,C.T,G
bool isValidDNA(const string& dna) {
    for (char c : dna) {
        if (c != 'A' && c != 'T' && c != 'G' && c != 'C'&& 
            c != 'a' && c != 't' && c != 'g' && c != 'c' ) {
            return false;
        }
    }
    return true;
}


int expectedCount(const string& dna, const string& pattern) {
    if (pattern.empty() || pattern.size() > dna.size())
        return 0;

    int count = 0;

    for (size_t i = 0; i + pattern.size() <= dna.size(); i++) {
        bool match = true;

        for (size_t j = 0; j < pattern.size(); j++) {
            if (dna[i + j] != pattern[j]) {
                match = false;
                break;
            }
        }

        if (match)
            count++;
    }

    return count;
}






//helper function that easily returns count of the Aho corasick algorithm so its easier to call many times
int Aho_CorasickCount(const string& DNA,string patterns[],int size){
    AhoCorasick<char> aho;      
    aho.createTrie(patterns, size);
    int a=aho.search(DNA);   
    return a;
}   

int BMH( const string& a, string pattern){}

//helper function that easily returns count of the suffix array algorithm so its easier to call many times
int suffixArrayCount( const string& dna, const string& pattern){
    SuffixTuple* tuples = suffixArrayBuilder(dna);
    int count = countPattern(dna, tuples, pattern.length(), pattern);
    return count;
}

int main() {
    string filename;
    cout << "Enter the DNA file path (.txt): ";
    getline(cin, filename);
    ifstream DNAfile(filename);
    string dna = loadDNA(filename);
    



    //confirms file is not empty and contains correct text
    if(!isValidDNA(dna)){
        return 1;
    }
    if (dna.empty()) {
        cout << "No DNA sequence loaded.\n";
        return 1;
    }


    //detrmines number of wanted patterns and then get these patterns and stores them in a dynamic arr.
    int patternNumber;                                   
    cout<<"how many patterns do you want to find?"<<endl;
    cin>>patternNumber;                                  
    string* patterns=new string[patternNumber];          
    for(int i=0;i<patternNumber;i++){                    
        cout<<"enter pattern: ";                         
        cin>>patterns[i];                                
    }                                                    

    
    //time/count code that helps determine by starting two clocks before and after algorithm to get total time
    //and using the created count function it determines count 
    auto start = chrono::steady_clock::now();
    int Aho_corasickCount = Aho_CorasickCount(dna, patterns, patternNumber);
    auto end = chrono::steady_clock::now();
    double Aho_CorasickElapsed = chrono::duration<double, milli>(end - start).count();

    
    

    //same as with the aho corasick algorithm but since this algorithm cant parallel
    // process patterms it uses a for loop to get tootal counts
    int bruteForceCounts=0;
    start = chrono::steady_clock::now();
    for(int i=0;i<patternNumber;i++){
        DNAfile.clear();
        DNAfile.seekg(0);
        bruteForceCounts+=bruteForceSearch(DNAfile,patterns[i]);
    }
    end = chrono::steady_clock::now();                                           
    double bruteForceElapsed = chrono::duration<double, milli>(end - start).count();




    //same as with the aho corasick algorithm but since this algorithm cant parallel
    // process patterms it uses a for loop to get tootal counts                     
    int rabin_KarpCounts=0;    
    start = chrono::steady_clock::now();       
    for(int i=0;i<patternNumber;i++){  
        DNAfile.clear();
        DNAfile.seekg(0);                     
        rabin_KarpCounts+=rabinKarpSearch(DNAfile,patterns[i]);
    }    
    end = chrono::steady_clock::now();                                              
    double rabin_KarpElapsed = chrono::duration<double, milli>(end - start).count();   
    
    

    //same as with the aho corasick algorithm but since this algorithm cant parallel
    // process patterms it uses a for loop to get tootal counts                     
    int BMHCounts=0;      
    start = chrono::steady_clock::now();      
    for(int i=0;i<patternNumber;i++){  
        DNAfile.clear();
        DNAfile.seekg(0);                     
        BMHCounts+=BMH(dna,patterns[i]);
    }                                    
    end = chrono::steady_clock::now();                                                 
    double BMHElapsed = chrono::duration<double, milli>(end - start).count();   

    //same as with the aho corasick algorithm but since this algorithm cant parallel
    // process patterms it uses a for loop to get tootal counts                         
    int suffixArrayCounts=0;  
    start = chrono::steady_clock::now();              
    for(int i=0;i<patternNumber;i++){    
        DNAfile.clear();
        DNAfile.seekg(0);                   
        suffixArrayCounts+=suffixArrayCount(dna,patterns[i]);    
    }                                                           
    end = chrono::steady_clock::now();                                       
    double suffixArrayElapsed = chrono::duration<double, milli>(end - start).count();



    //using the expected count function it determines count but since
    //the function can only get 1 pattern at a time it uses a for loop 
    int total=0;
    for(int i=0;i<patternNumber;i++){
        total+=expectedCount(dna,patterns[i]);
    }




    // help visualize the results of all 5 algorithms by comparing time,and occurances then printing actual number of occurances
    cout<<"ALGORITHM:"<<"                OCCURANCES                TIMES"<<endl;
    cout<<"AHO-CORASICK:                   "<< Aho_corasickCount<<"                  "<<Aho_CorasickElapsed<<endl;
    cout<<"BRUTE FORCE:                    "<< bruteForceCounts<<"                  "<<bruteForceElapsed<<endl;
    cout<<"RABIN KARP:                     "<< rabin_KarpCounts<<"                  "<<rabin_KarpElapsed<<endl;
    cout<<"BOORE MOORE HORSPOOL:           "<< BMHCounts<<"                  "<<BMHElapsed<<endl;  
    cout<<"SUFFIX ARRAY:                   "<< suffixArrayCounts<<"                  "<<suffixArrayElapsed<<endl;
    
    cout<<endl<<endl<<"EXPECTED:"<<total;
    delete [] patterns;
    return 0;
}

