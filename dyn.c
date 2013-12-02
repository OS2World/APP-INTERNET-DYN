/*
   DYN -- a simple DYNdns client for the Monolith Coalition

   Written by Jon Klippenstein <random@ocii.com>
   Copyright (C) 1997 Jon Klippenstein

   Network code based in part on fetchwww.c 1.0 by Artur Skawina
   <skawina@usa.net>

   The dyn.ml.org domain is hardcoded in, this will have to be changed if
   the dynamic domain changes.

   OS'es tested on:
   Linux 2.0.27 i386 (my 386DX/40 infinity.dyn.ml.org/dparrot.bohica.net (UUCP))
   SunOS 5.5 sparc (my ISP: nephi.ocii.com)
   FreeBSD


   Version: 1.0.4

   $Header: /root/Projects/dyn-1.0.4/RCS/dyn.c,v 1.6 1997/05/22 01:16:05 root Exp $

   $Log: dyn.c,v $
   Revision 1.6  1997/05/22 01:16:05  root
   fixed small stupid bug made by me :)

   Revision 1.5  1997/05/22 00:36:28  root
   Added a few things, like commandline hiding, and support for expire-time and expire-date, and tagline.
   generally cleaned up the source a bit.

   Revision 1.4  1997/02/01 20:36:46  root
   added sysname,machine check with uname
   added syslog stuff
   added sleep 5 bit to readme

   Revision 1.3  1997/01/31 23:52:42  root
   Added major funtionality
   Added usage message
   Added magic token checking
   Added whole pile 'o crap :)

   Revision 1.2  1997/01/31 03:36:24  root
   Made it work right.
   Using some of the network code from Artur Skawina's fetchwww.c

   Revision 1.1  1997/01/31 01:10:50  root
   Initial revision

   OS/2 Note:  #define SYSLOG	   and compile with    -lsyslog
			   if you want to use it and have found syslog.a, I didn't.
 */

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/utsname.h>	/* for uname functions */
#ifdef SYSLOG
#include <syslog.h>
#endif

#define NAME		"DYN"
#define VERSION 	"1.0.4"

/* Uncomment DEBUG if you want to see raw HTML output from the server
   (not neccessary unless you REALLY want to know :)) */
/* NOTE: could also add -DDEBUG on gcc commandline */
/* #define		DEBUG */

/* These are just in here so I can change them easily if the server or
   path the freed.cgi changes (or if the WWW port number changes!! :)) */
#define WWW_PORT	80
#define SERVER		"scripts.ml.org"
#define SCRIPT		"/mis-bin/freed.cgi"

int strtcmp(char *str1, char *str2)
{
  return (strncmp(str1, str2, strlen(str2)));
}

void nuke_string(char *targetstring)
{
  char *mystring = targetstring;
  while (*targetstring != '\0') {
	*targetstring = ' ';
	targetstring++;
  }
  *mystring = '\0';
}

