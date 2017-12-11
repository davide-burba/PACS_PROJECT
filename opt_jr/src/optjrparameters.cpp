
#include "optjrparameters.hh"
#include "optjrParam_helper.hh"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 Constructor
 */

optJrParameters::optJrParameters (char **args, int argc)
{

	if (argc != ARGS) Usage(argc);
	for (int i = 0; i < ARGS; i++)
		if (strstr(args[i], FILENAME)) filename=parseArg(args[i], (char*)FILENAME, STRING, argc);//strcpy(filename, parseArg(args[i], FILENAME, STRING));

		else if (strstr(args[i], NUM_N)) number = atoi(parseArg(args[i], (char*)NUM_N, NUMBER, argc));

				else if (strstr(args[i], LIST_LIMIT)) K = atoi(parseArg(args[i], (char*)LIST_LIMIT, NUMBER, argc));

					else if (strstr(args[i], DEBUG)) debug = atoi(parseArg(args[i], (char*)DEBUG, YES_NO, argc));


							else if (strstr(args[i], SIMULATOR))
							{
									if (strcmp(parseArg(args[i], (char*)FILENAME, STRING, argc), "dagSim") == 0) simulator = DAGSIM;
															else simulator = LUNDSTROM;
							}

								else if (strstr(args[i], GLOBAL_FO_CALCULATION)) globalFOcalculation = atoi(parseArg(args[i], (char*)GLOBAL_FO_CALCULATION, YES_NO, argc));
								else if (strstr(args[i], MAX_ITERATIONS))
									maxIterations = atoi(parseArg(args[i], (char*)MAX_ITERATIONS, NUMBER, argc));
								else if (strstr(args[i], CACHE))
									cache = atoi(parseArg(args[i], (char*)CACHE, YES_NO, argc));



}


void optJrParameters::set_numberOfThreads(sConfiguration &configuration)
{
	/* Read the threads number */
	numberOfThreads = stoi(configuration["THREADS_NUMBER"]);

}



/*
	Get Functions
*/
const std::string optJrParameters::get_filename()
{
		return filename;
}

const int optJrParameters::get_debug()
{
	return debug;
}

const int optJrParameters::get_cache()
{
	return cache;
}

const int optJrParameters::get_globalFOcalculation()
{
	return globalFOcalculation;
}

const int optJrParameters::get_K()
{
	return K;
}

const int optJrParameters::get_simulator()
{
	return simulator;
}

const int optJrParameters::get_number()
{
	return number;
}


const int optJrParameters::get_maxIteration()
{
	return maxIterations;
}


const int optJrParameters::get_numberOfThreads()
{
	return numberOfThreads;
}
