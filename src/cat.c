#include <stdio.h>

int main(int argc, char **argv)
{
	if(argc >= 2)
	{
		for(int i = 1; i != argc; i++)
		{
			FILE *fp = fopen(argv[i],"r");
			char buffer[200];
			while((fgets(buffer, sizeof(buffer),fp)) != NULL)
				printf("%s",buffer);
			fclose(fp);
		}
		return 0;
	}
	printf("file name required\n");
	return -1;
}