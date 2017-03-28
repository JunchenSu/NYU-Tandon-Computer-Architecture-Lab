/*
Cache Simulator
Level one L1 and level two L2 cache parameters are read from file (block size, line per set and set per cache).
The 32 bit address is divided into tag bits (t), set index bits (s) and block offset bits (b)
s = log2(#sets)   b = log2(block size)  t=32-s-b
*/
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <stdlib.h>
#include <cmath>
#include <bitset>

using namespace std;
//access state:
#define NA 0 // no action
#define RH 1 // read hit
#define RM 2 // read miss
#define WH 3 // Write hit
#define WM 4 // write miss




struct config{
       int L1blocksize;
       int L1setsize;
       int L1size;
       int L2blocksize;
       int L2setsize;
       int L2size;
       };

/* you can define the cache class here, or design your own data structure for L1 and L2 cache*/
class Cache {
      public: 
        int ways;
        int tagbit;
        int indexbit;
        int offsetbit;
        int totallength;
		int setnumber;
		int counter = 0;
        Cache(int blocksize, int setsize, int size){
            totallength = (int)(log(size)/log(2));
            offsetbit = (int)(log(blocksize)/log(2));
            ways = (int)(log(setsize)/log(2));
			indexbit = totallength - offsetbit - ways;
			setnumber = (int)(log(indexbit) / log(2));
            tagbit = 32 - indexbit - offsetbit;

			unsigned long** C = new unsigned long*[ways];//build dynamic array to store tag bit in decimal
			for (int i = 0; i < ways; ++i)
				C[i] = new unsigned long[setnumber];
        }

		int getSetNumber(bitset<32> accessaddr, int offsetbit, int indextbit) {//use this function to get the decimal index bit of an address
			int tmp = 0;
			for (int i = 0; i<indextbit; ++i) {
				if (accessaddr[offsetbit + i] == 1) tmp += (int)pow(2, i);
			}
			return tmp;
		}

		int getTag(bitset<32> accessaddr, int offsetbit, int indexbit) {//use this function to get the decimal tag bit of an address
			int tmp = 0;
			for (int i = 0; i<tagbit; i++) {
				if (accessaddr[offsetbit + indexbit + i] == 1) tmp += (int)pow(2, i);
			}
			return tmp;
		}

		bool Check()
      };
 





int main(int argc, char* argv[]){
    
    config cacheconfig;
    ifstream cache_params;
    string dummyLine;
    cache_params.open(argv[1]);
    while(!cache_params.eof())  // read config file
    {
      cache_params>>dummyLine;
      cache_params>>cacheconfig.L1blocksize;
      cache_params>>cacheconfig.L1setsize;              
      cache_params>>cacheconfig.L1size;
      cache_params>>dummyLine;              
      cache_params>>cacheconfig.L2blocksize;           
      cache_params>>cacheconfig.L2setsize;        
      cache_params>>cacheconfig.L2size;
      }
    
  
  
   // Implement by you: 
   // initialize the hirearch cache system with those configs
   // probably you may define a Cache class for L1 and L2, or any data structure you like
    Cache L1(cacheconfig.L1blocksize,cacheconfig.L1setsize,cacheconfig.L1size);
    Cache L2(cacheconfig.L2blocksize,cacheconfig.L2setsize,cacheconfig.L2size);
   
   
   
  int L1AcceState =0; // L1 access state variable, can be one of NA, RH, RM, WH, WM;
  int L2AcceState =0; // L2 access state variable, can be one of NA, RH, RM, WH, WM;
   
   
    ifstream traces;
    ofstream tracesout;
    string outname;
    outname = string(argv[2]) + ".out";
    
    traces.open(argv[2]);
    tracesout.open(outname.c_str());
    
    string line;
    string accesstype;  // the Read/Write access type from the memory trace;
    string xaddr;       // the address from the memory trace store in hex;
    unsigned int addr;  // the address from the memory trace store in unsigned int;        
    bitset<32> accessaddr; // the address from the memory trace store in the bitset;
    
    if (traces.is_open()&&tracesout.is_open()){    
        while (getline (traces,line)){   // read mem access file and access Cache
            
            istringstream iss(line); 
            if (!(iss >> accesstype >> xaddr)) {break;}
            stringstream saddr(xaddr);
            saddr >> std::hex >> addr;
            accessaddr = bitset<32> (addr);
           
           
           // access the L1 and L2 Cache according to the trace;
              if (accesstype.compare("R")==0)
              
             {    
                 //Implement by you:
                 // read access to the L1 Cache, 
                 //  and then L2 (if required), 
                 //  update the L1 and L2 access state variable;
				  int setnum = L1.getSetNumber(accessaddr, L1.offsetbit, L1.indexbit);
				  int tagnum = L1.getTag(accessaddr, L1.offsetbit, L1.indexbit);
                 
                 
                 
                 
                 
                 
                 
                 }
             else 
             {    
                   //Implement by you:
                  // write access to the L1 Cache, 
                  //and then L2 (if required), 
                  //update the L1 and L2 access state variable;
                  
                  
                  
                  
                  
                  
                  }
              
              
             
            tracesout<< L1AcceState << " " << L2AcceState << endl;  // Output hit/miss results for L1 and L2 to the output file;
             
             
        }
        traces.close();
        tracesout.close(); 
    }
    else cout<< "Unable to open trace or traceout file ";


   
    
  

   
    return 0;
}
