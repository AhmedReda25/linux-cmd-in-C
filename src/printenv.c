#include "help.h"


int main(int argc, char **argv, char **envp)
{
    
    if(argc == 1)
    {
        int i = 0;
        for(; envp[i]; i++) printf("%s\n", envp[i]);
        return 0;
    }

    for(int i = 1; i < argc; ++i)
    {
        char env[200];
        strcpy(env, getenv(argv[i]));
        if(env != NULL) printf("%s\n", env);
    }
    return 0;
}