int main(int argc, void *argv[])
{
  struct sockaddr_in sin;
  struct hostent *host;
  int sock, port, connected;
  char send[1024];
  char temp[1024];
  int i;
  FILE *rfile;
  struct utsname uts;
  int sec1, sec2, sec3;
  char mid[10];
  char hostname[10];
  char ip[16];
  unsigned long int rawip;

  printf("%s %s by Jon Klippenstein <random@ocii.com>  Copyright (C) 1997\n", NAME, VERSION);
  printf("Modified and compiled for OS/2 by Samuel Audet <guardia@cam.org>\n");
#ifdef DEBUG
  printf("*** DEBUG MODE ***\n");
#endif
#ifdef SYSLOG
  openlog(argv[0], LOG_PID, LOG_USER);
#endif

  if (argc < 6) {
	printf("\nusage: %s MID SEC1 SEC2 SEC3 MACHINE IP\n", argv[0]);
	printf("\tMID\tyour Monolith Coalition ID\n");
	printf("\tSEC1\tthe first number in your security code\n");
	printf("\tSEC2\tthe second number in your security code\n");
	printf("\tSEC3\tthe third number in your security code\n");
	printf("\tMACHINE\tyour Dynamic Domain Name, such as xpl.\n");
	printf("\tIP\tyour machine's CURRENT dynamic IP (optional).\n\n");
	printf("Example: %s klip1 11 22 33 xpl 123.123.123.123 \n\n", argv[0]);
	exit(1);
  }
  uname(&uts);

  strcpy(mid, argv[1]);
  sec1 = atoi(argv[2]);
  sec2 = atoi(argv[3]);
  sec3 = atoi(argv[4]);
  strcpy(hostname, argv[5]);
  if (argc > 6) {
	 strcpy(ip, argv[6]);
	 nuke_string(argv[6]);
  }
  else {
	 rawip = gethostid();
	 sprintf(ip, "%d.%d.%d.%d", rawip>>24,rawip<<8>>24,rawip<<16>>24,rawip<<24>>24);
  }

  nuke_string(argv[0]);
  nuke_string(argv[1]);
  nuke_string(argv[2]);
  nuke_string(argv[3]);
  nuke_string(argv[4]);
  nuke_string(argv[5]);
  sprintf(argv[0], "%s %s", NAME, VERSION);

  printf("Host:\t%s.dyn.ml.org\n", hostname);
  printf("IP:\t%s\n", ip);
  printf("MID:\t%s\n", mid);
  printf("MACTYP:\t%s\n", uts.machine);
  printf("OS:\t%s\n", uts.sysname);

  sin.sin_family = AF_INET;
  sin.sin_port = htons(80);

  if (!(host = gethostbyname(SERVER))) {
#ifdef SYSLOG
	syslog(LOG_WARNING, "gethostbyname: %m");
#endif
	perror("gethostbyname");
	exit(1);
  }
  memcpy((char *) &sin.sin_addr, host->h_addr, host->h_length);

  printf("\nConnecting to %s...", SERVER);

  if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
#ifdef SYSLOG
	syslog(LOG_WARNING, "socket: %m");
#endif
	perror("socket");
	exit(1);
  }
  if (connect(sock, (struct sockaddr *) &sin, sizeof(sin)) == -1) {
#ifdef SYSLOG
	syslog(LOG_WARNING, "connect: %m");
#endif
	perror("connect:");
	exit(1);
  }
  printf("connnected.\n");

  /* Now create line to send to HTTP server */

  printf("Sending update...");

  /* NEW 1.0.4: Added expire-date and expire-time fields for expiration,
	 if it ever becomes real.  For now they are set to 0.  Also added tagline
	 support.  Don't know if these have been implemented on the server
	 yet.  Removed mactype and os. */

  /* This sprintf is REALLY ugly.  Do something about it Jon :) */

  sprintf(send, "GET %s?do=mod\
&type=machine\
&domain=%s\
&db=DYN.ML.ORG\
&ipaddr=%s\
&WWW=yes\
&mail=\
&mactype=%s\
&os=%s\
&expire-date=0\
&expire-time=0\
&tagline=DYN+%s/%s\
&MID=%s\
&sec1=%d\
&sec2=%d\
&sec3=%d\
&agree=agree\n",
	  SCRIPT,
	  hostname,
	  ip,
	  uts.machine,
	  uts.sysname,
	  VERSION,
	  uts.sysname,
	  mid,
	  sec1,
	  sec2,
	  sec3);

#ifdef DEBUG
  printf("\n\n%s\n\n", send);
#endif

  write(sock, send, strlen(send));

  printf("done.\n");

  printf("Waiting for reply...");

  rfile = fdopen(sock, "r");

  while (!feof(rfile)) {
	fgets(temp, (int) sizeof(temp), (FILE *) rfile);

#ifdef DEBUG
	printf("%s", temp);
#endif

/* Check for the ml-host-added magic token as explained by Aveek Datta:

   MTTS has been updated to remove the "DYNDNS isn ot available yet".  And as
   someone asked, there is now a magic token in the return that you can test
   for:

   <!-- ML-HOST-ADDED -->

   is present if the host was added/modified OK

 */

	if (strtcmp(temp, "<!-- ML-HOST-ADDED -->") == 0) {
	  printf("\n\n%s [%s] was updated successfully.\n", hostname, ip);
#ifdef SYSLOG
	  syslog(LOG_INFO, "%s [%s] update successful.", hostname, ip);
#endif
	  fclose(rfile);
	  close(sock);
	  exit(0);
	}
  }

  printf("\nERROR: ML-HOST-ADDED magic token not found in reply.\nHost NOT updated.\n");
#ifdef SYSLOG
  syslog(LOG_ERR, "ERROR: ML-HOST-ADDED not found, update unsuccessful.");
#endif
  fclose(rfile);
  close(sock);
}
