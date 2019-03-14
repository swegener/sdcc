/******************************************************************************
 * to emulate the serial input and output of an 8051 controller               *
 * main.cc - the main stuff                                                   *
 ******************************************************************************/
#include "ddconfig.h"

#include <sys/types.h>
#include <iostream>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#if defined(HAVE_GETOPT_H)
# include <getopt.h>
#endif

#include "fileio.hh"
#include "frontend.hh"
#include "posix_signal.hh"


// globals
int doloop = 1;

// the signal handler
void HandleSig(int info)
{
  doloop = 0;
}


// usage
void PrintUsage(char *progname)
{
  std::cout << "Usage: " << progname << " [-i <filename>] [-o <filename>] [-hIO]\n";
  std::cout << "-i <filename>\t<filename> is the pipe to the controllers' serial input\n";
  std::cout << "-o <filename>\t<filename> is the pipe to the controllers' serial output\n";
  std::cout << "-I \t\thexa filter on input\n";
  std::cout << "-O \t\thexa filter on output\n";
  std::cout << "-L n\tSet line length of hex dump in output panel (def=8)\n";
  std::cout << "-h\t\tshow the help\n";
  std::cout << "\nTim Hurman - t.hurman@virgin.net\n";
  exit(0);
}


// the main function
int main(int argc, char **argv)
{
  char *string = new char[MAX_SIZ];
  extern char *optarg;
  int errflg=0;
  int c, l= 8;
  enum filter_t fi= flt_none, fo= flt_none;
  const char *infile = DEF_INFILE;
  const char *outfile = DEF_OUTFILE;
  
  // sort out any command line params
  while ((c = getopt(argc, argv, "i:o:hOIL:")) != EOF)
    switch(c) {
    case 'i':
      infile = optarg;
      break;
    case 'o':
      outfile = optarg;
      break;
    case 'I':
      fi= flt_hex;
      break;
    case 'O':
      fo= flt_hex;
      break;
    case 'L':
      l= strtol(optarg, 0, 0);
      break;
    case 'h':
      errflg++;
      break;
    default:
      std::cerr << "Invalid or unknown switch\n";
      errflg++;
      break;
    }
  
  // was there a problem
  if(errflg)
    PrintUsage(argv[0]);
  
  // the main objects needed
  Viewer *view = new Viewer();
  FileIO *fobj = new FileIO(infile, outfile);
  SigHandler *sig = new SigHandler();
  
  view->iflt_mode(fi);
  view->oflt_mode(fo);
  view->set_length(l);
  
  // add a signal handler for ^C
  sig->SetSignal(SIGINT, HandleSig);
  
  // set the timeout for waiting for a char
  fd_set s;
  while(doloop)
    {
      int ret, i;
      FD_ZERO(&s);
      FD_SET(fileno(stdin), &s);
      FD_SET(fobj->infile_id(), &s);

      i= select(FD_SETSIZE, &s, NULL, NULL, NULL);
      if (i >= 0)
	{
	  if (FD_ISSET(fileno(stdin), &s))
	    {
	      ret= view->GetChInWin(&string[0]);
	      if (ret > 0)
		{
		  fobj->SendByte(string[0]);
		}
	      else if (ret < 0)
		break;
	    }

	  if (FD_ISSET(fobj->infile_id(), &s))
	    {
	      if (fobj->RecvByte(string) > 0)
		view->AddChOutWin(string[0]);
	    }
	}
      //usleep(5000);
    }
  
  delete fobj;
  delete view;
  delete sig;
  delete string;
  return(0);
}
