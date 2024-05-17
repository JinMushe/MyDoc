#include <stdio.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/shm.h>
#define SHMKEY 75

int shmid,i;
int * addr;

int CLIENT()				
{
	int i;
	shmid=shmget(SHMKEY,1024,0777);		
	addr=(int *)shmat(shmid,0,0);		
	for(i=9;i>=0;i--)
	{
		while(*addr!=-1);
		printf("(client)sent\n");
		*addr=i;
	}
	exit(0);
}

int SERVER()				
{
	shmid=shmget(SHMKEY,1024,0777|IPC_CREAT);		
	addr=(int *)shmat(shmid,0,0);					
	do
	{
		*addr=-1;
		while(*addr==-1);
		printf("(server)received\n");
	}while(*addr);
	shmctl(shmid,IPC_RMID,0);
	exit(0);
}

int main()
{
	while((i=fork())==-1);
	if(!i)
		SERVER();		
	while((i=fork())==-1);
	if(!i)
		CLIENT();
	wait(0);
	wait(0);
	return 0;
}
