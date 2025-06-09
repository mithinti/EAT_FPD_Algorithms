/*
DFS 2 For Loops based program to compute earliest arrival time
Nodes in Transformed Graph are processed in decreasing order of departure times - Is it really required?
*/
#include<iostream>
#include<cassert>
#include<fstream>
#include <vector>
#include <algorithm>  //sort
#include <stack>
#include <limits.h> //INT_MAX


using namespace std;


typedef struct OutEvent //u,t outgoing events
{
	int u, t; //vertex id of original graph, departure time
}OutEvent;


class TRG
{
	//preprocess data
	vector<int> vArray, eArray, wArray; //weighted graph
	vector<int> specialNode; //first node in chain
	int vCount, eCount;		//Transformed Graph number of nodes and edges
	int vOrigialCount; // number of vertices in the original graph
	vector<OutEvent> nodes; // for every vertex
	//output data
	vector<int> eatDuration;

public:

	vector<int> sources,rt;

	//intermediate data
	vector<pair<int, OutEvent>> srcVertices;
	vector<bool> visited;     // bool

	void readTRGFromFile(char* in_trg_file);
	void memoryAllocation();
	void printTRG();
	void readQueriesFromFile(char * in_query_file);
	void printQuries();
	void writeFile(ofstream &fout);

	void initialize_algo(int src,int rt);
	void retriveSrcVertices(int src,int rt);
	void sortSrcVertices();
	void printSrcVertices();

	void dfs(int i, double &tot_chain_time);
	void printFPD();

	TRG()
	{

	}
	~ TRG()
	{

	}
};


// void memoryAllocation(struct TRG &trg)
void TRG::memoryAllocation()
{
	
}

// void readFile(ifstream &fin, struct TRG &trg)
void TRG::readTRGFromFile(char* in_trg_file)
{
	ifstream fin(in_trg_file);

	int primaryEdge, chainEdge;

	fin >>vOrigialCount >>vCount >> primaryEdge >> chainEdge;
	eCount = primaryEdge + chainEdge;
	
	// memoryAllocation();
	vArray.resize(vCount + 1); // 0 to n
	eArray.resize(eCount); // 0 to m-1
	wArray.resize(eCount); // 0 to m-1
	nodes.resize(vCount); //0 to n-1

	specialNode.resize(vCount);

	visited.resize(vCount);
	eatDuration.resize(vOrigialCount);

	//read nodes
	int index = 0;
	OutEvent outEvent;
	for (int nodeIndex = 0; nodeIndex < vCount; nodeIndex++)
	{
		fin >> index >> outEvent.u >> outEvent.t;
		if (index != nodeIndex)
		{
			cout << "Error While Reading Nodes. Data File is missing vertex " << nodeIndex << endl;
			break;
		}
		nodes[nodeIndex] = outEvent;
	}

	//read CSR Data
	int offset;
	for (int i = 0; i < vCount + 1; i++)
	{
		fin >> vArray[i];
	}

	for (int i = 0; i < eCount; i++)
	{
		fin >> eArray[i];
	}

	for (int i = 0; i < eCount; i++)
	{
		fin >> wArray[i];
	}


//  Finding special nodes
	int currNeighbour;
	for (int i = 0; i < vCount; i++)
	{
		currNeighbour = -1;
		for (int j = vArray[i] ; j < vArray[i + 1]; j++)
		{
			if (wArray[j] == 0)
			{
				currNeighbour = eArray[j];
				break;
			}
		}
		specialNode[i]=currNeighbour;
	}


	fin.close();

}

// void readQuery(ifstream &query, vector<int> &sources)
void TRG::readQueriesFromFile(char * in_query_file)
{
	
	int n;
	ifstream query(in_query_file);
	query >> n;

	sources.resize(n);
	rt.resize(n);

	int source;
	for (int i = 0; i < n; i++)
	{
		query >> sources[i] >> rt[i];
	}


	query.close();
}


void TRG::printTRG()
{
	cout << "Offset Array" << endl;
	for (int i = 0; i < vArray.size(); i++)
	{
		cout << vArray[i] << " ";
		// printf("%d ",vArray[i] );
	}
	cout << endl;

	cout << "Edge Array" << endl;
	for (int i = 0; i < eArray.size(); i++)
	{
		cout << eArray[i] << " ";
	}
	cout << endl;

	cout << "Weight Array" << endl;
	for (int i = 0; i < wArray.size(); i++)
	{
		cout << wArray[i] << " ";
	}
	cout << endl;

	cout << "All nodes " << endl;
	for (int i = 0; i < nodes.size(); i++)
	{
		cout << i << " " << nodes[i].u << " " << nodes[i].t << endl;
	}
}

void TRG::printQuries()
{
	cout<<"Queries: "<<endl;
	for (unsigned int i = 0; i < sources.size(); i++)
	{
		cout<<sources[i]<<" "<<rt[i]<<endl;
	}
	// cout<<endl;
}

void TRG::writeFile(ofstream &fout)
{
	int i=0;
	for (int duration : eatDuration){
		fout << i<<" "<< duration << endl;i++;
	}
	// fout << "Time Taken = " << time << endl;

}


void TRG::printFPD()
{
	int i=0;
	for (int duration : eatDuration){
		cout << i<<" "<< duration << endl;i++;
	}
}

