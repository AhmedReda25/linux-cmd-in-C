#include "help.h"


int main(int argc, char **argv)
{
    char env[100];

    for(int i = 1; i < argc; ++i)
    {
        strcpy(env, getenv(argv[i]));
        if(env != NULL) printf("%s\n", env);
    }
}