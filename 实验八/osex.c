#include <stdio.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;
 
struct pl_type
{
	int pn, pfn, time;
};
 
struct pfc_struct
{
	int pn, pfn;
	struct pfc_struct* next;
};
 
int s[320];					/*页面访问序列*/
struct pl_type pl[32];		/*页面数组*/
struct pfc_struct pfc[32];	/*用来构造帧队列*/
struct pfc_struct* freepf_head;
 
void initial(int pf);
void lru(int pf);
 
int main()
{
	int i, total_pf;
 
	srand(getpid());			/*构造页面访问序列*/
	for (i = 0; i < 320; i++)
	{
		s[i] = rand() % 32;
	}
 
	for (total_pf = 4; total_pf <= 32; total_pf++)		/*帧数从4到32*/
	{
		initial(total_pf);
		lru(total_pf);
	}
}
 
void initial(int pf)
{
	int i;
 
	for (i = 0; i < 32; i++)			/*初始化页面数组*/
	{
		pl[i].pn = i;
		pl[i].pfn = -1;
		pl[i].time = 0;
	}
 
	for (i = 0; i < pf - 1; i++)			/*初始化帧，并将所有帧放入空闲帧队列，被占用帧队列为空*/
	{
		pfc[i].pfn = i;
		pfc[i].next = &pfc[i + 1];
	}
	pfc[pf - 1].pfn = pf - 1;
	pfc[pf - 1].next = NULL;
 
	freepf_head = &pfc[0];
}
 
void lru(int pf)
{
	int i, j, diseffect = 0;			/*diseffect用来记录页面失效次数*/
	int mintime, minj, present_time;	/*present_time用于计时*/
 
	for (i = 0; i < 320; i++)				/*处理页面访问序列，计算失效数*/
	{
		if (pl[s[i]].pfn == -1)		/*被访问页面不在内存中，失效数加1并将其调入内存*/
		{
			diseffect += 1;
 
			if (freepf_head == NULL)		/*无空闲帧，则找出距现在最久的被访问页，并将淘汰，将其占用的帧加入空闲帧队列*/
			{
				mintime = 32767;
				for (j = 0; j < 32; j++)
				{
					if (mintime > pl[j].time && pl[j].pfn != -1)
					{
						mintime = pl[j].time;
						minj = j;
					}
				}
				freepf_head = &pfc[pl[minj].pfn];
				pl[minj].pfn = -1;
				pl[minj].time = -1;
				freepf_head->next = NULL;
			}
			pl[s[i]].pfn = freepf_head->pfn;			/*为待调入页面分配一帧，并记录下访问时间*/
			pl[s[i]].time = present_time;
			freepf_head = freepf_head->next;
		}
 
		else
			pl[s[i]].time = present_time;			/*若被访页面在内存中则更新访问时间*/
		present_time++;							/*每处理页面访问序列中的一项，计时器加1*/
	}
	printf("%d frames %f\n", pf, 1 - (float)diseffect / 320);
}