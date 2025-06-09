// All the connectios are processed decreasing order of departure times
// Queue status flag [if any connection is already exists in queue, we are not going to insert again]
// We will intialize the queue with conncetions, whose starting vertex is src. [ insert connections into the queue in increasing order of departure time]  
// We will perform multiple BFS on ESDG graph on one by one connection, whose starting vertex is src and decreasing order of departure times until all connections are processed
// with out using visited array

//Fastest Path duration version 1_b1 by considering decreasing orderd of departure time with qstatus and starting time with queue
#include<iostream>
#include<cassert>
#include<fstream>
#include<queue>
#include<algorithm>
#include<limits.h>
#include<sys/time.h>
#include<vector>

using namespace std;


typedef struct connection
{
	int u, v, t, lambda, levelNo;
}Graph;


class esdg
{
	int vCount;			// Number of Vertices present in the Original Graph
	int nCount;			// Number of connections in the original Graph or nodes in ESDG Graph
	int dCount;			// Number of dependencies in the original graph or edges in the ESDG Graph
	int lCount;			// Number of levels in the ESDG Graph.

	int *vArray;		// Offset array of ESDG Graph
	int *eArray;		// Neighbouring array of Vertices present in the ESDG Graph

	// Graph *cArray;		// Node to connection mapping

	vector< pair<int,connection> > connSrc;		//connections starting with vertex src

	int *nStartTime;		// Array to store the potential starting time at particular node in the ESDG Graph
	// int *nPStartTime;
	bool *qStatus;
	//Intialization of duration, nStarttime, Queue  : Need to optimize intialization phase
	int *duration;		// Array to store the fastest path duration from source to the respective vertex
	//Functions
public: 
	Graph *cArray;		// Node to connection mapping
	int *src;			// Query array consists of source vertices
	int noOfQueries;
	// int vCount;			// Number of Vertices present in the Original Graph

	void readESDGFromFile(char* esdg, char *qFile);
	void printESDG();
	void getVertexCount(int &n);
	void retriveSrcVertices(int src);
	// bool cmpNodes(int i,int j);
	void sortSrcVertices();
	void initialize_algo();
	// void fastestPathDuration(int src, ofstream &oFile);
	void fastestPathDuration(int src, int &an,int &ad);
	
	void printfpd(ofstream &oFile);
};

void esdg::getVertexCount(int &n)
{
	n=vCount;
}

void esdg::printfpd(ofstream &oFile)
{
	for(int i=0;i<vCount;i++)
		oFile<<i<<" "<<duration[i]<<endl;
}


void esdg::retriveSrcVertices(int src)
{
	int n = nCount;
	connection c;
	vector< pair<int,connection> > srcVertices;
	//cout << "Inside retriveSrcVertices and n = " << n << endl;
	// cout<<src<<endl;
	// cout<<"Retrived connection Id: ";
	for (int i = 0; i <= n - 1; i++)
	{
		// cout<<"i: "<<i<<" "<<cArray[i].u<<" "<<src<<endl;
		if(cArray[i].u == src)
		{
			c.u=cArray[i].u;
			c.v=cArray[i].v;
			c.t=cArray[i].t;
			c.lambda=cArray[i].lambda;
			c.levelNo=cArray[i].levelNo;
			srcVertices.push_back(make_pair(i,c));

			// cout<<i<<" ";
		}
	}

	// cout<<endl;
	connSrc = srcVertices;
}

bool cmpNodes(pair<int, connection> i, pair<int, connection> j)
{
	//i and j are src vertices
	return (i.second.t > j.second.t);
}


void esdg::sortSrcVertices()
{

/*	cout<<"Before Sorting :" <<endl;
	for(auto i=connSrc.begin();i<connSrc.end();i++)
	{
		cout<<" src ID "<<i->first<<" "<<i->second.u<<" "<<i->second.t<<endl;
	}

	cout<<"After sorting: "<<endl;*/
	sort(connSrc.begin(), connSrc.end(), cmpNodes);

	/*for(auto i=connSrc.begin();i<connSrc.end();i++)
	{
		cout<<" src ID "<<i->first<<" "<<i->second.u<<" "<<i->second.t<<endl;
	}

	getchar();*/
}

