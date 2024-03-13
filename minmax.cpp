// Implement Parallel Reduction using Min, Max, Sum and Average operations.

#include<iostream>
#include<omp.h>
using namespace std;

int average(int avg,int sum)
{

 avg=avg+sum;
 return avg;
}

int main()
{

int s=0,k,Max=4000;
#pragma omp parallel reduction(+:sum)
for(k=0;k<Max;k++)
{
  
  s+=k;

}
cout<<"Sum = "<<s<<endl;

    double array_[10];
    double maxNum=0.0;
    for( k=0; k<10; k++)
    {
        array_[k] = 2.0 + k;
        cout<<array_[k]<<endl;
    }
    #pragma omp parallel for reduction(max : maxNum)
    for( k=0;k<10; k++)
    {

        if(array_[k] > maxNum)
        {
            maxNum = array_[k];   
        }
    }
   
    cout<<"Maximum value = "<<maxNum<<endl;


    double arr_[10];
    for( k=0; k<10; k++)
    {
        arr_[k] = 2.0 + k;
        cout<<arr_[k]<<endl;
    }
    double minNum=arr_[k-1];
    #pragma omp parallel for reduction(min : minNum)
    for( k=0;k<10; k++)
    {

        if(arr_[k] < minNum)
        {
            minNum = arr_[k];   
        }
    }
   
    cout<<"Minimum value = "<<minNum<<endl;

int AvgNum=0;
#pragma omp declare reduction(avg:int:omp_out=average(omp_out,omp_in))
#pragma omp parallel reduction(avg:avg_val)
for(k=0;k<10;k++)
{
  AvgNum=average(AvgNum,array_[k]);
} 
AvgNum=AvgNum/10;
cout<<"Average value = "<<AvgNum;
return 0;
}
