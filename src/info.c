#include <stdio.h>
#include <sys/utsname.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>

#include "config.h"

typedef struct {
	char prompt[PROMPT_BUFFER];

	unsigned long long mem_total;
	unsigned long long mem_available;
	char mem[MEMORY_BUFFER];

	char kernel[KERNEL_BUFFER];
} SystemInfo;

SystemInfo get_info()
{
	SystemInfo info = {0};
	
	/* PROMPT */
	
	char username[PROMPT_BUFFER] = {0};
	char hostname[PROMPT_BUFFER] = {0};
	
  	struct passwd *pw = getpwuid(getuid());
	
	if (pw == NULL)
	{
		fprintf(stderr, "Error getting passwd struct via getpwuid\n");
		return info;
	}

  	snprintf(username, sizeof(username), "%s", pw->pw_name);

  	if (gethostname(hostname, sizeof(hostname)) == -1)
	{
		fprintf(stderr, "Error getting hostname via gethostname()");
		return info;
	}

	snprintf(info.prompt, sizeof(info.prompt), "%s@%s", username, hostname);

	/* KERNEL */

	struct utsname uts;

	if (uname(&uts) == -1) {
		fprintf(stderr, "Error getting kernel info via uname\n");
		return info;
	}

	snprintf(info.kernel, sizeof(info.kernel), "Kernel: %s", uts.release);
	
	/* MEMORY */

 	FILE *f_memory = fopen("/proc/meminfo", "r");

  	if (f_memory == NULL) {
    		fprintf(stderr, "Error opening /proc/meminfo\n");
    		return info;
  	}
  
  	char line[256] = {0};
  
	while (fgets(line, sizeof(line), f_memory))
	{
		if (strncmp(line, "MemTotal:", 9) == 0) 
		{
			sscanf(line, "MemTotal: %llu kB", &info.mem_total);
    		} 
		else if (strncmp(line, "MemAvailable:", 9) == 0) 
		{
			sscanf(line, "MemAvailable: %llu kB", &info.mem_available);
			break;
    		}
  	}

  	fclose(f_memory);
	
	double total_kb = info.mem_total;
	double used_kb  = info.mem_total - info.mem_available;

	double used;
	double total;
	char *unit;
	
	if ((used_kb / 1024.0) < 1000)
	{
		used = used_kb / 1024.0;
		total = total_kb / 1024.0;
		unit = "MiB";
	}
	else
	{
		used = used_kb / 1024.0 / 1024.0;
		total = total_kb / 1024.0 / 1024.0;
		unit = "GiB";
	}


	snprintf(info.mem, sizeof(info.mem),
    	"Memory: %.2f %s / %.2f %s",
    	used, unit, total, unit);	

	return info;
}
