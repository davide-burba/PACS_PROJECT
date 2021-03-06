#include "opt_jr_parameters_helper.hh"



/**
Usage explains the usage of the OPT_JR_CPP program.
*/
void Usage(int argc)
{
		printf("Arguments expected %d, actual arguments number was %d\n", ARGS, argc);
    	printf("Usage:\n");
    	printf("./OPT_JR_CPP -f <filename.csv> -n <N> -k <Limit> -d <Y/y|N/n> -c <Y/y|N/n> -g=<Y/y|N/n> -i <iterations> -st<a/A|s/S>\n");
    	printf("where:\n");
    	printf("<filename.csv> is the csv file (including the input values) under $UPLOAD_HOME in wsi_config.xml;\n");
    	printf("<N> is the total number of cores;\n");
    	printf("<Limit> is the maximum number of considered candidates (if equal to 0, all the candidates are considered).\n");
    	printf("-d represents debug (on/off)\n");
    	printf("-c represents cache (on/off)\n");
			printf("-i represents the maximum number of iterations\n");
    	printf("-st represents the type of local search (separing/alterning)\n");
			printf("EXAMPLE:\n");
    	printf("./OPT_JR_CPP -f=\"Test1.csv\" -n=150  -i=10 -k=0 -d=n -c=y -g=y -st=s\n");
    	printf("OPT_JR is executed on a file Test1.csv, the total number of cores is 150, all the candidates are considered, "
    			"the maximum number of iterations is 10, the cache is enabled, the calculation of the globabl objective function is enabled, "
					"the type of local search is Separing\n");
    	exit(-1);
    }


/*
Function to parse argument from command line;
Invoked by Opt_jr_parameters constructor
*/

char* parseArg(char* string, char* gap, int type, int argc)
{

        switch(type)
        {
          case NUMBER:
          case STRING:
            return string + strlen(gap);
            break;

          case YES_NO:

            if (strstr(string + strlen(gap), "y") || strstr(string + strlen(gap), "Y")) return (char*)"1";

            else if (strstr(string + strlen(gap), "n") || strstr(string + strlen(gap), "N")) return (char*)"0";

              else Usage(argc);

            break;

          default:

            printf("Fatal Error: parseArg: option not recognized\n");
            exit(-1);
        }

        return NULL;

}
