#include "help.h"

int main(int argc, char **argv)
{
	if(argc == 1)
	{
		ls_no_options(".");
	}
	else if(argc == 2) /*Another directory without options*/
	{
		// ls_no_options(argv[1]);
		if(argv[1][0] == '-')
			ls_with_options(".", argv[1]);
		else
			ls_no_options(argv[1]);
	}
	else if(argc == 3)
	{
		//ls_with_options(argv[1], argv[2]); // assuming the format -> ls dir [options]
		if(argv[1][0] == '-') //options passed first
			ls_with_options(argv[2], argv[1]);
		else
			ls_with_options(argv[1], argv[2]);
	}
}