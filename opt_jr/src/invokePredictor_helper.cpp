
#include "invokePredictor_helper.hh"
#include "debugmessage.hh"

#include <string>
#include <sys/types.h>
//#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <glob.h>

#define BIG_LINE 4000
#define BIG_TEXT 20000



/*
 * 		Name:					readFolder
 * 		Input parameters:		A path to a folder
 * 		Output parameters:		The name of subfolder contained in the folder corresponding to the folder in "path"
 * 		Description:			It's an helper function used by invoke predicttor; this function returns
 * 											the first subFolder in the folder corresponding to "path"
 */

char * readFolder(char *  path)
{
	 struct dirent *de;
	    DIR *dr = opendir(path);
	    if (dr == NULL)
	    {
	        printf("readFolder failure (%s)", path );
	        exit(-1);
	    }
	    while ((de = readdir(dr)) != NULL)
	    {
	    	if (de->d_type == 4) // folder
	    	{
	    		if (
	    				strcmp(de->d_name, ".") != 0 &&
							strcmp(de->d_name, "..")
	    			)
	    		return de->d_name;
	    	}
	    }
	    closedir(dr);
	    return NULL;
}



void writeFile(const char *filepath, const char *data)
{
    FILE *fp = fopen(filepath, "w");
    if (fp != NULL)
    {
        fputs(data, fp);
        fclose(fp);
    }
}



char * ls(char * pattern, optJrParameters &par)
{
	//char debugMsg[DEBUG_MSG];
	std::string debugMsg;
	glob_t pglob;
	char *filename = (char *)malloc(1024);
	 int outcome = glob(pattern, GLOB_ERR, NULL, &pglob);
	 if (pglob.gl_pathc == 1)
	 	  {
				debugMsg="ls= "; debugMsg+=  pglob.gl_pathv[0]; debugMessage(debugMsg, par);
	 		  strcpy(filename, pglob.gl_pathv[0]);
	 		  globfree(&pglob);
	 		  return filename;
	 	  }
	 	  else
	 	  {
	 		  switch(outcome)
	 		  {
	 		  	  case GLOB_NOSPACE:
	               printf("Fatal error: ls: running out of memory: %s\n", pattern);
	               exit(-1);
	 	 	 	 	 break;
	 		  	  case GLOB_ABORTED:
	               printf("Fatal error: ls: a read error has occurred%s\n", pattern);
	               exit(-1);
	               break;
	 		  	  case GLOB_NOMATCH:
	               printf("Fatal error: ls: no matches found%s\n", pattern);
	               exit(-1);
	 		  	  default:
	    	  	   printf("Fatal error: ls: unknown error%s\n", pattern);
	    	  	   exit(-1);
	 		  }
	 	  }
	  return NULL;
}

// extractRowN:  used by replace
char * extractRowN(char *text, int row)
{
	int len = strlen(text);
	int iText, iLine = 0;
	char * line = (char *)malloc(BIG_LINE);

	if (line == NULL)
	{
		printf("Malloc failure: edxtractRowN\n");
		exit(-1);
	}

	int countRow = 0;

	iText = 0;

	strcpy(line, "");

	while ( countRow < row && iText < len)
	{
		iLine = 0;
		while(text[iText] != '\n' &&
				iText < strlen(text))
		{
			line[iLine++] = text[iText++];
		}
		countRow++;
		iText++;
	}

	if (row > countRow) return "stop";
	line[iLine] = '\0';

	if (line == NULL)
	{
		printf("Fatal error: extractRowN: returned string cannot be NULL\n");
		exit(-1);
	}
	return line;
}


char * replace(char * text, char *newLine)
{

	int lineCount;
	char line[BIG_LINE];
	char *newText = (char *)malloc(BIG_TEXT);
	if (newText == NULL)
	{
		printf("Malloc failure: replace\n");
		exit(-1);
	}
	lineCount = 1;
	strcpy(newText, "");
	strcpy(line, extractRowN(text, lineCount));
	while ( strcmp(line, "stop") != 0)
	{
		if (strstr(line, "Nodes") != NULL) strcat(newText, newLine);

		else strcat(newText, line);

		strcat(newText, "\n");

		lineCount++;

		strcpy(line, extractRowN(text, lineCount));

	}
	return newText;
}


