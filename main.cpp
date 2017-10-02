#include <fstream>
#include <iostream>
#include <string>
#include <stdint.h>
#include <stdlib.h>
#include "sequitur.hpp"

//
// This file just executes some random actions using my sequitur implementation, to help give you a feel for how it works.
//

using namespace std;
//my namespace gives access to sequitur and related:
using namespace jw;
//typedef std::vector< std::pair< uint64_t, string > > aVector;


int main(int argc, char* argv[])
    {
    //
    // Look at the first string passed in at the console, and treat it as a file path, opening said file:
    //
    //if(argc != 3) cerr << "Need one argument (filename)" << endl;
   string filename = argv[3];
	string filename2 = argv[2];
	uint64_t bshift = atoi(argv[1]);
    
	ofstream fout(filename2, std::ios::binary);
    aVector theVector;
    ifstream input(filename, std::ios::binary);
    if(!input.is_open())
        {
        cerr << "File \"" << filename << "\" not found." << endl;
        return 1;
        }

    //
    // Make a new Sequitur for taking in the char type:
    //
    Sequitur<uint64_t> s;
	uint64_t pc, pAddr, vAddr;
	char status;
    unsigned count = 0;    uint64_t cur, prev; cur = prev = 0;
	cout << "Reading ";
    while(input >> pc >> pAddr >> vAddr >> status)
        {
        //
        //add chars to Sequitur, using the familiar push_back syntax:
        //
			if (status == 'M') {
				s.push_back((pAddr >> bshift) << bshift);
				//
				//record the count and print an output every 100,000 chars:
				//
				count++;
				if (count % 100000 == 0) cout << ".";
			}
        }
    //clear the input flags and seek back to the beginning, so we can read it in again:
    /*input.clear();
    input.seekg(0);*/

    //
    //we can't copy sequitur, but we can move it using the C++11 std::move function.
    //This will move 's' to 's2', invalidating s:
    //
	  cout << "\nMoving sequitur";
    Sequitur<uint64_t> s2(std::move(s));

    //
    //create a sequitur<char>::const_iter pointed to beginning char in s2:
    //
    auto seq_iter = s2.begin();



    //read through the file again, and compare each char with that which is stored in the sequitur container s2:
    //count = 0;
	  cout << "\nRead again ";
    /*while(input >> pc >> pAddr >> vAddr >> status.get(temp_char))
        {
        if(pAddr != *seq_iter)
            {
            cerr << "not equal at " << count << ", file: " << pAddr << " seq: " << *seq_iter << endl;
            }
        //
        // we can post or pre-increment the iterator to look at the next char:
        //
        ++seq_iter;
        ++count;
		if (count % 100000 == 0) cout << ".";
        }*/

    //### lets see some stats: ###

    //
    // get the rule table (of type std::unordered_map<Symbol*>):
    //
   	cout << "\nGetting the rule table\n";
    auto rule_table = s2.getRules();
    auto it = rule_table.begin();
    unsigned symbol_total = 0;
    unsigned rule_count = 0;
    while(it != rule_table.end())
        {
        ++rule_count;
        //
        // consult baselist.hpp for list traversal commands given symbols.
        // consult symbols.hpp for the different symbol types and functions available on each
        //
        // baselist is just a linked list subclass, which the Symbol class inherits from, so
        // symbols can be linked together. Thus, the functionality in baselist.hpp will allow you
        // to traverse the symbols found.
        //
        // Symbol::next() and Symbol::prev() allow iteration through them.
        // Symbol::end()/begin() returns a pair<[final_symbol], distance>
        //
        // Here, we just count the symbols and minus 2; there is a RuleHead symbol at one side of each 
        // rule, and a RuleTail symbol at the other, to allow for more efficient navigation. As such,
        // they are ignored in the count.
        //
        symbol_total += it->second->end().second - 1;
        ++it;
        }

    //
    // a few print commands are included to quikly visualise the rules created:
    //
    s2.printRules(fout);
	//s2.printSequence();
	unsigned int nonRep = s2.findTerminals();
	unsigned int nNew = s2.findNew(rule_count-1);
	unsigned int nHead = s2.findHead(rule_count - 1);

    s2.markSymbols(theVector, fout);
    fout.close();
    cout << "\ntotal symbols inserted: " << count << endl;
    cout << "symbols used in sequitur: " << symbol_total << endl;
    cout << "rules created: " << rule_count << endl;

	cout << "non repetitions: " << nonRep << endl;
	cout << "new: " << nNew << endl;
	cout << "head: " << nHead << endl;
	cout << "oppurtunity: " << count-(nonRep+nNew+nHead) << endl;

  /*  char a;
	cin >> a;*/
    return 0;
}


