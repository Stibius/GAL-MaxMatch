/* Bipartite graph class header */

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <vector>
#include <climits>
#include <ctime>
#include <cstdlib>

#define INF ULONG_MAX
#define NIL "_NIL"
#define HOPCROFT_KARP 1
#define EDMONDS_KARP 2

typedef unsigned long ulong;
typedef std::string TVertex;

typedef std::queue< TVertex              > TVertexQueue;
typedef std::set  < TVertex              > TVertexList;
typedef std::pair < TVertex, TVertex     > TVertexPair;
typedef std::pair < TVertex, TVertexList > TVertexListPair;
typedef std::map  < TVertex, ulong       > TDistance;
typedef std::map  < TVertex, TVertex     > TVertexMap;
typedef std::map  < TVertex, TVertexList > TAdjList;
typedef std::set  < TVertexPair          > TMatching;
typedef std::map  < TVertexPair, ulong   > TEdgeValueMap;
typedef std::pair < TVertexPair, ulong   > TEdgeValuePair;
typedef std::set  < TVertex              >::iterator TVertexListIter;
typedef std::map  < TVertex, ulong       >::iterator TDistanceIter;
typedef std::map  < TVertex, TVertex     >::iterator TVertexMapIter;
typedef std::map  < TVertex, TVertexList >::iterator TAdjListIter;
typedef std::set  < TVertexPair          >::iterator TMatchingIter;

/* Structure for a bipartite graph */
struct SGraph
{
    TAdjList adj;      /* Adjacency list */
    TVertexList boys;  /* First set of vertices */
    TVertexList girls; /* Second set of vertices */
};

/* Structure for a network created from a bipartite graph */
class CNetwork
{
private:
    TAdjList adj;            /* Adjacency list */
    TVertexList boys;        /* First set of vertices */
    TVertexList girls;       /* Second set of vertices */
    TVertex source;          /* Source vertex of the network */
    TVertex sink;            /* Sink vertex of the network */
	TEdgeValueMap flow;      /* Flow table */
	TEdgeValueMap cap;       /* Capacity table */
	TEdgeValueMap resCap;    /* Residual capacity table */
	TMatching match;         /* Result matching */

	bool BFS(); /* Searches for a way from source to sink in the residual network. If found, returns true and updates flow and resCap. */
	TMatching getMatching(); /* Returns the edges from the bipartite part of this network that have non-zero flow. */
 
public:

	CNetwork(const SGraph & graph);  /* Constructs a network from a bipartite graph */
	TMatching edmondsKarp();         /* Runs the Edmonds-Karp algorithm on this network and returns the resulting maximum matching. */
	
};

/* Class for a bipartite graph */
class CGraph
{
private:
    SGraph graph;/* Structure for a bipartite graph */
    TVertexMap pairBoys;  /* Matching pairs of boys-girls */
    TVertexMap pairGirls; /* Matching pairs of girls-boys */
    TDistance dist;       /* Distance of vertices in an augmented path */

public:
    CGraph();                                     /* Class constructor */
    virtual ~CGraph();                            /* Class destructor */
    void parseGraph(std::string s = "");          /* Loads a graph from a formatted string */
    std::string printGraph();                     /* Prints a graph to a formatted string */
    std::string printMaximumMatching(int method); /* Calculates and prints a maximum matching */
    void generateBipartiteGraph(                  /* Generates bipartite graph */
      ulong boys, ulong girls, ulong edges
    );

private:
    TMatching hopcroftKarp(); /* Calculates maximum matching using Hopcroft-Karp algorithm */
    TMatching edmondsKarp();  /* Calculates maximum matching using Edmonds-Karp algorithm */
    bool BFS();                   /* Auxilary method */
    bool DFS(TVertex vertex);     /* Auxilary method */
};
