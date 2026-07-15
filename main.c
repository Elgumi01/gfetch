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

static Logo *find_logo(char *distro)
{
	for (int i = 0; i < LOGO_COUNT; i++)
	{
		if (strcmp(distro, logos[i].name) == 0)
		{
			return &logos[i];
		}
	}
	return NULL;
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
	
	char info_lines[8][128] = {0};
	int info_count = 0;
	
	char logo_lines[16][64];
    	int logo_count = 0;


	if (show_kernel) snprintf(info_lines[info_count++], 128 ,"Kernel");
	if (show_mem) snprintf(info_lines[info_count++], 128 ,"Mem");

	if (distro != NULL)
	{	
		const Logo *logo = find_logo(distro);
		
		if (logo == NULL)
		{
			printf("gfetch: unknown distro '%s'.\n", distro);
			return 1;
		}

		char buf[512];
		strncpy(buf, logo->ascii, sizeof(buf) - 1);
		buf[sizeof(buf) - 1] = '\0';
		
		char *line = strtok(buf, "\n");
		while (line != NULL && logo_count < 16)
		{
			strncpy(logo_lines[logo_count++], line, 63);
			line = strtok(NULL, "\n");
		}
	} 

	int total_rows = (logo_count > info_count) ? logo_count : info_count;
	for (int row = 0; row < total_rows; row++)
	{
		if (row < logo_count)
		{
			printf("%-*s", 10, logo_lines[row]);
		}
		else
		{
			printf("%-*s", 10, "");
		}
		if (row < info_count)
		{
			printf(" %s", info_lines[row]);
		}

		printf("\n");
	}



	return 0;
	
}
