#ifndef HELP_H
#define HELP_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>

typedef struct dirent dirent_t;
typedef struct stat   stat_t;

typedef struct
{
	unsigned long ch_count;
	unsigned long word_count;
	unsigned long line_count;
}wc_data_t;

void wc_count(FILE *fp, wc_data_t *data);
void wc_process(const char *f_name, const char *opts, wc_data_t *data);
void wc_noopts(const char *f_name, wc_data_t *data);

long int line_count(FILE *fp);

void ls_no_options(const char *dir_name);
void ls_with_options(const char *dname, const char *ops);

//Head and tail commands
void head_print(const char *file, long int n);
long str_to_int(const char *st);
void tail_print(const char *file, long int n);

// grep command
int str_cmp(const char *s1, const char *s2);
int str_cmp_icase(const char *s1, const char *s2);
const char *strstr_icase(const char *s1, const char *s2);
int str_ncmp_icase(const char *s1, const char *s2, size_t len);

void grep_main(const char *pattern, const char *f_name);
void grep_icase(const char *pattern, const char *f_name);
#endif