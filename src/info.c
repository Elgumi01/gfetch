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

	double uptime_seconds;
	char uptime[UPTIME_BUFFER];

	char os[OS_BUFFER];
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
  
  	char memory_line[256] = {0};
  
	while (fgets(memory_line, sizeof(memory_line), f_memory))
	{
		if (strncmp(memory_line, "MemTotal:", 9) == 0) 
		{
			sscanf(memory_line, "MemTotal: %llu kB", &info.mem_total);
    		} 
		else if (strncmp(memory_line, "MemAvailable:", 9) == 0) 
		{
			sscanf(memory_line, "MemAvailable: %llu kB", &info.mem_available);
			break;
    		}
  	}

	
	double total_kb = info.mem_total;
	double used_kb  = info.mem_total - info.mem_available;

	double used;
	double total;
	char *memory_unit;
	
	if ((used_kb / 1024.0) < 1000)
	{
		used = used_kb / 1024.0;
		total = total_kb / 1024.0;
		memory_unit = "MiB";
	}
	else
	{
		used = used_kb / 1024.0 / 1024.0;
		total = total_kb / 1024.0 / 1024.0;
		memory_unit = "GiB";
	}


	snprintf(info.mem, sizeof(info.mem),
    	"Memory: %.2f %s / %.2f %s",
    	used, memory_unit, total, memory_unit);
	
 	/* UPTIME */

	FILE *f_uptime = fopen("/proc/uptime", "r");

  	if (f_uptime == NULL)
	{
    		fprintf(stderr, "Error opening /proc/uptime\n");
    		return info;
  	}

	if(fscanf(f_uptime, "%lf", &info.uptime_seconds) != 1)
	{
    		fprintf(stderr, "Error parsing /proc/uptime\n");
    		fclose(f_uptime);
    		return info;
  	}

	unsigned long long uptime_seconds = (unsigned long long)info.uptime_seconds;
	
	if (uptime_seconds >= 86400)
	{
		int days = uptime_seconds / 86400;
		int hours = (uptime_seconds % 86400) / 3600;
		snprintf(info.uptime, sizeof(info.uptime), 
		"Uptime: %dd %dh", 
		days, hours);
	}
	else if (uptime_seconds >= 3600)
	{
		int hours = uptime_seconds / 3600;
		int minutes = (uptime_seconds % 3600) / 60;
		snprintf(info.uptime, sizeof(info.uptime), 
		"Uptime: %dh %dm", 
		hours, minutes);

	}
	else if (uptime_seconds >= 60)
	{
		int minutes = uptime_seconds / 60;
		snprintf(info.uptime, sizeof(info.uptime), 
		"Uptime: %dm %llds", 
		minutes, uptime_seconds%60);
	}
	else
	{
		snprintf(info.uptime, sizeof(info.uptime), 
		"Uptime: %llds", 
		uptime_seconds);

	}
	
	/* OS  */

	FILE *f_os_release = fopen("/etc/os-release", "r");
	char os_release_line[256];

  	if (f_os_release == NULL)
	{
    		fprintf(stderr, "Error opening /proc/os-release\n");
    		return info;
  	}

	while(fgets(os_release_line, sizeof(os_release_line), f_os_release))
	{
		if (strncmp(os_release_line, "PRETTY_NAME=", 12) == 0)
		{	
			char *value = strchr(os_release_line, '=');
			if (value != NULL)
			{	
				value++;
				char cleaned[256] = {0};
				char *src = value;
				char *dst = cleaned;
				while (*src != '\0' && (dst - cleaned) < (long)sizeof(cleaned) - 1)
				{
					if (*src != '"' && *src != '\n')
					{
						*dst = *src;
						dst++;
					}

					src++;
				}
				*dst = '\0';
				snprintf(info.os, sizeof(info.os), "OS: %.251s", cleaned);
			}
			break;
		}
	}


	/* OTHERS  */
	
	fclose(f_memory);
	fclose(f_uptime);
	fclose(f_os_release);
	return info;
}
