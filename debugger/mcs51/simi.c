/*-------------------------------------------------------------------------
  simi.c - source file for simulator interaction

	      Written By -  Sandeep Dutta . sandeep.dutta@usa.net (1999)

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
   
   In other words, you are welcome to use, share and improve this program.
   You are forbidden to forbid anyone else to use, share and improve
   what you give them.   Help stamp out software-hoarding!  
-------------------------------------------------------------------------*/
#include "sdcdb.h"
#include "simi.h"

#ifdef HAVE_SYS_SOCKET_H
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#else
#error "Cannot build debugger without socket support"
#endif
FILE *simin ; /* stream for simulator input */
FILE *simout; /* stream for simulator output */

int sock = -1; /* socket descriptor to comm with simulator */
pid_t simPid;
static char simibuff[MAX_SIM_BUFF];    /* sim buffer       */
static char regBuff[MAX_SIM_BUFF];
static char *sbp = simibuff;           /* simulator buffer pointer */
extern char **environ;
char simactive = 0;

static memcache_t memCache[NMEM_CACHE];

/*-----------------------------------------------------------------*/
/* get data from  memory cache/ load cache from simulator          */
/*-----------------------------------------------------------------*/
static char *getMemCache(unsigned int addr,int cachenum, int size)
{
    char *resp, *buf;
    unsigned int laddr;
    memcache_t *cache = &memCache[cachenum];

    if ( cache->size <=   0 ||
         cache->addr > addr ||
         cache->addr + cache->size < addr + size )
    {
        if ( cachenum == IMEM_CACHE )
        {
            sendSim("di 0x0 0xff\n");
        }
        else
        {
            laddr = addr & 0xffffffc0;
            sprintf(cache->buffer,"dx 0x%x 0x%x\n",laddr,laddr+0xff );
            sendSim(cache->buffer);
        }
        waitForSim(100,NULL);
        resp = simResponse();
        cache->addr = strtol(resp,0,0);
        buf = cache->buffer;
        cache->size = 0;
        while ( *resp && *(resp+1) && *(resp+2))
        {
            /* cache is a stringbuffer with ascii data like
               " 00 00 00 00 00 00 00 00"
            */
            resp += 2;
            laddr = 0;
            /* skip thru the address part */
            while (isxdigit(*resp)) resp++;
            while ( *resp && *resp != '\n')
            {
                if ( laddr < 24 )
                {
                    laddr++ ;
                    *buf++ = *resp ;
                }
                resp++;
            }
            resp++ ;
            cache->size += 8;
        }
        *buf = '\0';
        if ( cache->addr > addr ||
             cache->addr + cache->size < addr + size )
            return NULL;
    }
    return cache->buffer + (addr - cache->addr)*3;
}

/*-----------------------------------------------------------------*/
/* invalidate memory cache                                         */
/*-----------------------------------------------------------------*/
static void invalidateCache( int cachenum )
{
    memCache[cachenum].size = 0;  
}

/*-----------------------------------------------------------------*/
/* waitForSim - wait till simulator is done its job                */
/*-----------------------------------------------------------------*/
void waitForSim(int timeout_ms, char *expect)
{
  int i=0;
  int ch;

Dprintf(D_simi, ("simi: waitForSim start(%d)\n", timeout_ms));
    sbp = simibuff;

    while ((ch = fgetc(simin)) > 0 ) {
      *sbp++ = ch;
    }
    *sbp = 0;
    Dprintf(D_simi, ("waitForSim(%d) got[%s]\n", timeout_ms, simibuff));

}