void TRG::initialize_algo(int src, int rt)
{
	for (int i = 0; i < vOrigialCount; i++)
	{
		eatDuration[i] = INT_MAX;
		// visited[i] = false;
	}
	for (int i = 0; i < vCount; i++)
	{
		// fpDuration[i] = INT_MAX;
		visited[i] = false;
	}
	eatDuration[src] = rt;
}

void TRG::retriveSrcVertices(int src, int rt)
{
	int n = vCount;
	vector<pair<int, OutEvent>> src_Vertices;
	//cout << "Inside retriveSrcVertices and n = " << n << endl;
	for (int i = 0; i <= n - 1; i++)
	{
		if (nodes[i].u == src && nodes[i].t>=rt)
		{
			//cout << "Source Vertex = " << i << " " << trg.nodes[i].u << " " << trg.nodes[i].t << endl;
			src_Vertices.push_back(make_pair(i, nodes[i]));
		}
	}
	srcVertices = src_Vertices;

}

void TRG::printSrcVertices()
{
	for (auto x : srcVertices)
		{
			cout << nodes[x.first].u << ", " << nodes[x.first].t << endl;
		}
}


// Compares two outgoing events according to departure times.
bool cmpVertexEvents(pair<int, OutEvent> i, pair<int, OutEvent> j)
{
	//i and j are src vertices
	return (i.second.t > j.second.t);
}

void TRG::sortSrcVertices()
{
	sort(srcVertices.begin(), srcVertices.end(), cmpVertexEvents);
}


void TRG::dfs(int i, double &tot_chain_time)
{
	int u, t, j, startTime, inTime, index, uPrime;

	stack<int> st;
	double chain_time=0.0,total_chain_time=0.0;
	clock_t start,end;
	visited[i] = true;
	// startTime = nodes[i].t;

	st.push(i);

	while (st.size() >= 1)
	{
		i = st.top(); st.pop();

		u = nodes[i].u;
		t = nodes[i].t;

		// cout << "Processing Node Id i = " << i << " u = " << u << " t = " << t << endl;

		for (index = vArray[i]; index < vArray[i + 1]; index++)
		{
			// cout << " Index = " << index << endl;
			j = eArray[index];
			// cout << " j = " << j <<" "<<nodes[j].u<<" "<<nodes[j].t<<endl;
			inTime = t + wArray[index];

			uPrime = nodes[j].u;

			if (inTime  < eatDuration[uPrime])
			{
				eatDuration[uPrime] = inTime;
			}
			// chain_time=0.0;
			start=clock();
			while (visited[j] == false) // -1 check
			{
				visited[j] = true;
				st.push(j);
				j = specialNode[j];
				if (j == -1) break;
			} //chain nodes
			end=clock();
			chain_time =double(end-start)/CLOCKS_PER_SEC;
			total_chain_time += chain_time;

		}//first for loop

	}//until stack is empty

	// cout<<" chain_time:"<<i<< " "<<total_chain_time<<endl;
	tot_chain_time += total_chain_time; 

}// end of dfs



int main(int argc, char* argv[])
{

	char *ip_trg, *ip_qfile;
	
	double total_time=0.0,q_time=0.0;
	double tot_chain_time=0.0, tot_prime_time=0.0;
    clock_t start,end;

	assert(argc==4);
    ip_trg=argv[1];
    ip_qfile=argv[2];
    

	ofstream fout;
	fout.open(argv[3]);

	TRG T;

	//reading input file->trg   trg.vCount etc..
	T.readTRGFromFile(ip_trg);
	cout << "Read Graph Completed" << endl;
	// T.printTRG();


	//readQueries
	T.readQueriesFromFile(ip_qfile);
	cout << "Read Query Completed" << endl;
	// T.printQuries();

	
	for (unsigned int i = 0; i < T.sources.size(); i++)
	{
		
		int src = T.sources[i];
		int readyTime=T.rt[i];

		start=clock();
		T.initialize_algo(src,readyTime);
		
		T.retriveSrcVertices(src,readyTime); // dont measure time
		// T.printSrcVertices();
		
		T.sortSrcVertices(); //w.r.t departure time
		// cout << "After Sorting srcVertices" << endl;
		// T.printSrcVertices();

		
		//cout << "Outevents in decreasing order by departure time : \n";
		
		for (auto x : T.srcVertices)
		{
			// cout << nodes[x.first].u << ", " << nodes[x.first].t << endl;

			T.dfs(x.first,tot_chain_time);
		}
		
		end=clock();

		q_time=double(end-start)/CLOCKS_PER_SEC;
		total_time+=q_time;

		// cout<<endl;
		// T.printFPD();
		T.writeFile(fout);
	}
	fout << "Average Running Time = " <<(total_time/T.sources.size())*1000<<"ms"<<endl;
	cout<<"Average Run Time:"<<(total_time/T.sources.size())*1000<<"ms"<<endl;
	
	tot_prime_time = total_time - tot_chain_time;
	fout << "Average Running Time for prime vertices = " <<(tot_prime_time/T.sources.size())*1000<<"ms"<<endl;
	cout << "Average Running Time for prime vertices = " <<(tot_prime_time/T.sources.size())*1000<<"ms"<<endl;
	
	fout << "Average Running Time for chain vertices = " <<(tot_chain_time/T.sources.size())*1000<<"ms"<<endl;
	cout << "Average Running Time for chain vertices = " <<(tot_chain_time/T.sources.size())*1000<<"ms"<<endl;
	// fin.close();
	// query.close();
	fout.close();

	return 0;
}