#include <iostream>
#include<queue>
#include<utility>
#include<cassert>
#include<fstream>
#include<limits.h>
#include<ctime>
using namespace std;

typedef struct connections
{
	int *u,*v,*st,*lambda;
	int n,m;
}connections;


typedef struct query
{
	int *sv,*t;
	int n;
}query;


void read_input(char * ip_conn, char *ip_qfile, connections &G, query &Q)
{
    
    ifstream ipFile(ip_conn);
    ifstream ipFile3(ip_qfile);

    ipFile>>G.n>>G.m;
    

    G.u=new int[G.m];
    G.v=new int[G.m];
    G.st=new int[G.m];
    G.lambda=new int[G.m];

    
    // int temp;
    for(int i=0;i<G.m;i++)
    {
        ipFile>>G.u[i]>>G.v[i]>>G.st[i]>>G.lambda[i];
    }
    
    ipFile.close();

    ipFile3>>Q.n;

    Q.sv=new int[Q.n];
    Q.t=new int[Q.n];

    for(int i=0;i<Q.n;i++)
    {
    	ipFile3>>Q.sv[i]>>Q.t[i];
    }

    ipFile3.close();
}


bool relax(connections &G, int *eat, int i)
{
	int et= G.st[i]+ G.lambda[i];
	if(eat[G.u[i]]<=G.st[i] && eat[G.v[i]]>et)
	{
		eat[G.v[i]]=et;
		return true;
	}

	return false;
}

void earliestArrivalTime(connections &G,int s,int ts, int *& eat)
{
	
	for(int i=0;i<G.n;i++)
	{
		eat[i]=INT_MAX;
	}
	eat[s]=ts;

	for(int i=0;i<G.m;i++)
	{
		relax(G,eat,i);
	}
}

void print(ofstream &oFile, int *A,int n,int s, int t)
{
	for(int i=0;i<n;i++)
		oFile<<i<<" "<<A[i]<<endl;
		// oFile<<"EAT["<<i<<"] from source "<<s<<" at departure time "<<t<<" is: "<<A[i]<<endl;
}

int main(int argc, char*argv[])
{
	char *ip_connections, *ip_qfile, *op_eatfile;
	connections G;
	query Q;
	int s,ts,n_quries;


    clock_t start,end;
    double total_time=0.0, q_time=0.0;

    assert(argc==4);
    ip_connections=argv[1];
    ip_qfile=argv[2];
    op_eatfile=argv[3];

	read_input(ip_connections,ip_qfile, G,Q);

	int *eat=new int[G.n];

	ofstream oFile;
	oFile.open(op_eatfile);


	for(int i=0;i<Q.n;i++)
	{
		start=clock();
		earliestArrivalTime(G,Q.sv[i],Q.t[i],eat);
		end =clock();
		q_time= double(end-start) / CLOCKS_PER_SEC;

		print(oFile,eat,G.n,Q.sv[i],Q.t[i]);

		total_time+=q_time;
	}

	// cout<<" Run Time:"<<total_time<<endl;
	cout<<op_eatfile<<" Run Time:"<<(total_time/Q.n)*1000<<" ms"<<endl; 

	oFile.close();


return 0;
}


