#include <stdio.h>
struct pl_type
{
	int pn,pfn;
};

struct pfc_struct
{
	int pn,pfn;
	struct pfc_struct * next;
};

int s[320];					/*页面访问序列*/
struct pl_type pl[32];		/*页面数组*/
struct pfc_struct pfc[32];	/*用来构造帧队列*/
struct pfc_struct * freepf_head,* busypf_head,* busypf_tail;

void initial(int pf);
void fifo(int pf);

main()
{
	int i,total_pf;

	srand(getpid());			/*构造页面访问序列*/
	for(i=0;i<320;i++)
	{
		s[i]=rand()%32;
	}

	for(total_pf=4;total_pf<=32;total_pf++)		/*帧数从4到32*/
	{
		initial(total_pf);		/*初始化相关数据结构*/
		fifo(total_pf);			/*计算FIFO的命中率*/
	}
}

void initial(int pf)
{
	int i;

	for(i=0;i<32;i++)			/*初始化页面数组*/
	{
		pl[i].pn=i;
		pl[i].pfn=-1;
	}

	for(i=0;i<pf-1;i++)			/*初始化帧，并将所有帧放入空闲帧队列，被占用帧队列为空*/
	{
		pfc[i].pfn=i;
		pfc[i].next=&pfc[i+1];
	}
	pfc[pf-1].pfn=pf-1;
	pfc[pf-1].next=NULL;

	freepf_head=&pfc[0];
	busypf_head=NULL;
	busypf_tail=NULL;
}

void fifo(int pf)
{
	int i,diseffect=0;			/*diseffect用来记录页面失效次数*/
	struct pfc_struct *p;

	for(i=0;i<320;i++)				/*处理页面访问序列，计算失效数*/
	{
		if(pl[s[i]].pfn==-1)		/*被访问页面不在内存中，失效数加1并将其调入内存*/
		{
			diseffect+=1;			

			if(freepf_head==NULL)		/*无空闲帧，则从被占帧队列中按FIFO释放第一个被占帧，将其加入空闲帧队列*/
			{
				p=busypf_head->next;
				pl[busypf_head->pn].pfn=-1;
				freepf_head=busypf_head;
				freepf_head->next=NULL;
				busypf_head=p;
			}

			p=freepf_head->next;		/*为待调入页面分配一帧，并将该帧挂到被占帧队列队尾*/
			freepf_head->next=NULL;
			freepf_head->pn=s[i];
			pl[s[i]].pfn=freepf_head->pfn;

			if(busypf_tail==NULL)
				busypf_head=busypf_tail=freepf_head;
			else
			{
				busypf_tail->next=freepf_head;
				busypf_tail=freepf_head;
				busypf_tail->next=NULL;
			}
			freepf_head=p;
		}
	}
	printf("%d frames %f\n",pf,1-(float)diseffect/320);
}