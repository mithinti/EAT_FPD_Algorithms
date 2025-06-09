#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<iostream>
#include<string.h>
#include<fstream>
#include<math.h>
#include<limits.h>
#include<queue>
#include<vector>
#include<bits/stdc++.h>

#define INF INT_MAX
#define MAX_THREADS_PER_BLOCK 1024
#define PRINTFLAG true

#define CEILING_POS(X) ((X-(int)(X)) > 0 ? (int)(X+1) : (int)(X))
#define CEILING_NEG(X) ((X-(int)(X)) < 0 ? (int)(X-1) : (int)(X))
#define CEILING(X) ( ((X) > 0) ? CEILING_POS(X) : CEILING_NEG(X) )


using namespace std;

typedef struct conn
{
    int u,v,t,lambda;
    bool operator()(conn a, conn b)
        {
                return (a.t+a.lambda < b.t+b.lambda);
        }
    

}conn;

typedef struct mytuple
{
    int srcTime, arrTime;
    bool operator()(mytuple a, mytuple b)
        {
                return (a.arrTime < b.arrTime);
        }
    

}mytuple;


void readQuery(char *fileName, int *&source, int &qCount)
{
	ifstream fin1;

	fin1.open(fileName);	

	fin1>>qCount;

	source = new int[qCount];

	int temp;
	for(int i=0; i<=qCount-1; i++)
		fin1>>source[i]; 
}


void initFastestPath(int *&fastestPath, int vCount, int source)
{
	fastestPath = new int[vCount];
	for(int i=0; i<=vCount-1; i++)
	{
		fastestPath[i] = INF;
	}

	fastestPath[source]=0;

}

void printFastestTimes(int fastestPath[], int vCount,	double runningTime, bool print)
{
      	if(print == true)
	    {
		    for(int i=0;i<=vCount-1;i++)
		    {
			    cout<<i<<" "<<fastestPath[i]<<endl;   
    		}
    	}   
        // cout<<"Time is "<<runningTime<<endl ;
        //cout<<runningTime<<endl ; 
}


void computeFastestPath(int src, vector<set<mytuple,mytuple> > setvec, vector<conn> vec,int vCount,int eCount,int *fastestPath)
{
    for(int i=0;i<eCount; i++)
    {
        int u,v,t,lambda;
        u = vec[i].u;
        v = vec[i].v;
        t = vec[i].t;
        lambda = vec[i].lambda;
        
        mytuple temp;

        if(u==src)
        {
            temp.srcTime = t;
            temp.arrTime = t; 
            auto it = setvec[src].find(temp);
            if(it == setvec[src].end())
            {
                setvec[src].insert(temp);
            }
        }   
        
        temp.srcTime = -1;
        temp.arrTime = t;
        auto itLow = setvec[u].lower_bound(temp);
        
        int nxtSrc = -1;
        int nxtArrTime = t+lambda;
        if(itLow != setvec[u].end())
        {
            if(itLow->arrTime == t)
            {
                nxtSrc = itLow->srcTime;
            }
            
            else if(itLow != setvec[u].begin())
            {
                itLow --;
                nxtSrc = itLow->srcTime;
            }
            
            else
            {
                continue;
            
            }
        }
        else
        {
            if(setvec[u].size() == 0)
            {
               continue;
            }
            else
            {
                itLow --;
                nxtSrc = itLow->srcTime;
            
            }     
              
        }
        
        mytuple nxttuple;
        nxttuple.srcTime = nxtSrc;
        nxttuple.arrTime = nxtArrTime;
        if(setvec[v].size() == 0)
        {
            setvec[v].insert(nxttuple);
        }
        else
        {
            auto it = setvec[v].end();
            it--;
            
            if(it->arrTime < nxtArrTime)
            {
                if(it->srcTime < nxtSrc)
                {
                    setvec[v].insert(nxttuple);
                }
                else 
                {
                    continue;
                }
            }
            else
            {
                if(it->srcTime < nxtSrc)
                {
                    setvec[v].erase(it);
                    setvec[v].insert(nxttuple);
                    
                }
                else
                {
                    continue;
                }
                
            }
                   
        }
        
        if(nxtArrTime - nxtSrc < fastestPath[v])
        {
            fastestPath[v] = nxtArrTime - nxtSrc;
        
        }
           
    }


}

int main(int argc , char *argv[])
{
    int vCount, eCount,qCount;
	int *fastestPath,  *source;
	char queryFile[100];
	char fileName[100];
	// struct timeval start,stop;
 //    double time;
    double total_time=0.0,q_time=0.0;
    clock_t start,end;
    
       
    strcpy(fileName, argv[1]);
      
	ifstream fin;
	fin.open(fileName);
    int temp1,temp2;
	fin>>vCount>>eCount;
	vector <conn> vec;
	for(int j=0;j<eCount;j++)
	{
	    int u,v,t,lambda;
	    fin>>u>>v>>t>>lambda;
	    conn curr;
	    curr.u = u;
	    curr.v = v;
	    curr.t = t;
	    curr.lambda = lambda;
	    vec.push_back(curr);
	
	}

    // for (auto element : vec) {
    //     cout << element.u << " "<<element.v<<" "<<element.t<<" "<<element.lambda<<endl;
    // }

	sort(vec.begin(),vec.end(),conn());

    // cout<<"After Sorting:"<<endl;
    // for (auto element : vec) {
    //     cout << element.u << " "<<element.v<<" "<<element.t<<" "<<element.lambda<<endl;
    // }
	
	strcpy(queryFile,argv[2]);
	readQuery(queryFile, source, qCount);
	// qCount=1;
    for(int i=0; i<qCount; i++)
	{
	    vector<set<mytuple,mytuple> > setvec(vCount);
		int src = source[i];
		initFastestPath( fastestPath, vCount, src);
        start=clock();
		// gettimeofday(&start,0);
		computeFastestPath(src,setvec, vec,vCount,eCount,fastestPath);
        end=clock();
        q_time=double(end-start)/CLOCKS_PER_SEC;
		// gettimeofday(&stop,0);
		// time = (1000000.0*(stop.tv_sec-start.tv_sec) + stop.tv_usec-start.tv_usec)/1000.0;
        

        total_time+=q_time;
        printFastestTimes(fastestPath,vCount,q_time,PRINTFLAG);
	
	} 

    cout<<" Run Time:"<<(total_time/qCount)*1000<<" ms"<<endl; 
	
	return 0;

}
