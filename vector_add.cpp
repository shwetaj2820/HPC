//adding 2 large vectors (parallel execution)
#include<stdio.h>
#include<iostream>
#include<cstdlib>
#include<omp.h>	
using namespace std;
#define MAX 10000


int main()
{
int a[MAX],b[MAX],c[MAX],i;
printf("\n vector 1-\t");


#pragma omp parallel for
for(i=0;i<MAX;i++)
	{
		a[i]=rand()%1000;
	}


for(i=0;i<MAX;i++)
	{
		printf("%d\t",a[i]);
	}

printf("\n vector 2-\t");


#pragma omp parallel for
for(i=0;i<MAX;i++)
	{
		b[i]=rand()%1000;
	}

for(i=0;i<MAX;i++)
	{
		printf("%d\t",b[i]);
	}

printf("\n Parallel-Vector Addition\t");



#pragma omp parallel for
for(i=0;i<MAX;i++)
	{
		c[i]=a[i]+b[i];
	}

for(i=0;i<MAX;i++)
	{
		printf("\n%d\t%d\t%d",a[i],b[i],c[i]);
	}
}



