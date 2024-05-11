#include <stdio.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#define MSGKEY 75
 
struct msgform
{
	long mtype;
	char mtrex[1030];
}msg;
 
int msgqid,i;
 
int CLIENT()				
{
	int i;
	msgqid=msgget(MSGKEY,0777);
	for(i=10;i>=1;i--)
	{
		msg.mtype=i;
		printf("(client)sent\n");
		msgsnd(msgqid,&msg,1024,0);
	}
	exit(0);
}
 
int SERVER()				
{
	msgqid=msgget(MSGKEY,0777|IPC_CREAT);		
	do
	{
		msgrcv(msgqid,&msg,1030,0,0);
		printf("(server)received\n");
	}while(msg.mtype!=1);
	msgctl(msgqid,IPC_RMID,0);
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