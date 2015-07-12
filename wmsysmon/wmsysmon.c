/*

 wmsysmon.c -- system monitor thing.

 by Dave Clark (clarkd@skynet.ca) (http://www.neotokyo.org/illusion)

 This software is licensed through the GNU General Public License.

*/


#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#include <X11/Xlib.h>
#include <X11/xpm.h>
#include <X11/extensions/shape.h>

#include "../wmgeneral/wmgeneral.h"
#include "../wmgeneral/misc.h"

#include "wmsysmon-master.xpm"
char wmsysmon_mask_bits[64*64];
int  wmsysmon_mask_width = 64;
int  wmsysmon_mask_height = 64;

long start_time=0;
long start_uptime=0;

long io_max;
long io_max_diff;

#define WMSYSMON_VERSION "0.2"

#define CHAR_WIDTH 5
#define CHAR_HEIGHT 7

#define BCHAR_WIDTH 6
#define BCHAR_HEIGHT 9

#define BOFFX   (93)
#define BOFFY   (66)
#define BREDX   (81)
#define BREDY   (66)
#define BGREENX (87)
#define BGREENY (66)

#define LITEW   (4)
#define LITEH   (4)

#define B_OFF   (0)
#define B_RED   (1)
#define B_GREEN (2)


long last_ints[16];
long ints[16];

long last_pageins=0;
long last_pageouts=0;

long last_swapins=0;
long last_swapouts=0;

int update_rate=200000;

extern	char **environ;

char	*ProgName;

char uconfig_file[256];

time_t		curtime;
time_t		prevtime;

void usage(void);
void printversion(void);
void BlitString(char *name, int x, int y);
void BlitNum(int num, int x, int y);
void wmsysmon_routine(int, char **);
int ReadConfigInt(FILE *fp, char *setting, int *value);
int ReadConfigString(FILE *fp, char *setting, char *value);
int Read_Config_File( char *filename );
void DrawBar(int sx, int sy, int w, int h, float percent, int dx, int dy);
void DrawLite(int state, int dx, int dy);
void DrawUptime(void);
void DrawStuff( void );
void DrawMem( void );

int main(int argc, char *argv[]) {

	int		i;

    uconfig_file[0] = 0;
    
	/* Parse Command Line */

	ProgName = argv[0];
	if (strlen(ProgName) >= 5)
		ProgName += (strlen(ProgName) - 5);
	
	for (i=1; i<argc; i++) {
		char *arg = argv[i];

		if (*arg=='-') {
			switch (arg[1]) {
			case 'd' :
				if (strcmp(arg+1, "display")) {
					usage();
					exit(1);
				}
				break;
			case 'g' :
				if (strcmp(arg+1, "geometry")) {
					usage();
					exit(1);
				}
				break;
			case 'v' :
				printversion();
				exit(0);
				break;
            case 'r':
                if (argc > (i+1))
                {
                    update_rate = (atoi(argv[i+1]) * 1000);
                    i++;
                }
                break;
            case 'c' :
                if (argc > (i+1))
                {
                    strcpy(uconfig_file, argv[i+1]);
                    i++;
                }
                break;
            default:
				usage();
				exit(0);
				break;
			}
		}
	}

	wmsysmon_routine(argc, argv);

	return 0;
}

/*******************************************************************************\
 |* wmsysmon_routine														   *|
\*******************************************************************************/

