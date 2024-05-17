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
	shmid=shmget(SHMKEY,1024,0777);		/*��ȡ������*/
	addr=(int *)shmat(shmid,0,0);			/*��������ʼ��ַΪaddr*/
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
	shmid=shmget(SHMKEY,1024,0777|IPC_CREAT);		/*����������*/
	addr=(int *)shmat(shmid,0,0);					/*��������ʼ��ַΪaddr*/
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
		SERVER();		/*�ӽ���server������Ϣ*/
	while((i=fork())==-1);
	if(!i)
		CLIENT();		/*�ӽ���client������Ϣ*/
	wait(0);
	wait(0);
}
