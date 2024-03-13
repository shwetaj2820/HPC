#include<iostream>
#include<omp.h>
using namespace std;

class BFS{
int a[5][5],var,e1,e2,arr[5],front,back;
public:
 BFS(){ for(int i=0;i<5;i++)
          for(int j=0;j<5;j++)
            a[i][j]=0;

    front=0;
    back=0;
  }
 void input();
 void output();
 void bfs();
};

void BFS::input(){
char ans;
cout<<"\nEnter no. of vertices";
cin>>var;

do{
 cout<<"\nEnter edges";
 cin>>e1>>e2;
 if(e1<=var && e2<=var)
 { 
   a[e1][e2]=1;
   a[e2][e1]=1;
 }
 else
  cout<<"\nIncorrect edges ";
 cout<<"\n continue ( Y OR N )";
 cin>>ans;
 }while(ans=='y'); 

}

void BFS::output(){
  for(int i=1;i<=var;i++){
     cout<<"\n";
    for(int j=1;j<=var;j++)
     cout<<" "<<a[i][j];}
}

void BFS::bfs(){
int v,visit[5]={0};
cout<<"\n Enter the starting vertex";
cin>>v;
arr[back++]=v;
visit[v]=1;
do{
 #pragma omp parallel
 {
 for(int i=1;i<=var ;i++){
  if(a[v][i]==1 && visit[i]==0){
   arr[back++]=i;
   visit[i]=1;
  }
 } 
 }
 v=arr[front++];
}while(back<var);

cout<<"\n Matrix is";
for(int i=0;i<back;i++)
 cout<<" "<<arr[i];  

}

int main(){
BFS b;
b.input();
b.output();
b.bfs();
return 0;
}
