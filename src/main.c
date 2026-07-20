#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "info.h"
#include "config.h"

#define VERSION "v0.1.0"

static void print_help(void)
{
    printf("Usage: gfetch [OPTION]...\n");
    printf("Minimalist and lightweight fetch\n\n");
    printf("--help                     Display a short overview of available options.\n");
    printf("--version                  Display gfetch version.\n");
    printf("--color <color>            Change de color of the logo and information. (colors: 1-7).\n");
    printf("--logo <path>              Displays the distro logo from a text file.\n");
    printf("--spacing <value>          Set the space between logo and information. (default: 10).\n");
    printf("--prompt                   Displays the user and hostname like a prompt.\n");
    printf("--mem                      Displays the used memory and total memory.\n");
    printf("--kernel                   Displays the kernel in use.\n");
    printf("--uptime                   Displays the uptime.\n");
    printf("--all                      Display all available information.\n");
    printf("--os                       Displays the os-release pretty name.\n");
    printf("Examples:\n");
    printf("  gfetch --logo ~/mylogo.txt --spacing 30  --all\n");
}

int main(int argc, char **argv)
{	
	char *logo_path = NULL;
	int spacing = 10;
	
	int has_color  = 0;
	int int_color   = 0;

	int show_prompt = 0;
	int show_mem    = 0;
	int show_kernel = 0;
	int show_uptime = 0;
	int show_os     = 0;

	if (argc <= 1)
	{
		printf("gfetch: no options provided. Try 'gfetch --help'.\n");
		return 1;
	}

	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "--help") == 0)
		{
			print_help();
			return 0;
		} 
		else if (strcmp(argv[i], "--version") == 0)
		{
			printf("gfetch %s\n", VERSION);
			return 0;
		}

	}
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "--logo") == 0)
		{
			if (i + 1 >= argc)
			{
				printf("gfetch: --logo requires an argument.\n");
				return 1;
			}
			logo_path = argv[i + 1];
			i++;
		}
		else if (strcmp(argv[i], "--color") == 0)
		{
			if (i + 1 >= argc)
			{
				printf("gfetch: --color requires an argument.\n");
				return 1;
			}
			has_color = 1;
			char *argv_end = NULL;
			int_color = strtol(argv[i + 1], &argv_end, 10);
			if (int_color == 0)
			{
				printf("gfetch: --color invalid argument.\n");
				return 1;
			}
			if (int_color < 0)
			{
				printf("gfetch: --color must be a non-negative number.\n");
				return 1;
			}
			if (*argv_end != '\0')
			{
				printf("gfetch: --color only accept numbers.\n");
				return 1;
			}
			if (int_color)
			i++;
		}

		else if (strcmp(argv[i], "--prompt") == 0)
		{
			show_prompt = 1;
		}

		else if (strcmp(argv[i], "--mem") == 0)
		{
			show_mem = 1;
		}
		else if (strcmp(argv[i], "--kernel") == 0)
		{
			show_kernel = 1;
		}
		else if (strcmp(argv[i], "--uptime") == 0)
		{
			show_uptime = 1;
		}
		else if (strcmp(argv[i], "--os") == 0)
		{
			show_os = 1;
		}

		else if (strcmp(argv[i], "--all") == 0)
		{
			show_kernel = 1;
			show_mem    = 1;
			show_prompt = 1;
			show_uptime = 1;
			show_os     = 1;
		}
		else if (strcmp(argv[i], "--spacing") == 0)
		{
			if (i + 1 >= argc)
			{
				printf("gfetch: --spacing requires an argument.\n");
				return 1;
			}
			spacing = atoi(argv[i + 1]);
			if (spacing < 0)
			{
				printf("gfetch: --spacing must be a non-negative number.\n");
				return 1;
			}
			i++;
		}


		else
		{
			printf("gfetch: invalid flag '%s'.\n", argv[i]);
			printf("Try: 'gfetch --help' for help.\n");
			return 1;
		}
	}
	
	/* Today we have 8 lines for info, probably should increase in future, same with the 32 lines for logos  */

	char info_lines[8][PROMPT_BUFFER] = {0};
	int info_count = 0;
	
	char logo_lines[LOGO_SIZE][128] = {0}; 
    	int logo_count = 0;
	
	/* Amazing colors */

	char *primary_esc = {0};
	
	/* Writing the info and checking colors*/

	SystemInfo info = get_info();
	
	if (has_color)
	{
		switch (int_color)
		{
			case 1: // BLACK
				primary_esc = "\033[30m";
				break;
			case 2: // RED
				primary_esc = "\033[31m";
				break;
			case 3: // GREEN
				primary_esc = "\033[32m";
				break;
			case 4: // YELLOW
				primary_esc = "\033[33m";
				break;
			case 5: // BLUE
				primary_esc = "\033[34m";
				break;
			case 6: // MAGENTA
				primary_esc = "\033[35m";
				break;
			case 7: // CYAN
				primary_esc = "\033[36m";
				break;
			case 8: // WHITE
				primary_esc = "\033[37m";
				break;
			default:
				printf("gfetch: --color argument must be in range.\n");
				return 1;
		}	
	}
	
	char *color = !primary_esc ? "" : primary_esc;
	char *reset = !primary_esc ? "" : "\033[0m";

	if (show_prompt) 
	{
		snprintf(info_lines[info_count++],     
		         PROMPT_BUFFER,
			 "%s%.110s%s",
			 color,
			 info.prompt,
			 reset);
		snprintf(info_lines[info_count++],
			 PROMPT_BUFFER,
			 "%s%s%s",
			 color,
			 "───────────────────────",
			 reset);
	}
	if (show_mem) snprintf(info_lines[info_count++],     
		         MEMORY_BUFFER,
			 "%smemory : %s%.235s",
			 color,
			 reset,
			 info.mem);
	if (show_kernel) snprintf(info_lines[info_count++],     
		         KERNEL_BUFFER,
			 "%skernel : %s%.490s",
			 color,
			 reset,
			 info.kernel);
	if (show_uptime) snprintf(info_lines[info_count++],     
		         UPTIME_BUFFER,
			 "%suptime : %s%.100s",
			 color,
			 reset,
			 info.uptime);
	if (show_os) snprintf(info_lines[info_count++],     
		         OS_BUFFER,
			 "%sos     : %s%.490s",
			 color,
			 reset,
			 info.os);

	if (logo_path != NULL)
	{
    		FILE *fp = fopen(logo_path, "r");
    		if (fp == NULL)
		{
        		fprintf(stderr, "Could not open %s\n", logo_path);
			return 1;
		}

    		fseek(fp, 0, SEEK_END);
    		long size = ftell(fp);
		if (size < 0)
		{	
			fprintf(stderr, "ftell failed");
			fclose(fp);
			return 1;
		}
    		rewind(fp);

    		char *buf = malloc(size + 1);
    		if (buf == NULL)
    		{
        		fclose(fp);
        		return 1;
    		}

    		fread(buf, 1, size, fp);
    		buf[size] = '\0';

    		fclose(fp);	
		
		char *line = strtok(buf, "\n");
		while (line != NULL && logo_count < LOGO_SIZE)
		{
			strncpy(logo_lines[logo_count++], line, sizeof(logo_lines[0]) - 1);
			line = strtok(NULL, "\n");
		}
		free(buf);
	} 


	int total_rows = (logo_count > info_count) ? logo_count : info_count;
	for (int row = 0; row < total_rows; row++)
	{
		if (row < logo_count)
		{
			printf("%s%-*s%s", color, spacing, logo_lines[row], reset);
		}
		else
		{
			printf("%-*s", spacing, "");
		}
		if (row < info_count)
		{
			printf(" %s", info_lines[row]);
		}

		printf("\n");
	}



	return 0;
	
}
