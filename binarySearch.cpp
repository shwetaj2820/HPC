#include<iostream>
#include<stdlib.h>
#include<omp.h>
using namespace std;

int binarySearch(int *, int, int, int);

int binarySearch(int *arr, int low, int high, int key)
{
	
	int mid;	
	mid=(low+high)/2;
	int low1,low2,high1,high2,mid1,mid2,found=0,loc=-1;

	#pragma omp parallel sections
	{
	    #pragma omp section
    		{ 
			low1=low;
			high1=mid;
			
			while(low1<=high1)
			{

				if(!(key>=arr[low1] && key<=arr[high1]))
				{
					low1=low1+high1;
					continue;
				}
				
								mid1=(low1+high1)/2;
				
				if(key==arr[mid1])
				{
					found=1;
					loc=mid1;
					low1=high1+1;
				}
					
				else if(key>arr[mid1])
				{

					low1=mid1+1;
				}
				
				else if(key<arr[mid1])
					high1=mid1-1;
			
			}
		}
				   			
    

    	    #pragma omp section
    		{ 
      			low2=mid+1;
			high2=high;
			while(low2<=high2)
			{
	
				if(!(key>=arr[low2] && key<=arr[high2]))
				{
					low2=low2+high2;
					continue;
				}
				
				cout<<"here2";
				mid2=(low2+high2)/2;
				
				if(key==arr[mid2])
				{

					found=1;
					loc=mid2;
					low2=high2+1;	
				}									
				else if(key>arr[mid2])
				{

				low2=mid2+1;
				}
				else if(key<arr[mid2])
				high2=mid2-1;

			}	
    		}
	}

	return loc;
}


int main()
{
	int *a,i,n,key,loc=-1;
	cout<<"\n no. of elements?";
	cin>>n;
	a=new int[n];
	
	cout<<"\n input elements";
	for(i=0;i<n;i++)
	{
	  cin>>a[i];
        }
	
	cout<<"\n input key: ";
	cin>>key;
	
	loc=binarySearch(a,0,n-1,key);

	if(loc==-1)
		cout<<"\n search unsuccessful";
	else
		cout<<"\n element located at position "<<loc+1;

	return 0;
}