void esdg::initialize_algo()
{

	duration = new int[nCount];
	nStartTime  = new int[nCount];
	qStatus  = new bool[nCount];

	
	//Intialization============================
	for(int vertex=0; vertex < vCount; ++vertex)
		duration[vertex] = INT_MAX;

	for(int node=0; node < nCount; ++node){
		nStartTime[node] = INT_MIN;
		qStatus[node]=false;
	}
	// Intialization completed =======================
}

void esdg::fastestPathDuration(int src,int &averageActiveNodes,int &averageActiveDependencies)
{
	duration[src] = 0;
	/*
	for(int node=srcArray_offset[src];node<srcArray_offset[src+1];node++)
	{
			nStartTime[srcArray[node]] = cArray[srcArray[node]].t;
			status[srcArray[node]] = true;
	}
	*/
	// int c=0;
	for(auto node=connSrc.begin();node<connSrc.end();node++)
	{
		nStartTime[node->first] = cArray[node->first].t;
		qStatus[node->first]=true;
	}

	int aNodes=0,aDependencies=0;
	for(int node=0; node < nCount; ++node)
    {

    	if(qStatus[node]==true)
		{
			qStatus[node]=false;
			aNodes++;
			if(duration[cArray[node].v]> cArray[node].t+ cArray[node].lambda - nStartTime[node])
			{
				duration[cArray[node].v]= cArray[node].t+ cArray[node].lambda - nStartTime[node];
			}
			// current connection neighbours 
			for(int nodeIndex=vArray[node]; nodeIndex < vArray[node+1]; ++nodeIndex)
			{
				aDependencies++;
				// update node starting as late as possible
				if(nStartTime[eArray[nodeIndex]]<= nStartTime[node])
				{
					nStartTime[eArray[nodeIndex]]= nStartTime[node];
					if(qStatus[eArray[nodeIndex]]==false){
						qStatus[eArray[nodeIndex]]=true;
					}
				}
			}
		}
	}
	averageActiveNodes += aNodes;
	averageActiveDependencies += aDependencies;
}

void esdg::readESDGFromFile(char* esdg, char *qFile)
{
    ifstream fEsdg(esdg);
    // ifstream fGraph(conn);
    ifstream fQuery(qFile);

    int n;
    fEsdg >> vCount;
    fEsdg >> nCount;
    fEsdg >> dCount;
    fEsdg >> lCount;

    vArray = new int[nCount+1];
    eArray = new int[dCount];

    
    

    cArray = new Graph[nCount];

   

    int id;
	for(int index=0; index < nCount; ++index)
		fEsdg >> id >> cArray[index].u >> cArray[index].v >> cArray[index].t >> cArray[index].lambda >> cArray[index].levelNo;

	/*for(int node=0; node < nCount+1; ++node)
		fEsdg >> vArray[node];
	*/
	
	 for(int node=0; node <= nCount; ++node)
		fEsdg >> vArray[node];
	// vArray[nCount]=dCount;
	for(int neighbor=0; neighbor < dCount; ++neighbor)
		fEsdg >> eArray[neighbor];


	fEsdg.close();

	fQuery >> noOfQueries;

	src = new int[noOfQueries];
	for(int index=0; index < noOfQueries; ++index)
		fQuery >> src[index];
}

