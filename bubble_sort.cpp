//bubble sort (parallel execution)
#include<iostream>
#include<stdlib.h>
#include<omp.h>
using namespace std;

void bubble_sort(int *, int);
void swap(int &, int &);


void bubble_sort(int *arr, int siz)
{
	for(  int i = 0;  i < siz;  i++ )
	 {       
		int element = i % 2;      

		#pragma omp parallel for shared(arr,element)
		for(  int j = element;  j < siz-1;  j += 2  )
		  {       
			if(  arr[ j ]  >  arr[ j+1 ])
			 {       
  				swap(  arr[ j ],  arr[ j+1 ]  );
			 }       
	    	  }       
	 }
}


void swap(int &a, int &b)
{

	int var;
	var=a;
	a=b;
	b=var;

}

int main()
{

	int *arr,siz;
	cout<<"\n no. of elements ?";
	cin>>siz;
	arr=new int[siz];
	cout<<"\n enter elements";
	for(int i=0;i<siz;i++)
	{
		cin>>arr[i];
	}
	
	bubble_sort(arr,siz);
	
	cout<<"\n array (sorted) :\n";
	for(int i=0;i<siz;i++)
	{
		cout<<arr[i]<<endl;
	}


return 0;
}