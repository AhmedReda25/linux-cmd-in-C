#include "help.h"

long int line_count(FILE *fp)
{
	char c;
	long count = 0;
	
	while((c = fgetc(fp)) != EOF)
	{
		if(c == '\n')
			count++;
	}
	return count;
}



static void decode_perms(stat_t *st)
{
	char perm[11] = "";
	
	/*File type*/
	if(S_ISDIR(st->st_mode))
		perm[0] = 'd';
	else if(S_ISCHR(st->st_mode))
		perm[0] = 'c';
	else if(S_ISBLK(st->st_mode))
		perm[0] = 'b';
	else
		perm[0] = '-';


	/*Permissions*/
	(st->st_mode & S_IRUSR) ?  (perm[1] = 'r') : (perm[1] = '-');
	(st->st_mode & S_IWUSR) ?  (perm[2] = 'w') : (perm[2] = '-');
	(st->st_mode & S_IXUSR) ?  (perm[3] = 'x') : (perm[3] = '-');
	(st->st_mode & S_IRGRP) ?  (perm[4] = 'r') : (perm[4] = '-');
	(st->st_mode & S_IWGRP) ?  (perm[5] = 'w') : (perm[5] = '-');
	(st->st_mode & S_IXGRP) ?  (perm[6] = 'x') : (perm[6] = '-');
	(st->st_mode & S_IROTH) ?  (perm[7] = 'r') : (perm[7] = '-');
	(st->st_mode & S_IWOTH) ?  (perm[8] = 'w') : (perm[8] = '-');
	(st->st_mode & S_IXOTH) ?  (perm[9] = 'x') : (perm[9] = '-');

	printf("%s  ", perm);
}

static void print_file_attrs(dirent_t *entry, stat_t *st, int sizeF, int inode_state, int lng)
{
	size_t siz;
	char size_unit; 
	if(sizeF)
	{
		/*Formatting file size to a human-readable format (rather than bytes)*/
		if(st->st_size > 1024 && st->st_size < (1024 * 1024)) /*less than 1M*/{
			siz = (st->st_size / 1024) + 1;
			size_unit = 'K';
		}else if(st->st_size > (1024 * 1024)){
			size_unit = 'M';
			siz = (st->st_size / (1024 * 1024)) + 1;
		}else if(st->st_size < 1024){
			siz = 1;
		}
		
	}else{
		siz = st->st_size;
	}
	
	if(lng)
	{
		decode_perms(st);
		printf("%lu  %u  %u %lu", st->st_nlink, st->st_uid, st->st_gid, siz);
		
		if(sizeF)
			printf("%c  ", size_unit);
		
		if(inode_state)
			printf("%lu  ", st->st_ino);
	
	}else if(inode_state) /*non-long format but inode required*/
		printf("%lu  ", st->st_ino);
	
	printf("  %s\n",entry->d_name);
}

static void process_ls_options(const char *opt, dirent_t *entry, stat_t *st)
{
	char c;
	int formatted_size = 0, inode_print = 0;
	int lng = 0;
	while(c = *opt)
	{
		switch(c)
		{
			case 'l':
			lng = 1;
			break;
			
			case 'h':
			formatted_size = 1;
			break;
			
			case 'i':
			inode_print = 1;
			break;
			
		}
		opt++;
	}
	print_file_attrs(entry, st, formatted_size, inode_print, lng);
}

void ls_no_options(const char *dir_name)
{
	DIR *dir = opendir(dir_name);

	if(dir == NULL){
		printf("Cannot open the directory\n");
		return;
	}
	dirent_t *entry;
	
	/*Display directory entries*/
	while(entry=(dirent_t *)readdir(dir))
	printf("%s\n", entry->d_name);

	closedir(dir);
}

void ls_with_options(const char *dname, const char *ops)
{
	/*
	- open
	- read the entry
	- resolve the full path for the entry
	- read the entry state
	- process the options
	- print
	*/
	
	DIR *dir = opendir(dname);
	if(dir == NULL){
		printf("Cannot open the directory\n");
		return;
	}
	
	dirent_t *entry;
	char path_to_entry[100];
	stat_t filestat; //store the file state attributes
	
	while(entry=(dirent_t *)readdir(dir)) /*For every entry in the directory*/
	{
		/*Resolve the full name*/
		strcpy(path_to_entry,dname);
		strcat(path_to_entry,"/");
		strcat(path_to_entry,entry->d_name);
		
		if(stat(path_to_entry, &filestat) == 0)
		{
			process_ls_options(ops+1, entry, &filestat);
		}
	}
	closedir(dir);
}

/*Head command*/
void head_print(const char *file, long int n)
{
	FILE *fp = fopen(file,"r"); /*open for read*/
	char c;
	long int line_count = 0;
	
	while((c = fgetc(fp)) != EOF)
	{
		printf("%c", c);
		if(c == '\n')
			line_count++;
		if(line_count == n)
			break;
	}
	fclose(fp);
}

