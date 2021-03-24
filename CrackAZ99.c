#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <unistd.h>
#include <pthread.h>
/******************************************************************************
  Demonstrates how to crack an encrypted password using a simple
  "brute force" algorithm. Works on passwords that consist only of 2 uppercase
  letters and a 2 digit integer.

  Compile with:
    cc -o CrackAZ99 CrackAZ99.c -lcrypt

  If you want to analyse the output then use the redirection operator to send
  output to a file that you can view using an editor or the less utility:
    ./CrackAZ99 > output.txt

pthread library - google for C pthread 

******************************************************************************/

int count=0;     // A counter used to track the number of combinations explored so far

struct arg_struct {
  char *salt_and_encrypted;
  char start;
  char end;
};

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

void* crackThread(void *arguments)
{
  struct arg_struct *args = (struct arg_struct *)arguments;

  printf("pThread Start: Start = %c, End = %c, Cipher = %s\n", args->start, args->end, args->salt_and_encrypted);

  int x, y, z;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space for \0 // incase you have modified the salt value, then should modifiy the number accordingly
  char plain[7];   // The combination of letters currently being checked // Please modifiy the number when you enlarge the encrypted password.
  char *enc;       // Pointer to the encrypted password

  char *salt_and_encrypted = args->salt_and_encrypted;

  substr(salt, salt_and_encrypted, 0, 6);
  for(x=args->start; x < args->end; x++){    
    for(y='A'; y<='Z'; y++){
      for(z=0; z<=99; z++){
        sprintf(plain, "%c%c%02d", x, y, z); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
	        printf("#%-8d%s %s\n", count, plain, enc);
        } 
      }
    }
  }
}

void crack(char *salt_and_encrypted){
  int x, y, z;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space for \0 // incase you have modified the salt value, then should modifiy the number accordingly
  char plain[7];   // The combination of letters currently being checked // Please modifiy the number when you enlarge the encrypted password.
  char *enc;       // Pointer to the encrypted password

  substr(salt, salt_and_encrypted, 0, 6);

  for(x='A'; x<='Z'; x++){
    for(y='A'; y<='Z'; y++){
      for(z=0; z<=99; z++){
        sprintf(plain, "%c%c%02d", x, y, z); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
	        printf("#%-8d%s %s\n", count, plain, enc);
        } 
      }
    }
  }
}

int main(int argc, char *argv[]){
  printf("Start Program\n");
  char cipher[] = "$6$AS$trzArPfsZZhu.mUTHuJzlZH97mFsPhbLg0Xuan.BKx7JeRMSxwj7e7XKK3DE.B4Qy8Pq1pPgYGNCvoa6MuJEI.";
  // crack(cipher);		//Copy and Paste your ecrypted password here using EncryptShA512 program

  // Let us create three threads
  int count = 6;
  int interval = 26 / count;
  for(int i = 0; i < count + 1; i++)
  {
    struct arg_struct *args = (struct arg_struct*) malloc(sizeof(struct arg_struct));
    args->salt_and_encrypted = (char *) malloc(strlen(cipher) + 1);
    strcpy(args->salt_and_encrypted, cipher);
    args->start = 'A' + i * interval;
    args->end = args->start + interval;
    if( args->end > 'Z' )
      args->end = 'Z' + 1;

    printf("pThread Create: Start = %c, End = %c, Cipher = %s\n", args->start, args->end, args->salt_and_encrypted);

    pthread_t tid;
    pthread_create(&tid, NULL, crackThread, (void *)args);
  }

  // printf("%d solutions explored\n", count);
  pthread_exit(NULL);

  return 0;
}

