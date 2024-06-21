#include "../libft/libft.h"
#include <stdio.h>

int main(int argc, char *argv[], char *envp[])
{
    (void) argc;
    (void) envp;
    for (int i = 0; argv[i] != 0; i++)
		printf("%s\n", argv[i]);
    return (0);
}