void tail_print(const char *file, long int n)
{
	FILE *fp = fopen(file, "r");
	long line_cnt = line_count(fp);
	char c;
	long current_line = 0;
	
	
	if(line_cnt <= n)
	{
		rewind(fp); //go to the beginning
		while((c = fgetc(fp)) != EOF)
		{
			printf("%c", c);
		}
		
	}else
	{
		rewind(fp); //go to the beginning
		while((c = fgetc(fp)) != EOF)
		{
			if(c == '\n')
				current_line++;
			
			if(current_line == line_cnt - n)
				break;

		}
		
		while((c = fgetc(fp)) != EOF)
			printf("%c", c);
	}
	
	fclose(fp);
}

long str_to_int(const char *st)
{
	long res = 0;
	while(*st)
	{
		res = (res * 10) + (*st - '0');
		st++;
	}
	return res;
}

// wc command
void wc_count(FILE *fp, wc_data_t *data)
{
	unsigned long c_count=0, w_count=0, l_count=0;
	char c;
	
	while((c = fgetc(fp)) != EOF)
	{
		c_count++;
		
		if(c == '\n')
		{l_count++; w_count++;}
		else if(c == ' ')
			w_count++;
	}
	
	*data = (wc_data_t){c_count, w_count, l_count};
}

void wc_process(const char *f_name, const char *opts, wc_data_t *data)
{
	char c;
	
	FILE *fp = fopen(f_name, "r");
	
	if(fp == NULL){
		printf("Cannot open %s\n", f_name);
		return;
	}
	
	wc_count(fp, data);
	
	while(c = *opts++)
	{
		switch(c)
		{
			case 'c':
			printf("%lu  ", data->ch_count);
			break;
			
			case 'l':
			printf("%lu  ", data->line_count);
			break;
			
			case 'w':
			printf("%lu  ", data->word_count);
			break;
		}
	}
	printf("%s\n", f_name);
	
	fclose(fp);
}

void wc_noopts(const char *f_name, wc_data_t *data)
{	
	FILE *fp = fopen(f_name, "r");
	
	if(fp == NULL){
		printf("Cannot open %s\n", f_name);
		return;
	}
	
	wc_count(fp, data);
	
	printf("%lu  %lu  %lu  %s\n", data->ch_count, data->word_count, data->line_count, f_name);
	fclose(fp);
}


// grep command
int str_cmp(const char *s1, const char *s2)
{
	while(*s1 && *s2)
	{
		if(*s1 == *s2){
			s1++;
			s2++;
		}
		else
			break;
	}
	
	if(*s1 == '\0' && *s2 == '\0') // equal
		return 0;
	
	return (*(uint8_t *)s1 - *(uint8_t *)s2); // not equal
}

int str_cmp_icase(const char *s1, const char *s2)
{
	while(*s1 && *s2)
	{
		if((*s1 == *s2) || (*s1 == *s2 - 'a' + 'A') || (*s2 == *s1 - 'a' + 'A')){
			s1++;
			s2++;
		}
		else
			break;
	}
	
	if(*s1 == '\0' && *s2 == '\0') // equal
		return 0;
	
	return (*(uint8_t *)s1 - *(uint8_t *)s2); // not equal
}

int str_ncmp_icase(const char *s1, const char *s2, size_t len)
{
	int i;
	for(i = 0; i < len, *s1, *s2; i++, s1++, s2++)
	{
		if(*s1 == *s2 || *s2 == (*s1 - 'a' + 'A') || *s1 == (*s2 - 'a' + 'A'))
			continue;
		else
			return (*s1 - *s2);
	}
	if(i != len)
		return *s1 - *s2;
	
	return 0;
}

const char *strstr_icase(const char *s1, const char *s2) //find is (s2) is in (s1) ignoring case
{
	int len1 = strlen(s1);
	int len2 = strlen(s2);
	
	for(int i = 0; i <= len1 - len2; i++)
	{
		if(str_ncmp_icase(s1+i, s2, len2) == 0)
			return (s1+i);
	}
	return NULL;
}


void grep_main(const char *pattern, const char *f_name)
{
	FILE* fp = fopen(f_name, "r");
	
	if(fp == NULL){printf("can't open the file"); return;}
	
	char buffer[500];
	
	while(fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		if(strstr(buffer,pattern) != NULL)
			printf("%s",buffer);
	}
	
	fclose(fp);
}

void grep_icase(const char *pattern, const char *f_name)
{
	FILE* fp = fopen(f_name, "r");
	if(fp == NULL){printf("can't open the file"); return;}
	
	char buffer[500];
	
	while(fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		if(strstr_icase(buffer,pattern) != NULL)
			printf("%s",buffer);
	}
	
	fclose(fp);
}