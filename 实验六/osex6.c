#include <stdio.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>

#define SHMKEY 75

int shmid,i;
int * addr;

CLIENT()				
{
	int i;
	shmid=shmget(SHMKEY,1024,0777);		/*获取共享区*/
	addr=(int *)shmat(shmid,0,0);			/*共享区起始地址为addr*/
	for(i=9;i>=0;i--)
	{
		while(*addr!=-1);
		printf("(client)sent\n");
		*addr=i;
	}
	exit(0);
}

SERVER()				
{
	shmid=shmget(SHMKEY,1024,0777|IPC_CREAT);		/*创建共享区*/
	addr=(int *)shmat(shmid,0,0);					/*共享区起始地址为addr*/
	do
	{
		*addr=-1;
		while(*addr==-1);
		printf("(server)received\n");
	}while(*addr);
	shmctl(shmid,IPC_RMID,0);
	exit(0);
}

main()
{
	while((i=fork())==-1);
	if(!i)
		SERVER();		/*子进程server接收消息*/
	while((i=fork())==-1);
	if(!i)
		CLIENT();		/*子进程client发送消息*/
	wait(0);
	wait(0);
}
