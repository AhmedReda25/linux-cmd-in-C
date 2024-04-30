#include "help.h"


int main (int argc, char **argv){
	
	if(argc == 1) {printf("Missing file operand\n"); return -1;}
	
	FILE *fp;
	
	for(int i = 1; i < argc; i++){
		fp = fopen(argv[i], "w");
		if(fp != NULL) fclose(fp);
	}
	
	return 0;
	
}
