// multiply vector and a matrix (parallel algorithm)
#include<stdio.h>
#include<iostream>
#include<cstdlib>
#include<omp.h>
using namespace std;

int main()
{
int a=3,b=2;
int arr[a][b],vector_[b],output[a];

//3*2 arrrix
for(int row=0;row<a;row++)
{
	for(int col=0;col<b;col++)
	{
		arr[row][col]=1;
	}
}

cout<<"the input matrix is : "<<endl;
for(int row=0;row<a;row++)
{
	for(int col=0;col<b;col++)
	{
		cout<<"\t"<<arr[row][col];
	}
	cout<<""<<endl;
}


//2*1 vector
for(int row=0;row<b;row++)
	{
		vector_[row]=2;
	}

//display vector
cout<<"The input vector is : "<<endl;
for(int row=0;row<b;row++)
	{
		cout<<vector_[row]<<endl;
	}

#pragma omp parallel
{
#pragma omp parallel for
for(int row=0;row<a;row++)
{
		output[row]=0;
		for(int col=0;col<b;col++)
		{
			output[row]+=arr[row][col]*vector_[col];
			
		}
}
}

cout<<"output column vector"<<endl;

for(int row=0;row<a;row++)
	{
		cout<<"\nvec["<<row<<"]:"<<output[row]<<endl;
	}

return 0;
}