/*-----------------------------------------------------------------*/
/* openSimulator - create a pipe to talk to simulator              */
/*-----------------------------------------------------------------*/
void openSimulator (char **args, int nargs)
{
    struct sockaddr_in sin;     
    int retry = 0;
    int i ;
 Dprintf(D_simi, ("simi: openSimulator\n"));

    invalidateCache(XMEM_CACHE);
    invalidateCache(IMEM_CACHE);
    /* fork and start the simulator as a subprocess */
    if ((simPid = fork())) {
      Dprintf(D_simi, ("simi: simulator pid %d\n",(int) simPid));
    }
    else {
      /* we are in the child process : start the simulator */
      signal(SIGHUP , SIG_IGN );
      signal(SIGINT , SIG_IGN );
      signal(SIGABRT, SIG_IGN );
      signal(SIGCHLD, SIG_IGN );

      if (execvp(args[0],args) < 0) {
        perror("cannot exec simulator");
        exit(1);
      }
    }
    
 try_connect:
    sock = socket(AF_INET,SOCK_STREAM,0);
    
    memset(&sin,0,sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr("127.0.0.1");
    sin.sin_port = htons(9756);
    
    sleep(1);
    /* connect to the simulator */
    if (connect(sock,(struct sockaddr *) &sin, sizeof(sin)) < 0) {
	/* if failed then wait 1 second & try again
	   do this for 10 secs only */
	if (retry < 10) {
	    retry ++;
	    sleep (1);
	    goto try_connect;
	}
	perror("connect failed :");
	exit(1);
    }
    /* go the socket now turn it into a file handle */
    if (!(simin = fdopen(sock,"r"))) {
	fprintf(stderr,"cannot open socket for read\n");
	exit(1);
    }

    if (!(simout = fdopen(sock,"w"))) {
	fprintf(stderr,"cannot open socket for write\n");
	exit(1);
    }

    /* now that we have opened, wait for the prompt */
    waitForSim(200,NULL);
    simactive = 1;
}
/*-----------------------------------------------------------------*/
/* simResponse - returns buffer to simulator's response            */
/*-----------------------------------------------------------------*/
char *simResponse()
{
    return simibuff;
}

/*-----------------------------------------------------------------*/
/* sendSim - sends a command to the simuator                 */
/*-----------------------------------------------------------------*/
void sendSim(char *s)
{
    if ( ! simout ) 
        return;

    Dprintf(D_simi, ("simi: sendSim-->%s", s));  // s has LF at end already
    fputs(s,simout);
    fflush(simout);
}


static int getMemString(char *buffer, char wrflag, 
                        unsigned int addr, char mem, int size )
{
    int cachenr = NMEM_CACHE;
    char *prefix;
    char *cmd ;

    if ( wrflag )
        cmd = "set mem";
    else
        cmd = "dump";
    buffer[0] = '\0' ;

    switch (mem) 
    {
        case 'A': /* External stack */
        case 'F': /* External ram */
            prefix = "xram";
            cachenr = XMEM_CACHE;
            break;
        case 'C': /* Code */
        case 'D': /* Code / static segment */
            prefix = "rom";
            break;
        case 'B': /* Internal stack */  
        case 'E': /* Internal ram (lower 128) bytes */
        case 'G': /* Internal ram */
            prefix = "iram";
            cachenr = IMEM_CACHE;
            break;
        case 'H': /* Bit addressable */
        case 'J': /* SBIT space */
            cachenr = BIT_CACHE;
            if ( wrflag )
            {
                cmd = "set bit";
            }
            sprintf(buffer,"%s 0x%x\n",cmd,addr);
            return cachenr;
            break;
        case 'I': /* SFR space */
            prefix = "sfr" ;
            break;
        case 'R': /* Register space */ 
            if ( !wrflag )
            {
                cachenr = REG_CACHE;
                sprintf(buffer,"info reg\n");
                return cachenr;
            }
            prefix = "iram";
            /* get register bank */
            cachenr = simGetValue (0xd0,'I',1); 
            addr   += cachenr & 0x18 ;
            cachenr = IMEM_CACHE;
            break;
        default: 
        case 'Z': /* undefined space code */
            return cachenr;
    }
    if ( wrflag )
        sprintf(buffer,"%s %s 0x%x\n",cmd,prefix,addr,addr);
    else
        sprintf(buffer,"%s %s 0x%x 0x%x\n",cmd,prefix,addr,addr+size-1);
    return cachenr;
}

int simSetValue (unsigned int addr,char mem, int size, unsigned long val)
{
    char cachenr, i;
    char buffer[40];
    char *s;

    if ( size <= 0 )
        return 0;

    cachenr = getMemString(buffer,1,addr,mem,size);
    if ( cachenr < NMEM_CACHE )
    {
        invalidateCache(cachenr);
    }
    s = buffer + strlen(buffer) -1;
    for ( i = 0 ; i < size ; i++ )
    {
        sprintf(s," 0x%x", val & 0xff);
        s += strlen(s);
        val >>= 8;
    }
    sprintf(s,"\n");
    sendSim(buffer);
    waitForSim(100,NULL);
    simResponse();   
}


/*-----------------------------------------------------------------*/
/* simGetValue - get value @ address for mem space                 */
/*-----------------------------------------------------------------*/
unsigned long simGetValue (unsigned int addr,char mem, int size)
{
    unsigned int b[4] = {0,0,0,0}; /* can be a max of four bytes long */
    char cachenr, i;
    char buffer[40];
    char *resp;

    if ( size <= 0 )
        return 0;

    cachenr = getMemString(buffer,0,addr,mem,size);

    resp = NULL;
    if ( cachenr < NMEM_CACHE )
    {
        resp = getMemCache(addr,cachenr,size);
    }
    if ( !resp )
    {
        /* create the simulator command */
        sendSim(buffer);
        waitForSim(100,NULL);
        resp = simResponse();

        /* got the response we need to parse it the response
           is of the form 
           [address] [v] [v] [v] ... special case in
           case of bit variables which case it becomes
           [address] [assembler bit address] [v] */
        /* first skip thru white space */
        while (isspace(*resp)) resp++ ;

        if (strncmp(resp, "0x",2) == 0)
            resp += 2;

        /* skip thru the address part */
        while (isxdigit(*resp)) resp++;

        /* then make the branch for bit variables */
        if ( cachenr == REG_CACHE ) 
        {
            /* skip registers */
            for (i = 0 ; i < addr ; i++ ) 
            {
                while (isspace(*resp)) resp++ ;
                /* skip */
                while (isxdigit(*resp)) resp++;
            }
        }
    }   
    /* make the branch for bit variables */
    if ( cachenr == BIT_CACHE) 
    {
        /* skip until newline */
        while (*resp && *resp != '\n' ) resp++ ;
        if ( *--resp != '0' )
            b[0] = 1;
    }
    else 
    {	
        for (i = 0 ; i < size ; i++ ) 
        {
            /* skip white space */
            while (isspace(*resp)) resp++ ;
	    
            b[i] = strtol(resp,&resp,16);
        }
    }

    return b[0] | b[1] << 8 | b[2] << 16 | b[3] << 24 ;
	
}

/*-----------------------------------------------------------------*/
/* simSetBP - set break point for a given address                  */
/*-----------------------------------------------------------------*/
void simSetBP (unsigned int addr)
{
    char buff[50];

    sprintf(buff,"break 0x%x\n",addr);
    sendSim(buff);
    waitForSim(100,NULL);
}

/*-----------------------------------------------------------------*/
/* simClearBP - clear a break point                                */
/*-----------------------------------------------------------------*/
void simClearBP (unsigned int addr)
{
    char buff[50];

    sprintf(buff,"clear 0x%x\n",addr);
    sendSim(buff);
    waitForSim(100,NULL);
}

/*-----------------------------------------------------------------*/
/* simLoadFile - load the simulator file                           */
/*-----------------------------------------------------------------*/
void simLoadFile (char *s)
{
    char buff[128];

    sprintf(buff,"l \"%s\"\n",s);
    printf(buff);
    sendSim(buff);
    waitForSim(500,NULL);
}

/*-----------------------------------------------------------------*/
/* simGoTillBp - send 'go' to simulator till a bp then return addr */
/*-----------------------------------------------------------------*/
unsigned int simGoTillBp ( unsigned int gaddr)
{
    char *sr, *svr;
    unsigned addr ; 
    char *sfmt;
    int wait_ms = 1000;

    invalidateCache(XMEM_CACHE);
    invalidateCache(IMEM_CACHE);
    if (gaddr == 0) {
      /* initial start, start & stop from address 0 */
      char buf[20];

         // this program is setting up a bunch of breakpoints automatically
         // at key places.  Like at startup & main() and other function
         // entry points.  So we don't need to setup one here..
      //sendSim("break 0x0\n");
      //sleep(1);
      //waitForSim();

      sendSim("run 0x0\n");
    } else	if (gaddr == -1) { /* resume */
      sendSim ("run\n");
      wait_ms = 100;
    }
    else 	if (gaddr == 1 ) { /* nexti */
      sendSim ("next\n");
      wait_ms = 100;
    }
    else 	if (gaddr == 2 ) { /* stepi */
      sendSim ("step\n");
      wait_ms = 100;
    }
    else  { 
      printf("Error, simGoTillBp > 0!\n");
      exit(1);
    }

    waitForSim(wait_ms, NULL);
    
    /* get the simulator response */
    svr  = sr = strdup(simResponse());

    if ( gaddr == 1 || gaddr == 2 )
    {
        int nl;
        for ( nl = 0; nl < 3 ; nl++ )
        {
            while (*sr && *sr != '\n') sr++ ;
            sr++ ;
        }
        if ( nl < 3 )
            return 0;
        gaddr = strtol(sr,0,0);
        /* empty response */
        simibuff[0] = '\0';
        return gaddr;
        
    }
    /* figure out the address of the break point the simulators 
       response in a break point situation is of the form 
       [... F* <addr> <disassembled instruction> ] 
       we will ignore till we get F* then parse the address */
    while (*sr) {
      if (strncmp(sr,"Stop at",7) == 0) {
          sr += 7;
          sfmt = "%x";
          break;
      } 

      if (*sr == 'F' && ( *(sr+1) == '*' || *(sr+1) == ' ')) {
          sr += 2;
          sfmt = "%x";
          break;
      }
      sr++;
    }

    if (!*sr) {
      fprintf(stderr, "Error?, simGoTillBp failed to Stop\n");
      return 0;
    }

    while (isspace(*sr)) sr++ ;

    if (sscanf(sr,sfmt,&addr) != 1) {
      fprintf(stderr, "Error?, simGoTillBp failed to get Addr\n");
      return 0;
    }
	return addr;

}

/*-----------------------------------------------------------------*/
/* simReset - reset the simulator                                  */
/*-----------------------------------------------------------------*/
void simReset ()
{
    invalidateCache(XMEM_CACHE);
    invalidateCache(IMEM_CACHE);
    sendSim("res\n");
    waitForSim(100,NULL);
}

/*-----------------------------------------------------------------*/
/* getValueStr - read a value followed by a string =               */
/*-----------------------------------------------------------------*/
static unsigned int getValueStr (char *src,char *cstr)
{
    int i = strlen(cstr);
    int rv;
    /* look for the string */
    if (! (src = strstr(src,cstr)))
	return 0;

    src += i;
    if (!*src) return 0;

    /* look for the digit */
    while (*src && !isxdigit(*src)) src++;
    sscanf(src,"%x",&rv);
    return rv;
}

/*-----------------------------------------------------------------*/
/* simRegs - returns value of registers                            */
/*-----------------------------------------------------------------*/
char  *simRegs()
{   
    char *resp ;
    unsigned int rv;
    char *rb = regBuff;
    int i;

    sendSim("info registers\n");

    waitForSim(100,NULL);
         
    resp  = simResponse();

#if 0
    return resp;

#else
    /*Take this out(2-09-02) cant see as its that useful to reformat, karl.*/
  
    /* the response is of the form 
       XXXXXX R0 R1 R2 R3 R4 R5 R6 R7 ........
       XXXXXX XX . ACC=0xxx dd cc B=0xxx dd cc DPTR= 0xxxxx @DPTR= 0xxx dd cc
       XXXXXX XX . PSW= 0xxx CY=[1|0] AC=[0|1] OV=[0|1] P=[1|0]

Format as of 8-4-01:
       0x00 00 00 00 00 00 00 00 00 ........
       000000 00 .  ACC= 0x00   0 .  B= 0x00   DPTR= 0x0000 @DPTR= 0x00   0 .
       000000 00 .  PSW= 0x00 CY=0 AC=0 OV=0 P=0
F  0x006d 75 87 80 MOV   PCON,#80
*/

    memset(regBuff,0,sizeof(regBuff));
    /* skip the first numerics */
    while (*resp && !isxdigit(*resp)) resp++;

    if (strncmp(resp, "0x", 2)) {
      fprintf(stderr, "Error: Format1A\n");
      return regBuff;
    }
    resp += 2;
    while (*resp && isxdigit(*resp)) resp++;

    /* now get the eight registers */
    for (i = 0 ; i < 7 ; i++) {
        while (*resp && isspace(*resp)) resp++;
	if (!*resp)
	    break;
	rv = strtol(resp,&resp,16);
	sprintf(rb,"R%d  : 0x%02X %d %c\n",i,rv,rv,(isprint(rv) ? rv : '.'));
	rb += strlen(rb);
    }

    if (!*resp) return regBuff;

    /* skip till end of line */
    while (*resp && *resp != '\n') resp++;
    while (*resp && !isxdigit(*resp)) resp++;
    while (*resp && isxdigit(*resp)) resp++;
    
    /* accumulator value */
    rv = getValueStr(resp,"ACC");
    sprintf(rb,"ACC : 0x%02X %d %c\n",rv,rv,(isprint(rv) ? rv : '.'));
    rb += strlen(rb);
    
    /* value of B */
    rv = getValueStr(resp,"B=");
    sprintf(rb,"B   : 0x%02X %d %c\n",rv,rv,(isprint(rv) ? rv : '.'));
    rb += strlen(rb);

    rv = getValueStr(resp,"DPTR=");
    sprintf(rb,"DPTR: 0x%04X %d\n",rv,rv);
    rb += strlen(rb);

    rv = getValueStr(resp,"@DPTR=");
    sprintf(rb,"@DPTR: 0x%02X %d %c\n", rv,rv,(isprint(rv) ? rv : '.'));
    rb += strlen(rb);
    
    sprintf(rb,"PSW  : 0x%02X | CY : %d | AC : %d | OV : %d | P : %d\n",
	    getValueStr(resp,"PSW="),
	    getValueStr(resp,"CY="),
	    getValueStr(resp,"AC="),
	    getValueStr(resp,"OV="),
	    getValueStr(resp,"P="));

    return regBuff;
#endif

}

/*-----------------------------------------------------------------*/
/* closeSimulator - close connection to simulator                  */
/*-----------------------------------------------------------------*/
void closeSimulator ()
{
    if ( ! simin || ! simout || sock == -1 )
    {
        simactive = 0;
        return;
    }
    sendSim("q\n");
    kill (simPid,SIGKILL);
    fclose (simin);
    fclose (simout);
    shutdown(sock,2);   
    close(sock);    
    sock = -1;
}


