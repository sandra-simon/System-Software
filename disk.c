#include <stdio.h>
#include <stdlib.h>

int req[20],max,n,init,i,total;
void fcfs()
{
	total=abs(init-req[0]);
	for(i=0;i<n-1;i++)
	{
		total+=abs(req[i]-req[i+1]);
	}
	printf("Total seek time (FCFS) : %d\n",total);
}

void scan()
{
	int large=i;
	for(i=0;i<n;i++)
	{
		if(req[i] < req[i+1])
		{
			large=i+1;
		}
	}
	total=init+req[large];
	printf("Total seek time (SCAN) : %d\n",total);
}

void cscan()
{
	int large=9999;
	for(i=0;i<n;i++)
	{
		if((init < req[i] && req[i] < large))
		{
			large=req[i];
		}
	}
	total=init+max+abs(max-large);
	printf("Total seek time (CSCAN) : %d\n",total);
}

void main()
{
	printf("Enter the number of requests : ");
	scanf("%d",&n);
	printf("Enter request sequence : \n");
	for(i=0;i<n;i++)
	{
		scanf("%d",&req[i]);
	}
	printf("Enter maximum cylinder size in the disk : ");
	scanf("%d",&max);
	printf("Enter initial head position : ");
	scanf("%d",&init);
	fcfs();
	scan();
	cscan();
}
