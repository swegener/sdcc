#include <tinibios.h>
#include <ds400rom.h>

#include <stdio.h>
#include <string.h>

#define BUF_LEN 80

void usage(void)
{
    puts("Available commands:\n");
    puts("ledon: turns LED on.");
    puts("ledoff: turns LED off.");
    puts("startclock: starts millisecond timer.");
    puts("clock: reports millisecond timer.");
    puts("sleep: sleeps for 10 seconds (or forever if you didn't startclock first).");
}

void main(void)
{
    char buffer[80];
    
    // At this stage, the rom isn't initalized. We do have polled serial I/O, though.
    printf("TINIm400 monitor rev 0.0\n");

    // Intialize the ROM.
    if (romInit(1))
    {
	// We're hosed. romInit will have printed an error, nothing more to do.
	return;
    }
    
    P5 &= ~4; // LED on.

    // Switch to interrupt driven serial I/O now that the rom is initialized.
    Serial0SwitchToBuffered();

    while (1)
    {
	// monitor prompt.
        printf("-> ");
	
	gets(buffer); // unsafe, of course, should use some equivalent of fgets.
	
	if (!strcmp(buffer, "ledon"))
	{
	    P5 &= ~4; // LED on.
	    printf("LED on.\n");
	}
	else if (!strcmp(buffer, "ledoff"))
	{
	    P5 |= 4;
	    printf("LED off.\n");
	}
	else if (!strcmp(buffer, "startclock"))
	{
	    printf("Starting clock...\n");
	    ClockInit();
	}
	else if (!strcmp(buffer, "clock"))
	{
	    printf("Clock: %ld\n", ClockTicks());
	}
	else if (!strcmp(buffer, "sleep"))
	{
	    printf("Sleeping for 10 seconds...\n");
	    
	    ClockMilliSecondsDelay(10 * 1000);
	    
	    printf("Back.\n");
	}
	
	else if (buffer[0])
	{
	    printf("Unknown command \"%s\".\n", buffer);
	    usage();
	}
    }
}
