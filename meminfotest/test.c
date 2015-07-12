#include	<stdio.h>
#include	<dirent.h>


main(void)
{
	char		buf[128];
	FILE		*fp;
	int		i = 0;

	fp = fopen("/proc/meminfo", "r");

	while(i < 1000) {
		freopen("/proc/meminfo", "r", fp);
		while(fgets(buf, 128, fp));
		i++;
	}
}