void wmsysmon_routine(int argc, char **argv)
{
    int			i;
	XEvent		Event;
	int			but_stat = -1;
    FILE *fp;
    struct timeval tv={0,0};
    struct timeval last={0,0};
    

//    char config_file[512];

    createXBMfromXPM(wmsysmon_mask_bits, wmsysmon_master_xpm, wmsysmon_mask_width, wmsysmon_mask_height);
    
	openXwindow(argc, argv, wmsysmon_master_xpm, wmsysmon_mask_bits, wmsysmon_mask_width, wmsysmon_mask_height);

//	AddMouseRegion(0, 5, 6, 58, 16);

    // We don't need a config file (yet)...
#if 0
    // Read config file

    if (uconfig_file[0] != 0)
    {
        // user-specified config file
        fprintf(stderr, "Using user-specified config file '%s'.\n", uconfig_file);
        Read_Config_File(uconfig_file);
     }
    else
    {
        sprintf(config_file, "%s/.wmsysmonrc", getenv("HOME"));

        if (!Read_Config_File(config_file))
        {
            // Fall back to /etc/wminetrc
            sprintf(config_file, "/etc/wmsysmonrc");
        
            Read_Config_File(config_file);
        }
    }
#endif

    // init ints
    bzero(&last_ints, sizeof(last_ints));
    bzero(&ints, sizeof(ints));
    

    
    // init uptime
    fp = fopen("/proc/uptime", "r");
    if (fp)
    {
        fscanf(fp, "%ld", &start_time);
        fclose(fp);
        start_uptime = time(0);
    }

    RedrawWindow();


    while (1)
    {
        
        curtime = time(0);

        if (1)
        {
            memcpy(&last, &tv, sizeof(tv));
            
            // Update display

            DrawUptime();

            DrawStuff();

            DrawMem();

            RedrawWindow();
        }
        
        // X Events
        while (XPending(display))
        {
			XNextEvent(display, &Event);
            switch (Event.type)
            {
			case Expose:
				RedrawWindow();
				break;
			case DestroyNotify:
				XCloseDisplay(display);
				exit(0);
                break;
			case ButtonPress:
				i = CheckMouseRegion(Event.xbutton.x, Event.xbutton.y);

				but_stat = i;
				break;
			case ButtonRelease:
				i = CheckMouseRegion(Event.xbutton.x, Event.xbutton.y);

                if (but_stat == i && but_stat >= 0)
                {
                    switch (but_stat)
                    {
                    case 0 :
                        break;
                    case 1 :
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                    case 4:
                        break;

					}
				}
				but_stat = -1;
//				RedrawWindow();
				break;
			}
		}

		usleep(update_rate);
	}
}

void DrawBar(int sx, int sy, int w, int h, float percent, int dx, int dy)
{
    int tx;

    tx = (float)((float)w * ((float)percent / (float)100.0));

    
    copyXPMArea(sx, sy, tx, h, dx, dy);

    copyXPMArea(sx, sy+h+1, w-tx, h, dx+tx, dy);
        

}

void DrawLite(int state, int dx, int dy)
{
    switch(state)
    {
    case B_RED:
        copyXPMArea(BREDX, BREDY, LITEW, LITEH, dx, dy);
        break;
    case B_GREEN:
        copyXPMArea(BGREENX, BGREENY, LITEW, LITEH, dx, dy);
        break;
    default:
    case B_OFF:
        copyXPMArea(BOFFX, BOFFY, LITEW, LITEH, dx, dy);
        break;
    }
}


void DrawUptime(void)
{
    long uptime;
    char buf[32];
    int i;
    
    uptime = curtime - start_uptime + start_time;

    uptime /=60;
    // blit minutes
    i = uptime % 60;
    uptime /=60;
    sprintf(buf, "%02i", i);
    BlitString(buf, 45, 37);
    
    // blit hours
    i = uptime % 24;
    uptime /=24;
    sprintf(buf, "%02i", i);
    BlitString(buf, 29, 37);
    
    // blit days
    i = uptime;
    sprintf(buf, "%03i", i);
    BlitString(buf, 6, 37);
}


