# EAT-FPD-Algorithms
Efficient Algorithms for Earliest and Fastest Paths in Public Transport Networks

This repository provides efficient near-linear time algorithms for solving the Earliest Arrival Time (EAT) and Fastest Path Duration (FPD) problems in temporal graphs, with a focus on real-world public transportation networks.

✨ Overview

We propose four novel algorithms:

EAT-T

EAT-TL

FPD-T

FPD-TL

Technical Specifications: The experimentation is conducted on a machine equipped with an INTEL XEON E5-2620 v4 CPU operating at a frequency of 2.20 GHz and featuring 32 GB of primary memory and 512 MB cache memory. The compiler used is gcc version 5.4.0.


Datasets Preprocessing

For our implementations, we present the constraints and input format for the input graph:

Constraints:

    The graph is directed, and the vertices are labelled from 0 to n-1, where n is the total number of vertices.

Temporal Graph Input Format:
****************************************************************************************************************************************************
The first line consists of two positive values separated by a space: n and m. Here, n represents the number of vertices in the Temporal Graph G, and m represents the number of connections in the Temporal Graph G.

The following m lines consist of four positive integers separated by spaces: u, v, t, and lambda. Here, u and v denote the end vertices of a connection, t is the starting time at u, and lambda represents the duration required to reach v from u.

Note: These connections are sorted in increasing order of departure time t-value
****************************************************************************************************************************************************
****************************************************************************************************************************************************
``plaintext
6 11
0 1 0 1
0 1 2 1
1 2 1 2
3 2 5 2
1 2 3 3
3 2 2 3
1 3 4 4
1 3 6 4
3 2 10 5
3 4 10 10
2 5 20 25
``plaintext
****************************************************************************************************************************************************
   
ESDG Input Format:

In the Preprocessing directory, we provide the scripts to convert the datasets and the instructions to run the scripts. After preprocessing, we will get the data in the following format. It will represent the Edge-Scan-Dependency Graph G'
****************************************************************************************************************************************************
****************************************************************************************************************************************************
The first line consists of three positive values separated by a space: m1, m2, and n. Here, n represents the number of vertices in the Temporal Graph G, m1 represents the number of connections in the Temporal Graph G, and m2 represents the number of dependencies in the ESDG graph G'.

The second line consists of m1 space-separated values, indicating the offset array representing the ESDG graph G'.  
The second line consists of m2 space-separated values that indicate the neighbours of the nodes in the ESDG graph G'.
****************************************************************************************************************************************************
****************************************************************************************************************************************************
11 13 6
0 2 4 6 8 9 10 10 10 11 12 
2 4 2 5 7 10 7 10 6 6 6 6 6 
0 1 
2 3 4 5 
6 
7 8 9 10 
-1
-1
****************************************************************************************************************************************************

##Query format
We have generated $100$ queries and used the same $100$ queries to run baseline and proposed algorithms on fourteen public transportation data sets, measuring the average query running time in milliseconds. In \textsc{eat} experiment, each query consists of two values: a source vertex and a ready time and in \textsc{fpd} experiment, each query has a source vertex. The source vertices are randomly selected from $0$ to $n-1$, where $n$ denotes the number of vertices in the underlying graph, and the corresponding ready times are chosen randomly within the range of $0$ to $100$.

Contact
If you have any questions, contact us by sending an email to srikanth.mithinti@gmail.com or rama@iittp.ac.in
