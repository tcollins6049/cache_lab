#include <fstream>
#include <iostream>
#include <string>
#include <assert.h>
#include "Line.h"
#include "Set.h"
#include "Cache.h"
#include "Simulate.h"
#include <stdio.h>
#include <string.h>

//TO DO: implement run and accessCache functions
//Look for TODO 

/* Simulator constructor
 * Dynamically create a Cache object and initialize the Simulator object.
 */

Simulate::Simulate(int32_t associativity, int32_t blockOffsetBits, 
                   int32_t setIndexBits, bool verbose)
{
   hits = misses = evictions = 0;
   cacheP = new Cache(associativity, blockOffsetBits, setIndexBits, verbose);
   this->verbose = verbose;
}

/*
 * Destroy the Simulator object.
 */
Simulate::~Simulate()
{
   delete cacheP;
}

void Simulate::run(std::string filename)
{
   std::fstream fs;
   fs.open(filename, std::fstream::in);
   
   //TODO
   //Loop, reading lines from the file.
   //From each line, get the type and the address.
   //Depending upon the type (L or S), you'll pass it to the
   //accessCache method. (See assignment.)
   //If the verbose flag is true, print out the input line (see csim-ref).
   std::string line;
   while(std::getline(fs, line)) {
      if (line[0] == 32) {
         char type = line[1];

         int n = line.length();
         char array[n+1];
         strcpy(array, line.c_str());
         std::string address = "";

         for (int i = 3; i < line.length() && line[i] != 44; ++i) {
            address += array[i];
         }
         uint64_t temp = std::stol(address, NULL, 16);
         std::cout << temp << "\n";

         if (type == 'L' || type == 'S') {
            accessCache(temp);
         } else if (type == 'M') {
            accessCache(temp);
            accessCache(temp);
         }

         if (verbose) {
            printSummary();
         }
         
      }
   }

}

/* accessCache
 * Takes as input a 64 bit address.
 * Uses the address to access the cache pointed to by cacheP
 * and updates hits, misses, and evictions.
 */
void Simulate::accessCache(int64_t address)
{
   //TODO
   //cacheP contains a pointer to the cache object
   //Call the appropriate methods in the Cache class
   //to determine whether to increment hits, misses, and/or evictions
   //If verbose is true, output the verbose output (see assignment)
   if (cacheP->isHit(address)) {
      hits++;
   } else {
      if (cacheP->isEvict(address)) {
         evictions++;
      }
      misses++;
   }
   cacheP->update(address);

   if (verbose) {
      //printSummary();
      cacheP->print();
   }
}

/*
 * printSummary
 * Print the number of hits, misses, and evictions at the end of
 * the simulation.
 */
void Simulate::printSummary()
{
   std::fstream fs;
   printf("hits:%d misses:%d evictions:%d\n", hits, misses, evictions);
   fs.open(".csim_results", std::fstream::out);
   assert(fs.is_open());
   fs << hits << " " << misses << " " << evictions <<"\n";;
   fs.close();
}

