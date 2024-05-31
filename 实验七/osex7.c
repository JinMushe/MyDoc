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

int s[320];					/*ҳ���������*/
struct pl_type pl[32];		/*ҳ������*/
struct pfc_struct pfc[32];	/*��������֡����*/
struct pfc_struct * freepf_head,* busypf_head,* busypf_tail;

void initial(int pf);
void fifo(int pf);

main()
{
	int i,total_pf;

	srand(getpid());			/*����ҳ���������*/
	for(i=0;i<320;i++)
	{
		s[i]=rand()%32;
	}

	for(total_pf=4;total_pf<=32;total_pf++)		/*֡����4��32*/
	{
		initial(total_pf);		/*��ʼ��������ݽṹ*/
		fifo(total_pf);			/*����FIFO��������*/
	}
}

void initial(int pf)
{
	int i;

	for(i=0;i<32;i++)			/*��ʼ��ҳ������*/
	{
		pl[i].pn=i;
		pl[i].pfn=-1;
	}

	for(i=0;i<pf-1;i++)			/*��ʼ��֡����������֡�������֡���У���ռ��֡����Ϊ��*/
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
	int i,diseffect=0;			/*diseffect������¼ҳ��ʧЧ����*/
	struct pfc_struct *p;

	for(i=0;i<320;i++)				/*����ҳ��������У�����ʧЧ��*/
	{
		if(pl[s[i]].pfn==-1)		/*������ҳ�治���ڴ��У�ʧЧ����1����������ڴ�*/
		{
			diseffect+=1;			

			if(freepf_head==NULL)		/*�޿���֡����ӱ�ռ֡�����а�FIFO�ͷŵ�һ����ռ֡������������֡����*/
			{
				p=busypf_head->next;
				pl[busypf_head->pn].pfn=-1;
				freepf_head=busypf_head;
				freepf_head->next=NULL;
				busypf_head=p;
			}

			p=freepf_head->next;		/*Ϊ������ҳ�����һ֡��������֡�ҵ���ռ֡���ж�β*/
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