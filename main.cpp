//#include <QtCore/QCoreApplication>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <limits.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <locale>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <deque>
#include<vector>


using namespace std;
char* decimal_binary(long long int n)  /* Function to convert decimal to binary.*/
{
        char isbinary[32] = " ";
        long long int rem, i = 0;
        long long int binary = 0;

        for (int c = 31; c >= 0; c--)
        {
                long long int  k = n >> c;

                if (k & 1)
                        isbinary[31 - c] = '1';
                else
                        isbinary[31 - c] = '0';
        }

        isbinary[31] = '\0';
        return isbinary;
}



long long int miss = 0;
long long int hit = 0;
long double access = 0;
long double missRate;
long double hitRate;

int main()
{
/* string test=" ";
 test=decimal_binary(128);
    cout<<"test"<<test<<endl;*/

long long int CacheSize =131072;//65536;//6438912;//1048576; //524288;//262144;//524288; //65536; // bits

#define BlockSize  1024 //bits
#define way  8
#define AddressSize 32

        enum replacementPolicy {random, fifo, lru};
        int r =lru;
        long double blockNumber;
        long int offset;
        long int index;
        long int tagBits;
        long double SetNumber;
        int c=1;// //variable for scheduling fifo 1

        typedef struct
        {
                int valid;
                unsigned long long int tag;
                unsigned long long int block;
                int a; //variable for scheduling lru 1
                int b;//variable for scheduling fifo 2
        } line;

        typedef struct
        {
                line *cacheLine;
                vector<int> myvector;//for scheduling lru 2

        }Set;


        typedef struct
        {

                Set *CacheSet;

        }Cache_t;


        Cache_t cache;


        blockNumber = CacheSize / BlockSize; // cacheSize doesn't include index & tag

    //    cout<<"blocknumber"<<blockNumber<<endl;
        SetNumber = blockNumber / way;
      //  cout<<"setnumber"<<SetNumber<<endl;
        offset = log2(BlockSize/8);
     //   cout<<"offset"<<offset<<endl;
        index = log2(SetNumber);
       // cout<<"index"<<index;
        tagBits = AddressSize - (index + offset);
       // cout<<"tagbits"<<tagBits;

        cache.CacheSet = (Set*)malloc(SetNumber * sizeof(Set));

        for (int i = 0; i < SetNumber; i++)
        {
                cache.CacheSet[i].cacheLine = (line*)malloc(sizeof (line)* way);
        }
        for(int i=0; i<SetNumber; i++){
        for (int j = 0; j<way; j++){
            cache.CacheSet[i].cacheLine[j].a = j;
            cache.CacheSet[i].cacheLine[j].valid = 0;

        }}

        ///////
        int counter=0;
        ifstream myfile("LIB_L2_s.txt");
        string kernel=" ";
        myfile>>kernel;
        char first_kernel;
        first_kernel=kernel[11];
        while (!myfile.eof())
        {

            int j;
            string input=" ";
            char address[40]=" ";
            string address1=" ";
               myfile >> input;
              // cout<<"inout is : "<<input<<endl;
               if(input[0]=='k')
               {

                   if(input[11]==first_kernel);
                   else
                   {
                        cout<<"Kernel  : "<<first_kernel<<"\n";
                       first_kernel=input[11];

                       cout<< "access number= " << access <<" \n";
                       cout<< "hit number= " << hit << " \n";
                       cout<< "miss number= " << miss << " \n";
                       cout << "hit rate= " << (hit/access)*100<< "% \n";
                       cout << "miss rate= " << (miss/access)*100<< "% \n"<<"---------------------------------------------\n";

                       miss=0;
                       hit=0;
                       access=0;
                       free( cache.CacheSet);
                        cache.CacheSet = (Set*)malloc(SetNumber * sizeof(Set));
                        for (int i = 0; i < SetNumber; i++)
                        {
                            free(cache.CacheSet[i].cacheLine) ;
                        }
                        for (int i = 0; i < SetNumber; i++)
                        {
                            cache.CacheSet[i].cacheLine = (line*)malloc(sizeof (line)* way);
                        }
                        for(int i=0; i<SetNumber; i++){
                            for (int j = 0; j<way; j++){
                                cache.CacheSet[i].cacheLine[j].a = j;
                            }}

                   }
               }

                if (input[5] == 'a')
                {
                        access++;
                        j = 10;
                        while (j<(input.length())){

                                address[j - 10] = input[j];
                                j++;
                        }
                        address[j - 10] = '\0';
                        // cout<<input<<endl;
                        address1 = address;
                        long long int Result;
                        istringstream buffer(address1);
                        buffer >> Result;
                        //  cout<<address<<endl;
                        // cout<<Result<<endl;
                        string binary = " ";
                        binary = decimal_binary(Result);
                       // cout<<"add "<<binary<<endl;
                        counter++;
                      //  cout<<"counter: "<<counter<<endl;

                        //////---------------------------------------Find Index---------------------------///////

                        long long int IndexNumber=0;

                        for (int k = 0; k<index; k++){

                            if(binary[tagBits +k]== '1')
                                IndexNumber = IndexNumber + pow(2,index - k -1);}//
                       /// cout<<"indexnum "<<IndexNumber<<endl;

                        /////////---------------------------- find tag-----------------------------//////////
                        long long int TagNumber=0;
                        for (int k = 0; k<tagBits; k++){
                           // cout<<"tagbit "<<binary[k]<<endl;
                            if(binary[k] == '1'){
                                int p= tagBits - k - 1;
                                long long int pp= pow(2,p);
                                TagNumber =  TagNumber + pp;
                        }}

                      //  cout<<"Tag "<<TagNumber<<endl;

                      ///////////-----------------------------search-------------------------------/////////

                        bool find = 0;
                        for (int numberOfway = 0; numberOfway<way; numberOfway++){
                        if (cache.CacheSet[IndexNumber].cacheLine[numberOfway].tag == TagNumber)
                        {
                                hit++;
                                find = 1;

                                if(r==lru){


                             //   std::deque<int>::iterator it = cache.CacheSet[IndexNumber].lrudeque.begin();
                                 ///cout<<cache.CacheSet[IndexNumber].myvector.size();
                                for(int j=0; j <= cache.CacheSet[IndexNumber].myvector.size() ; j++){

                                  //  cout<<"hello"<<endl;
                                    //cout<<cache.CacheSet[IndexNumber].lrudeque[j]<<endl;
                                    if((int)(cache.CacheSet[IndexNumber].myvector[j]) ==(int)(cache.CacheSet[IndexNumber].cacheLine[numberOfway].a)){
                                    //it=it+j;

                                    //cache.CacheSet[IndexNumber].lrudeque.erase(it);
                                    cache.CacheSet[IndexNumber].myvector.erase (cache.CacheSet[IndexNumber].myvector.begin()+j);
                                    cache.CacheSet[IndexNumber].myvector.push_back(cache.CacheSet[IndexNumber].cacheLine[numberOfway].a);
                                    }

                                                                        break;
                                                                } }  // lru



                                break;



                        } }


                        if (find == 0)


                     ////////---------------------------replacement-------------------------///////////////
                        {
                            int whereindex;
                                miss++;
                                bool v=0;
                                for (int numberOfway = 0; numberOfway<way; numberOfway++){
                                if (cache.CacheSet[IndexNumber].cacheLine[numberOfway].valid == 0){
                                    v=1;
                                    cache.CacheSet[IndexNumber].cacheLine[numberOfway].tag = TagNumber;
                                    cache.CacheSet[IndexNumber].cacheLine[numberOfway].valid =1;
                                    cache.CacheSet[IndexNumber].cacheLine[numberOfway].b= c;//fifo
                                    c++;

                                     //cout<<"in "<<cache.CacheSet[IndexNumber].cacheLine[numberOfway].a<<endl;
                                    cache.CacheSet[IndexNumber].myvector.push_back(cache.CacheSet[IndexNumber].cacheLine[numberOfway].a);
                                   //std::cout<<cache.CacheSet[IndexNumber].myvector.back()<<endl;
                                     //cout<<"in2 "<<cache.CacheSet[IndexNumber].cacheLine[numberOfway].a<<endl;
                                  //  cout<<"noeee"<<endl;

                                    break;

                                }}// valid == 0
                                if(v==0){

                                        switch (r){
                                        case(random):{
                                                        long long int randLine;
                                                        randLine = rand() % way + 1;
                                                        cache.CacheSet[IndexNumber].cacheLine[randLine].tag = TagNumber;
                                        }
                                        case(fifo) : {
                                            int min;
                                            min=cache.CacheSet[IndexNumber].cacheLine[0].b;
                                            whereindex=0;
                                            for(int i=1; i<=way; i++){
                                            if(min> cache.CacheSet[IndexNumber].cacheLine[i].b){
                                            min=cache.CacheSet[IndexNumber].cacheLine[i+1].b;}
                                            whereindex=i;}
                                           cache.CacheSet[IndexNumber].cacheLine[whereindex].tag = TagNumber;
                                           cache.CacheSet[IndexNumber].cacheLine[whereindex].b=c;
                                            c++;
                                            }
                                        case(lru):{


                                            cache.CacheSet[IndexNumber].cacheLine[cache.CacheSet[IndexNumber].myvector.front()].tag = TagNumber;

                                            cache.CacheSet[IndexNumber].myvector.push_back(cache.CacheSet[IndexNumber].cacheLine[cache.CacheSet[IndexNumber].myvector.back()].a);
                                            cache.CacheSet[IndexNumber].myvector.erase(  cache.CacheSet[IndexNumber].myvector.begin());

                                                }//case lru
                                        }//switch replacementPolicy

                                }// valid == 1

                       }// replacemnet

                }// access

        }// while file
         cout<<"Kernel  : "<<first_kernel<<"\n";
         cout<< "access number= " << access <<" \n";
         cout<< "hit number= " << hit << " \n";
         cout<< "miss number= " << miss << " \n";
         cout << "hit rate= " << (hit/access)*100<< "% \n";
         cout << "miss rate= " << (miss/access)*100<< "% \n"<<"---------------------------------------------\n";

}//main