void DrawStuff( void )
{
    FILE *fp;
    char buf[1024];
    char *tok;
    char seps[]={" "};
    int i;

    long io=0;
    long iodiff=0;
    int iopercent=0;

    long pageins=0;
    long pageouts=0;
    long swapins=0;
    long swapouts=0;

    fp = fopen("/proc/stat", "r");

    if (!fp)
        return;

    //gather data...
    while( fgets(buf, 127, fp) )
    {
        if (strstr(buf, "disk_rio"))
        {
            tok = strtok(buf, seps);
            for(i=0; i<4; i++)
            {
                io += atoi(tok);
                tok = strtok(NULL, seps);
            }
        }

        if (strstr(buf, "disk_wio"))
        {
            tok = strtok(buf, seps);
            for(i=0; i<4; i++)
            {
                io += atoi(tok);
                tok = strtok(NULL, seps);
            }
        }

        if (strstr(buf, "intr"))
        {
            sscanf(buf, "intr %*d %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld",
                   &ints[0], &ints[1], &ints[2], &ints[3], &ints[4], &ints[5], &ints[6], &ints[7],
                   &ints[8], &ints[9], &ints[10], &ints[11], &ints[12], &ints[13], &ints[14], &ints[15]);
        }

        if (strstr(buf, "page"))
        {
            sscanf(buf, "page %ld %ld", &pageins, &pageouts);
        }

        if (strstr(buf, "swap"))
        {
            sscanf(buf, "swap %ld %ld", &swapins, &swapouts);
        }
    }


    // ------------------ IO bar ------------------
    if (io_max == 0)
        io_max = io;
    
    if (io > io_max)
        iodiff = abs(io_max - io);
    else
        iodiff=0;

    io_max = io;

    
    if (io_max_diff !=0)
        iopercent = ((float) iodiff / (float) io_max_diff) * 100.0;
    else
        iopercent=0;

    if (iodiff > io_max_diff)
        io_max_diff=iodiff;

    if (iopercent >100)
        iopercent =100;
    
//    printf("io: %i\n", io);
//    printf("io_max: %i\n", io_max);
//    printf("iodiff: %i\n", iodiff);
//    printf("iopercent: %i\n", iopercent);

    DrawBar(67, 36, 58, 6, iopercent, 3, 26);

    // -------------- ints --------------------
    // top 8
    for (i=0; i<8; i++)
    {
        if ( ints[i] > last_ints[i] )
        {
            DrawLite(B_GREEN, 4 + (i*LITEW), 51);
        }
        else
        {
            DrawLite(B_OFF, 4 + (i*LITEW), 51);
        }
    }

    // bottom 8
    for (i=8; i<16; i++)
    {
        if ( ints[i] > last_ints[i] )
        {
            DrawLite(B_GREEN, 4 + ((i-8)*LITEW), 56);
        }
        else
        {
            DrawLite(B_OFF, 4 + ((i-8)*LITEW), 56);
        }
    }

    memcpy(&last_ints, &ints, sizeof(ints));

    // ----- page in / out ---------------

    if (pageins > last_pageins)
    {
        DrawLite(B_RED, 51, 51);
    }
    else
    {
        DrawLite(B_OFF, 51, 51);
    }

    if (pageouts > last_pageouts)
    {
        DrawLite(B_RED, 56, 51);
    }
    else
    {
        DrawLite(B_OFF, 56, 51);
    }

    last_pageins = pageins;
    last_pageouts = pageouts;

    // --------- swap in/out ------------------

    if (swapins > last_swapins)
    {
        DrawLite(B_RED, 51, 56);
    }
    else
    {
        DrawLite(B_OFF, 51, 56);
    }

    if (swapouts > last_swapouts)
    {
        DrawLite(B_RED, 56, 56);
    }
    else
    {
        DrawLite(B_OFF, 56, 56);
    }

    last_swapins = swapins;
    last_swapouts = swapouts;

    
    fclose(fp);
}


void DrawMem( void )
{
    FILE *fp;
    char buf[128];
    long mem_total=0;
    long mem_used=0;
    long mem_free=0;
    long mem_shared=0;
    long mem_buffers=0;
    long mem_cache=0;
    long swap_total=0;
    long swap_used=0;
    long swap_free=0;

    int mempercent=0;
    int swappercent=0;

    fp = fopen("/proc/meminfo", "r");

    if (!fp)
        return;

    while (fgets(buf, 127, fp))
    {
        if (strstr(buf, "Mem:"))
        {
            sscanf(buf, "Mem: %ld %ld %ld %ld %ld %ld", &mem_total, &mem_used, &mem_free, &mem_shared, &mem_buffers, &mem_cache);
        }

        if (strstr(buf, "Swap:"))
        {
            sscanf(buf, "Swap: %ld %ld %ld", &swap_total, &swap_used, &swap_free);
        }
    }
            
    mempercent = ((float) (mem_used - mem_buffers - mem_cache) / (float) mem_total) * 100;

    swappercent = ((float) (swap_used) / (float) swap_total) * 100;

//    printf("mempercent: %i\n", mempercent);
//    printf("swappercent: %i\n", swappercent);
    
    DrawBar(67, 36, 58, 6, mempercent, 3, 4);
    DrawBar(67, 36, 58, 6, swappercent, 3, 15);

    fclose(fp);
}




