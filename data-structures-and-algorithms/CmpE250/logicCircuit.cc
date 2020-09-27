#include <iostream>
#include<fstream>
#include<cstdio>
#include<string>
#include<sstream>
#define MAXLEN 100 //define maximum nr of elements in the arrays

using namespace std;
/*inputs*/
int n1,n2,n3,n;
int inputs[MAXLEN];
int outputs[MAXLEN];
char gates[MAXLEN];
int valueof[MAXLEN]; //output values
int visited[MAXLEN]; //visited nodes - 1 for visited, 0 otherwise
int pp[MAXLEN][MAXLEN] = {0}; //point-point matrix

void clear_visited_array()
{
  int i;
  for(i=0;i<MAXLEN;i++)
    visited[i]=0;     
}

void read_data(char* s)
{
     ifstream fin(s);
     string line;     
     std::getline(fin,line);
     std::istringstream iss; //(line);
     iss.str(line);
     iss>>n1>>n2>>n3;
     n=n1+n2+n3; //for finding cycles
     int i;
     /*read inputs*/
     std::getline(fin,line);
	 iss.clear();
     iss.str(line);
     for(i=0;i<n1;i++)
     {
       iss>>inputs[i];
       valueof[i]=inputs[i];
     }
     /*read gates*/
     for(i=0;i<n2;i++)
     {
       iss>>gates[i];
     }
     /*read outputs
     outputs are 'O', their values calculated later*/
     
     /*read file line by line*/
     while (getline(fin, line)) {
        iss.clear(); //clear stream
        iss.str(line);
        int x,y;
        iss>>x;
        /*read every value in line, create point-point matrix*/
        while (iss >> y) {
            pp[x][y]=1;
        }
    }
     fin.close();   //close input file 
}

/*dfs algorithm, modified to find cycles*/
int dfs_for_node(int node)
{
   int i;
   for(i=0;i<n;i++)
   {
     if(pp[node][i]==1)
     {
       if(visited[i]==0)
       {
         /*checked node, go deeper*/
         visited[i]=1;
         if(dfs_for_node(i))
         {
           return 1;                   
         }
         /*uncheck node*/
         visited[i]=0;
       }
       else
       {
         return 1;   //there is a cycle in the graph 
       }               
     }      
   }
      return 0;  //no cycle
}

/*return 1 if graph has cycle, 0 otherwise*/
int has_cycle()
{
  int i;
  for(i=0;i<n;i++)
  {
    clear_visited_array();
    visited[i]=1;
        if(dfs_for_node(i)==1)
    {
      return 1;                      
    }
  }
  return 0;
}

int main(int argc, char *argv[]){

    if(argc!=3)
    {
      cout<<"Usage: main.exe input_file_name output_file_name"<<endl;
      return 0;           
    }    
    
    read_data(argv[1]); //read input file
    if(has_cycle())
    {
      ofstream fout(argv[2]);
      fout<<"-1";
      fout.close();   
      return 0;            
    }

    int i,j,x1,x2;

    for(i=0;i<n2;i++)    //find all gate values
    {
      x1=-1;
      x2=-1;
      if(gates[i]=='A') //AND gate
      {
        for(j=0;j<n;j++)
        {
          if(pp[j][n1+i]==1)
          {
            if(x1==-1)
            {
              x1=j;
            }
            else
            {
              x2=j;
            }                  
          }                
        }
        if(x1==1 && x2==1)
          valueof[n1+i]=1;     
        else
          valueof[n1+i]=0;
      }
      else if(gates[i]=='R') //OR gate
      {
        for(j=0;j<n;j++)
        {
          if(pp[j][n1+i]==1)
          {
            if(x1==-1)
            {
              x1=j;
            }
            else
            {
              x2=j;
            }                  
          }                
        }
        if(x1==0 && x2==0)
          valueof[n1+i]=0;     
        else
          valueof[n1+i]=1;
      }    
      else if(gates[i]=='X') //XOR gate
      {
        for(j=0;j<n;j++)
        {
          if(pp[j][n1+i]==1)
          {
            if(x1==-1)
            {
              x1=j;
            }
            else
            {
              x2=j;
            }                  
          }                
        }
        if(x1==x2)
          valueof[n1+i]=0;     
        else
          valueof[n1+i]=1;
      }
      else //NOT gate
      {
        for(j=0;j<n;j++)
        {
          if(pp[j][n1+i]==1)
          {
            x1=j;                  
          }                
        }
        if(x1==1)  
          valueof[n1+i]=0;     
        else
          valueof[n1+i]=1;
      }                 
    }
    
    for(i=0;i<n3;i++) //find all output values
    {
      for(j=0;j<n;j++)
        if(pp[j][n1+n2+i]==1)
        {
          /*output takes the value of the gate connected to it*/
          valueof[n1+n2+i]=valueof[j];                     
        }                
    }
    
    ofstream fout(argv[2]);  //write output
    /*space after all outputs, except last one*/
    n--;
    for(i=0;i<n;i++)
      fout<<valueof[i]<<" ";
    fout<<valueof[n]; //last output without space
    fout.close();   

    return 0;
}
