#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <signal.h>
#include <sys/mman.h>


int blocksize;
char*ptr=NULL;
void* thread_job(void *arg)
{
int i=*(int*)arg;
int pos=i*blocksize;
printf("thread 0x%x,i=%d pos=%d,blocksize=%d\n",(unsigned int)pthread_self(),i,pos,blocksize);
char buf[blocksize];
bzero(buf,sizeof(buf));
snprintf(buf,blocksize+1,"%s",ptr+pos);
int dest_fd=open("00.c",O_RDWR|O_CREAT,0664);
lseek(dest_fd,pos,SEEK_SET);
write(dest_fd,buf,strlen(buf));
close(dest_fd);
pthread_exit(NULL);
}



int main(int argc,char**argv)
{
int	fd=open("FILES",O_RDONLY);
	int fsize=lseek(fd,0,SEEK_END);
	if((ptr=mmap(NULL,fsize,PROT_READ,MAP_PRIVATE,fd,0))==MAP_FAILED)
	{
	perror("MMAP FAILED:");
	exit(0);
	}
	if(fsize%10==0)
	{
	blocksize=fsize/10;
	}else
	{
	blocksize=fsize/10+1;
	}

	pthread_t tid[10];
	int i;
	for(i=0;i<10;i++)
	{
	pthread_create(&tid[i],NULL,thread_job,(void*)&i);
	usleep(10000);
	}
	while(i--)
	{
	pthread_join(tid[i],NULL);
	}
	munmap(ptr,fsize);
	close(fd);
	return 0;
}