char * readFile(char * filename)
{
	   struct stat sb;
	   stat(filename, &sb);
	   FILE *fp=fopen(filename, "r");
	   if (fp == NULL)
	   {
		   printf("Fatal error: readFile: could not open %s\n", filename);
		   exit(-1);
	   }
	   char *str=(char*)malloc(sb.st_size+1);
	   if (str == NULL)
	   {
		   printf("Malloc failure: readFile\n");
		   exit(-1);
	   }
	   fread(str, 1, sb.st_size, fp);
	   fclose(fp);
	   str[sb.st_size]=0;
	   return str;
}

/*

 * 		Name:					_run

 * 		Input parameters:		char * cmd

 * 		Output parameters:		The output provided by the executed command

 * 		Description:			This function executes a command ("cmd")

 *

 */

char *_run(char * cmd, optJrParameters &par)

{
	int BUFSIZE = 10240;
	//char debugMsg[DEBUG_MSG];
	std::string debugMsg;
	int outcome;
	char *buf = (char *)malloc(BUFSIZE);
	if (buf == NULL)
	{
		printf("Malloc failure: _run\n");
		exit(-1);
	}
	    FILE *fp;
	    if ((fp = popen(cmd, "r")) == NULL) {
	        printf("Fatal Error: _run: %s _cmd %s (%d)\n", strerror(errno), cmd, errno);
	        exit(-1);
	    }
	    while (fgets(buf, BUFSIZE, fp) != NULL) {
	        printf("OUTPUT: %s", buf);
	    }
	    outcome = pclose(fp);

	    if(outcome == -1)  {
	        printf("Fatal error: Command %s not found or exited with error status\n", cmd);
	        exit(-1);
	    } else
				debugMsg ="_run has returned";debugMsg+=outcome;debugMsg+=" status"; debugMessage(debugMsg, par);
	  return buf;

}


char * extractWord(char * line, int pos)
{

	char *word = (char *)malloc(64);

	if (word == NULL)
	{
		printf("Malloc failure: extractWord\n");
		exit(-1);
	}

	int lineIndex = 0;
	int wordIndex = 0;
	int len = strlen(line);
	int countwords = 0;

	while (lineIndex <= len)
	{
		if (line[lineIndex] != '\t') word[wordIndex++] = line[lineIndex++];
		else
		{
			countwords++;
			if (countwords == pos)
			{
				word[wordIndex] = '\0';
				break;
			}
			wordIndex = 0;
			lineIndex++;
		}
	}

	if (word == NULL)
	{
		printf("Fatal error: extracWord: returned string is NULL\n");
		exit(-1);
	}
	return word;
}

char * extractRowMatchingPattern(char *text, char *pattern)
{

	char * line = (char *)malloc(1024);

	if (line == NULL)
	{
		printf("Malloc failure: extractRowMatchingPattern\n");
		exit(-1);
	}

	line = strstr(text, pattern);
	if (line == NULL)
	{
		printf("Fatal error: extractRowMatchingPattern: pattern %s was not found in predictor output file\n", pattern);
		exit(-1);
	}
	line = line + strlen(pattern) + 1;

	return line;
}

/*
 * 		Name:					doubleCompare
 * 		Input parameters:		double a, double b
 * 		Output parameters:		0  if a = b
 * 								-1 if a < b
 * 								1  if a > b
 * 		Description:			Compare two doubles according to a certain precision (epsilon)
 *
 */

int doubleCompare(double a, double b)
{
	  if (((a - epsilon) < b) && ((a + epsilon) > b))
	   {
	    return 0;
	   }
	  else
	   {
	    if (a > b) return 1; else return -1;
	   }
}