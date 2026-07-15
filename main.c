#include <stdio.h>
#include <string.h>

#include "logos.h"

static void print_help(void)
{
    printf("USE: gfetch [OPTION]...\n");
    printf("Minimalist and lightweight fetch\n\n");
    printf("--help          Display a short resume of available options.\n");
    printf("--logo <name>   Displays the distro logo.\n");
}

static int display_logo(char *distro)
{
	for (int i = 0; i < LOGO_COUNT; i++)
	{
		if (strcmp(distro, logos[i].name) == 0)
		{
			printf("%s", logos[i].ascii);
			return 0;
		}
	}


	printf("gfetch: unknown distro '%s'.\n", distro);
	return 1;
}

int main(int argc, char **argv)
{	
	char *distro = {0};

	int show_mem = 0;
	int show_kernel = 0;

	if (argc <= 1)
	{
		printf("gfetch: gfetch requires an argument\n");
		return 1;
	}

	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "--help") == 0)
		{
			print_help();
			return 0;
		}
	}
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "--logo") == 0)
		{
			if (i + 1 >= argc)
			{
				printf("gfetch: --logo requires an argument\n");
				return 1;
			}
			distro = argv[i + 1];
			i++;
		}
		else if (strcmp(argv[i], "--mem") == 0)
		{
			show_mem = 1;
		}
		else if (strcmp(argv[i], "--kernel") == 0)
		{
			show_kernel = 1;
		}
		else
		{
			printf("gfetch: invalid flag '%s'.\n", argv[1]);
			printf("Try: 'gfetch --help' for help.\n");
			return 1;
		}
	}

	if (distro != NULL)
	{
		display_logo(distro);
	}
	if (show_kernel)
	{
		printf("kernel\n");
	}
	if (show_mem)
	{
		printf("mem\n");
	}

	return 0;
	
}
