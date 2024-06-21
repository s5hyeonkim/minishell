#include "../libft/libft.h"
#include <stdio.h>

// ㅇㅣ도ㅇ 상대경로도 가능해야함.
// pipe 안됨

int main(int argc, char *argv[], char *envp[])
{
	(void) argc;
	(void) envp;
	
	if (ft_strncmp(argv[0], "cd", 3))
	{
		printf("cd : %s\n", "different cases");
		return (0);
	}
	if (argc == 1)

	return (0);
}