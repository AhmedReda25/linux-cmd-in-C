#include "help.h"


int main(int argc, char **argv)
{
	long int n = 10; //print 10 lines by default
	
	if(strcmp(argv[1], "-n") == 0) /*option before files*/
	{
		n = str_to_int(argv[2]); //get line count out of the option
		
		for(int i = 3; i != argc; i++) //run head on all files
		{
			tail_print(argv[i], n);
			printf("\n   =================   \n");
		}
	}
	else if(strcmp(argv[argc - 2],"-n") == 0) /*options after files*/
	{
		n = str_to_int(argv[argc - 1]); //get line count out of the option
		
		for(int i = 1; i != argc - 2; i++) //run head on all files
		{
			tail_print(argv[i], n);
			printf("\n   =================   \n");
		}
	}
	else /*no options*/
	{
		for(int i = 1; i != argc; i++) //run head on all files
		{
			tail_print(argv[i], n);
			printf("\n   =================   \n");
		}
	}
	
	return 0;
}