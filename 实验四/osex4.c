#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
	int i,r,p1,p2,fd[2];
	char buf[50],s[50];
	pipe(fd);
	while((p1=fork())==-1);
	if(p1==0)
	{
		lockf(fd[1],1,0);
		sprintf(buf,"child process P1 is sending messages! \n");
		printf("child process P1! \n");
		write(fd[1],buf,50);
		sleep(5);
		lockf(fd[1],0,0);
		exit(0);
	}
	else
	{
		while((p2=fork())==-1);
		if(p2==0)
		{
			lockf(fd[1],1,0);
			sprintf(buf,"child process P2 is sending messages! \n");
			printf("child process P2! \n");
			write(fd[1],buf,50);
			sleep(5);
			lockf(fd[1],0,0);
			exit(0);
		}
	}
	wait(0);
	if((r=read(fd[0],s,50))==-1)
		printf("can't read pipe\n");
	else
		printf("%s\n",s);
	wait(0);
	if((r=read(fd[0],s,50))==-1)
		printf("can't read pipe\n");
	else
		printf("%s\n",s);
	exit(0);
}
