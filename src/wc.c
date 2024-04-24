#include "help.h"


int main(int argc, char **argv)
{
	if(argc == 1){
		printf("File name required");
		return -1;
	}
	
	wc_data_t data; /* a struct that combine counted elements */
	
	if(argv[1][0] == '-') /* Options first */
	{
		for(int i = 2; i != argc; ++i){
			wc_process(argv[i], argv[1]+1, &data);
		}
	}
	else if(argv[argc-1][0] == '-') /* files first */
	{
		for(int i = 1; i != argc-1; ++i){
			wc_process(argv[i], argv[argc-1]+1, &data);
		}
	}
	else /* no options */
	{
		for(int i = 1; i != argc; i++)
			wc_noopts(argv[i], &data);
	}
	
	
	return 0;
}