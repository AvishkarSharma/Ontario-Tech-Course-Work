/* -----------------------------------------------------------
 * OS Security Assign - Target Application to Confine
 * -----------------------------------------------------------
 * Imagine this is a custom application that your organization
 * wants to run, and that there is some risk in running it, 
 * so you have been asked to confine it using SELinux.
 * 
 * Refer to the Week 8 SELinux material for a good starting point 
 * for learning how to confine a process.  
 *
 *
 * Compile by running gcc -o FirstnameLastname_app targetprogram.c
 */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

int
main (void)
{
  /* Note that you will need to create this file! Put 
     some parameters (of your choice!) in params.txt 
     for your testing. */

  static const char paramsfname[] = "./priv/params.txt";
  static const char recordsfname[] = "./priv/records.txt";
  FILE *pfile = fopen (paramsfname, "r");
  FILE *rfile = fopen (recordsfname, "a");
  time_t sec;
  char str[256] = "";
  unsigned int counter = 0;
  char *lsparams[] = { "/usr/bin/strings", "/usr/sbin/passwd", NULL };
  sec = time (NULL);
  snprintf (str, 256, "%s%d\n", "params READ at: ", sec);

  if (rfile != NULL)
    {
      if (pfile != NULL)
        {
          char line[255];

          while (fgets (line, sizeof line, pfile) != NULL) /* read a line */
            {
              fputs (line, stdout); /* write the line */
              counter++;
            }

          fwrite (str, 1, strlen (str), rfile); /*append a record */
          fclose (pfile);

          /* Here you can imagine all sorts of other things happen
             with the target application, but I'm being easy on you
             since this is already difficult enough :) */
        }
      else
        {
          perror (paramsfname); /* why didn't the file open? */
        }
      fclose (rfile);
    }
  else
    {
      perror (recordsfname); /* why didn't the file open? */
    }

  if (counter < 2)
    {
      execve ("/usr/bin/strings", lsparams, NULL);
      printf ("error on running: %i\n", errno);
    }
  else
    {
      printf ("You have a large params file, that's good to start with for testing...\n");
    }

  sleep (60); /* This gives you a minute to verify the SELinux domain by typing ps -auZ */
  return 0;
}