// Blits a string at given co-ordinates
void BlitString(char *name, int x, int y)
{
    int		i;
	int		c;
    int		k;

	k = x;
    for (i=0; name[i]; i++)
    {

        c = toupper(name[i]); 
        if (c >= 'A' && c <= 'Z')
        {   // its a letter
			c -= 'A';
			copyXPMArea(c * 6, 74, 6, 8, k, y);
			k += 6;
        }
        else
        {   // its a number or symbol
			c -= '0';
			copyXPMArea(c * 6, 64, 6, 8, k, y);
			k += 6;
		}
	}

}

void BlitNum(int num, int x, int y)
{
    char buf[1024];
    int newx=x;

    sprintf(buf, "%03i", num);

    BlitString(buf, newx, y);
}
    

// ReadConfigSetting
int ReadConfigString(FILE *fp, char *setting, char *value)
{
    char str[1024];
    char buf[1024];
    int i;
    int len;
    int slen;
    char *p=NULL;


    if (!fp)
    {
        return 0;
    }

    sprintf(str, "%s=", setting);
    slen = strlen(str);
    
    fseek(fp, 0, SEEK_SET);

    while ( !feof(fp) )
    {
        
        if (!fgets(buf, 512, fp))
            break;
        
        len = strlen(buf);

        // strip linefeed
        for (i=0; i!=len; i++)
        {
            if (buf[i] == '\n')
            {
                buf[i] = 0;
            }
        }

        //printf("Scanning '%s'...\n", buf);
        if ( strncmp(buf, str, strlen(str)) == 0)
        {
            // found our setting
            
            for(i=0; i!=slen; i++)
            {
                if ( buf[i] == '=' )
                {
                    p=buf+i+1;
                    strcpy(value, p);
                    return 1;
                }
            }
    
        }
    }
    
        return 0;
}

int ReadConfigInt(FILE *fp, char *setting, int *value)
{
    char buf[1024];

    if (ReadConfigString(fp, setting, (char *) &buf))
    {
        *value = atoi(buf);
        return 1;
    }

    return 0;
}

int Read_Config_File( char *filename )
{
    FILE *fp;

    fp = fopen(filename, "r");
    if (fp)
    {
        
        fclose(fp);
        return 1;
    }
    else
    {
        perror("Read_Config_File");
        fprintf(stderr, "Unable to open %s, no settings read.\n", filename);
        return 0;
    }

}

    
            
            



/*******************************************************************************\
|* usage																	   *|
\*******************************************************************************/

void usage(void)
{
    fprintf(stderr, "\nWMsysmon - illusion <clarkd@skynet.ca>  http://www.neotokyo.org/illusion\n\n");
	fprintf(stderr, "usage:\n");
	fprintf(stderr, "    -display <display name>\n");
	fprintf(stderr, "    -geometry +XPOS+YPOS      initial window position\n");
    fprintf(stderr, "    -r                        update rate in milliseconds (default:200)\n");

//    fprintf(stderr, "    -c <filename>             use specified config file\n");
    fprintf(stderr, "    -h                        this help screen\n");
	fprintf(stderr, "    -v                        print the version number\n");
    fprintf(stderr, "\n");
}

/*******************************************************************************\
|* printversion																   *|
\*******************************************************************************/

void printversion(void)
{
	fprintf(stderr, "wmsysmon v%s\n", WMSYSMON_VERSION);
}
