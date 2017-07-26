#include "random/pcg_basic.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>


#define ROWHEIGHT 3


pcg32_random_t rng;

/*
	weighted dice
	Picks a random item from a weighted distribution
	input: Expected array format  [numItems, totalOfWeights, item1Wight, item2Weight, ...]
	output: the index of the item selected

*/
int weightedDice(int* dist)
{
	int pick;

	int dice = pcg32_boundedrand_r(&rng, dist[1]);
	int checkpoint=0;

	for (pick = 2; pick<=(dist[0]+1); pick++)
	{
			checkpoint += dist[pick];
			if (dice < checkpoint) break;
	}
	return pick-2;
}

/*
	printScreen
	shows the current reel array	
*/

void printScreen(int (*finalReelStop)[ROWHEIGHT])
{
	int reel=0;
	int row=0;

	for (row=0; row<3; row++)
	{
		for (reel=0; reel<5; reel++)
		{
			printf("%3i ", finalReelStop[reel][row]);
		}
		printf("\n");		
	}
	printf("---------------\n");
}

int main(int argc, char** argv)
{
	//Create and seed the rng
	
	int     rounds = atoi(argv[0]);
    pcg32_srandom_r(&rng, time(NULL) ^ (intptr_t)&printf,      (intptr_t)&rounds);
	//rngResult = pcg32_boundedrand_r(&rng, 5);

    const int NUMTRIALS = 1000000;

	std::unordered_map<int, int> winStats;
	int rngResult = 0;


	for (int trial=0; trial<NUMTRIALS; trial++)
	{

		rngResult = pcg32_boundedrand_r(&rng, 5);



		if (winStats.count(rngResult) == 0)	winStats.insert({rngResult, 1});
		else winStats[rngResult]++;
		
	}


	//Write to an output file

	std::ofstream myfile;
	std::ostringstream os;
	os << "./output/output_"<<time(NULL)<<".txt";
	std::string filename = os.str();

  	myfile.open (filename.c_str());

	for (std::pair<int, int> element : winStats)
  		{
  			myfile << element.first << ";" << element.second << '\n';
  		}

  	myfile.close();


}