void esdg:: printESDG()
{
	/*cout << "\nOffset Array";
	for(int node=0; node < nCount+1; ++node)
		cout << vArray[node] << " ";

	cout << "\nNeighbors Array";
	for(int neighbor=0; neighbor < dCount; ++neighbor)
		cout << eArray[neighbor] << " ";

	cout << "\nConnections Array";
	for(int index=0; index < nCount; ++index)
		cout<<"\n"<< index<< " " << cArray[index].u << " "<< cArray[index].v<< " " << cArray[index].t << " " << cArray[index].lambda << " " << cArray[index].levelNo;
		// cout << "\n" << index << " " << G.u[index] << " " << G.v[index] << " " << G.t[index] << " " << G.lambda[index];

	cout << "\nQuery Array";
	for(int index=0; index < noOfQueries; ++index)
		cout << "\n" << src[index];

	cout<< "\nNumber of Vertices: "<< vCount <<"\nNumber of levels in ESDG Grpah:" <<lCount;

	cout << "\n\n\n";

	
	for(int index=0; index < nCount; ++index)
	{
		cout << "\nNeighbors of "<< index<< " :";
		for(int neighbor=vArray[index]; neighbor < vArray[index+1]; ++neighbor)
			cout<<eArray[neighbor]<<" ";
		cout<<endl;
	}*/

	
	cout << "\nConnections Array";
	for(int index=0; index < nCount; ++index)
	{
		if(cArray[index].u > vCount || cArray[index].v > vCount)
		{
			cout<<"\n"<< index<< " " << cArray[index].u << " "<< cArray[index].v<< " " << cArray[index].t << " " << cArray[index].lambda << " " << cArray[index].levelNo;
			// cout << "\n" << index << " " << G.u[index] << " " << G.v[index] << " " << G.t[index] << " " << G.lambda[index];
		}
	}

	cout << "\nQuery Array";
	for(int index=0; index < noOfQueries; ++index)
	{
		if(src[index]> vCount)
			cout << "\n" << src[index];
	}

	cout<< "\nNumber of Vertices: "<< vCount <<"\nNumber of levels in ESDG Grpah:" <<lCount;

	cout << "\n\n\n";

}

int main(int argc, char*argv[])
{
	char *ip_esdg, *ip_qfile, *op_fpdfile;
	esdg G;

	int averageActiveNodes=0,averageActiveDependencies=0;
	
	double total_time=0.0,q_time=0.0;
    clock_t start,end;

	assert(argc==4);
    ip_esdg=argv[1];
    ip_qfile=argv[2];
    op_fpdfile=argv[3];

    // cout<<"File reading started"<<endl;

	G.readESDGFromFile(ip_esdg,ip_qfile);

	// cout<<"Files read successfully"<<endl;
	
	// G.printESDG();
	// G.noOfQueries=1;

	ofstream oFile;
	oFile.open(op_fpdfile);

	// int n;   //number of vertices in temporal graph
	// G.getVertexCount(n);


	

	// cout<<"First query started processing...."<<endl;

	for(int queryIndex=0; queryIndex<G.noOfQueries; ++queryIndex){
		// cout<<"\nQuery Source: "<<G.src[queryIndex]<<endl;
		// G.fastestPathDuration(G.src[queryIndex],oFile);
		// start=clock();
		G.retriveSrcVertices(G.src[queryIndex]);
		G.sortSrcVertices();
		G.initialize_algo();
		start=clock();
		G.fastestPathDuration(G.src[queryIndex],averageActiveNodes,averageActiveDependencies);
		// G.fastestPathDuration(G.src[queryIndex],fpd,nofConnectionsTouched,noofRelax);
		// G.fastestPathDuration(331,fpd,nofConnectionsTouched,noofRelax,noofEmptyQueue);
		end=clock();
		// getchar();
		q_time=double(end-start)/CLOCKS_PER_SEC;
		total_time+=q_time;
		
		G.printfpd(oFile);

		// cout<<"Query Index "<<queryIndex<<" Processed successfully"<<endl;
		/*for(int i=0;i<6;i++)
			cout<<i<<" "<<fpd[i];
		cout<<endl;*/
	}
	cout<<" Run Time:"<<(total_time/G.noOfQueries)*1000<<"ms"<<endl;
	// cout<<"Average Number of connections Touched : "<< nofConnectionsTouched/G.noOfQueries<<endl;
	// cout<<"Average Number of connections processed : "<<(float)noofRelax/G.noOfQueries<<endl;
	cout<<"updated with sorting"<<endl;

	cout<<"averageActiveNodes: "<<(averageActiveNodes/G.noOfQueries)<<endl;
	cout<<"averageActiveDependencies: "<<(averageActiveDependencies/G.noOfQueries)<<endl;
	
	// cout<<G.noOfQueries;
	// cout<<noofRelax;
	// cout<<"Number of Empty queues : "<<noofEmptyQueue/G.noOfQueries<<endl;
	// cout<<"FPD Calculated Sucessfully"<<endl;

	// oFile.close();
	// G.neighbours(i);

	// G.bestStarttime(i);
	// G.getconnection(i);
	return 0;
}
