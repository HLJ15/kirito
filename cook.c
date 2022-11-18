#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <pthread.h>
#include <string.h>


pthread_mutex_t lock1;
pthread_mutex_t lock2;
pthread_mutex_t lock3;
pthread_t tid;
pthread_cond_t No_empty;
pthread_cond_t No_full;
char buffA[1024];
char buffB[1024];
int fd;

void* jobA(char* arg)
{
pthread_detach(pthread_self());
 buffA[1024]=NULL;
 pthread_mutex_lock(&lock1);
int size=lseek(fd,0,SEEK_END);
fwrite(fd,1,size,buffA);
printf("write to buffA sucess..\n");
pthread_cond_signal(&No_empty);
pthread_mutex_unlock(&lock1);
pthread_exit(NULL);
}

void* thread_B(char*arg)
{
pthread_detach(pthread_self());
char buffB[1024];
pthread_mutex_lock(&lock2);

while(1)
{
while(!buffA)
{
pthread_cond_wait(&No_empty,&lock2);
}
char string[]="E CamX";
char string2[]="CHIUSECASE";

int number=strcmp(buffA,string);
int number2=strcmp(buffA,string2);
if(number==1)
{
fwrite(string,1,sizeof(string),buffB);
lseek(buffB,0,SEEK_END);
}

if(number2==1)
{
fwrite(string2,1,sizeof(string2),buffB);
lseek(buffB,0,SEEK_END);
}
}
pthread_cond_signal(&No_full);
pthread_mutex_unlock(&lock2);
pthread_exit(0);
}



int strcmp(const char*str1,const char* str2)
{
while(*str1==*str2)
{
if(*str2=='\0')
{
  return 1;
  }
    str1++;
	str2++;
}
return 0;
}


void* thread_c(void* arg)
{
pthread_detach(pthread_self());
int fd=open("RESULT.c",O_RDWR|O_CREAT,0664);
pthread_mutex_lock(&lock3);
while(1)
{
while(!buffB)
{
pthread_cond_wait(&No_full,&lock3);
}
int size=lseek(fd,0,SEEK_END);
fwrite(buffB,1,size,fd);
}
pthread_mutex_unlock(&lock3);
pthread_exit(0);
}

int main(void)
{
fd=open("ERROR.txt",O_RDWR,0664);
pthread_create(&tid,NULL,jobA,NULL);
pthread_create(&tid,NULL,thread_B,NULL);
pthread_create(&tid,NULL,thread_c,NULL);
if(pthread_mutex_init(&lock1,NULL)!=0||pthread_mutex_init(&lock2,NULL)!=0||pthread_mutex_init(&lock3,NULL)!=0||pthread_cond_init(&No_full,NULL)!=0||pthread_cond_init(&No_empty,NULL)!=0)
{
perror("init lock or cond failed");
exit(0);
}
while(1)
sleep(1);
pthread_mutex_destroy(&lock1);
pthread_mutex_destroy(&lock2);
return 0;
}

