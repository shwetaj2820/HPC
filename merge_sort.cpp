//merge sort (parallel execution)
#include<iostream>
#include<stdlib.h>
#include<omp.h>
using namespace std;

void mergeSort(int arr[],int i,int j);
void mergeFun(int arr[],int i1,int j1,int i2,int j2);

void mergeSort(int arr[],int i,int j)
{
    int mid;
    if(i<j)
    {
        mid=(i+j)/2;
        
        #pragma omp parallel sections 
        {

            #pragma omp section
            {
                mergeSort(arr,i,mid);        
            }

            #pragma omp section
            {
                mergeSort(arr,mid+1,j);    
            }
        }

        mergeFun(arr,i,mid,mid+1,j);    
    }

}
 
void mergeFun(int arr[],int i1,int j1,int i2,int j2)
{
    int temp[1000];    
    int i,j,k;
    i=i1;    
    j=i2;    
    k=0;
    
    while(i<=j1 && j<=j2)    
    {
        if(arr[i]<arr[j])
        {
            temp[k++]=arr[i++];
        }
        else
        {
            temp[k++]=arr[j++];
	}    
    }
    
    while(i<=j1)    
    {
        temp[k++]=arr[i++];
    }
        
    while(j<=j2)    
    {
        temp[k++]=arr[j++];
    }
        
    for(i=i1,j=0;i<=j2;i++,j++)
    {
        arr[i]=temp[j];
    }    
}


int main()
{
    int *arr,siz,i;
    cout<<"no. of elements? \n";
    cin>>siz;
    arr= new int[siz];

    cout<<"enter elements:\n";
    for(i=0;i<siz;i++)
    {
        cin>>arr[i];
    }
        
    mergeSort(arr, 0, siz-1);

    cout<<"\nsorted array: ";
    for(i=0;i<siz;i++)
    {
        cout<<"\n"<<arr[i];
    }
       
    return 0;
}

