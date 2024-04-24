#include "help.h"

int main(int argc, char **argv)
{
	if(strcmp(argv[1], "-i") == 0)
	{
		for(int i = 3; i < argc; i++)
			grep_icase(argv[2], argv[i]); // pattern->argv[2] , then files
	}
	else if(strcmp(argv[argc - 1], "-i") == 0)
	{
		for(int i = 2; i < argc-1; i++)
			grep_icase(argv[1], argv[i]); // pattern files -i
	}
	else /* no options */
	{
		for(int i = 2; i < argc; i++)
			grep_main(argv[1], argv[i]);
	}
	
	return 0;
}