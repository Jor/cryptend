/*
    Frontend to crypt() libc function.

    cryptend.c

    Compile:
	Linux: cc -o cryptend cryptend.c -lcrypt
	POSIX: cc -o cryptend cryptend.c

    Written by Juergen P. Meier

    This is Free Software (Free as in Speech and Beer) and open source.

    Generates Passwordhashes useable for unix shadow, apache .htpasswd,
    Cisco type 5 secrets (uses md5 hashes with 5-letter seeds) etc.

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#ifndef crypt
# include <crypt.h>
#endif

int main(int argc, char **argv)
{
 const char letters[]="./abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
 char md5seed[1024]="$1$";
 char *ptr,*seed;
 char def[3];
 int i;
 int len=sizeof(letters) - 1;

 switch (argc)
   {
   case 2:
    {
       srand((int) getpid());
       def[0]=letters[rand() % len];
       def[1]=letters[rand() % len];
       def[2]=0;
       seed=def;
       break;
     }
   case 3:
     {
       seed=argv[2];
       if ((strlen(seed) == 3) && (strncmp(seed,"$1$",3) == 0)) {
         srand((int) getpid());
         i = (rand() % 4) + 3;
         md5seed[i + 3 + 2]=0;
         md5seed[i + 3 + 1]='$';
	 while ( i >= 0 ) {
           md5seed[i + 3]=letters[rand() % len];
           i--;
         }
         seed=md5seed;
       }
       break;
     }
   default:
     fprintf(stderr,"%s: Invalid number of arguments.\n",argv[0]);
     fprintf(stderr,"Usage: %s text [seed]\n",argv[0]);
     fprintf(stderr,"\ttext - the text that will be hashed (a password).\n");
     fprintf(stderr,"\tseed - an optional two-letters seed.\n");
     fprintf(stderr,"If seed starts with $1$, an MD5 hash is generated, see crypt(3).\n");
     fprintf(stderr,"If seed is $1$, then this tool will provide a random seed.\n");
     exit (1);
   }
 ptr = (char *) crypt(argv[1],seed);
 printf("%s\n",ptr);
#ifdef DEBUG
  fprintf(stderr,"PWD=%s:SEED=%s\n",argv[1],seed);
#endif
 exit(0);
}
/*EOF*/
