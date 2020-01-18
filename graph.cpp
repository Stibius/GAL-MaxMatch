/* Bipartite graph class source */

#include "graph.h"

CNetwork::CNetwork(const SGraph & graph)
{
    /* Copy the original bipartite graph first. */
    boys = graph.boys;
    girls = graph.girls;
    adj = graph.adj;

	/* Add the source and sink vertices. */
	source = "S";
	sink = "T";

	/* Set initial capacity, flow and residual capacity for all the edges from the original bipartite graph. */
	for (TVertexListIter it1 = boys.begin(); it1 != boys.end(); it1++)
	{
		for (TVertexListIter it2 = adj[*it1].begin(); it2 != adj[*it1].end(); it2++)
		{
			/* capacity is 1 in the direction from the boys to the girls */
			cap[TVertexPair(*it1, *it2)] = 1; 
			cap[TVertexPair(*it2, *it1)] = 0;
			flow[TVertexPair(*it1, *it2)] = 0;
			flow[TVertexPair(*it2, *it1)] = 0;
			resCap[TVertexPair(*it1, *it2)] = cap[TVertexPair(*it1, *it2)] - flow[TVertexPair(*it1, *it2)];
			resCap[TVertexPair(*it2, *it1)] = cap[TVertexPair(*it2, *it1)] - flow[TVertexPair(*it2, *it1)];
		}
	}

	/* Add edges between the source vertex and all the boys and set initial capacity, flow and residual capacity for them */
	adj[source] = boys;
	for (TVertexListIter it = boys.begin(); it != boys.end(); it++)
	{
		/* capacity is 1 in the direction from the source to the boys */
		adj[*it].insert(source);
		cap[TVertexPair(*it, source)] = 0;
		cap[TVertexPair(source, *it)] = 1; 
		flow[TVertexPair(*it, source)] = 0;
		flow[TVertexPair(source, *it)] = 0;
		resCap[TVertexPair(*it, source)] = cap[TVertexPair(*it, source)] - flow[TVertexPair(*it, source)];
		resCap[TVertexPair(source, *it)] = cap[TVertexPair(source, *it)] - flow[TVertexPair(source, *it)];
	}

	/* Add edges between the sink vertex and all the girls and set initial capacity, flow and residual capacity for them */
	adj[sink] = girls;
    for (TVertexListIter it = girls.begin(); it != girls.end(); it++)
    {
		/* capacity is 1 in the direction from the girls to the sink */
		adj[*it].insert(sink);
		cap[TVertexPair(*it, sink)] = 1; 
		cap[TVertexPair(sink, *it)] = 0;
		flow[TVertexPair(*it, sink)] = 0;
		flow[TVertexPair(sink, *it)] = 0;
		resCap[TVertexPair(*it, sink)] = cap[TVertexPair(*it, sink)] - flow[TVertexPair(*it, sink)];
		resCap[TVertexPair(sink, *it)] = cap[TVertexPair(sink, *it)] - flow[TVertexPair(sink, *it)];
    }
}

bool CNetwork::BFS()
{
	/* predecessors */
	TVertexMap preds;
	/* set predecessors of all vertices to NIL */
	preds[source] = NIL;
	preds[sink] = NIL;
	for (TVertexListIter it = boys.begin(); it != boys.end(); it++)
	{
		preds[*it] = NIL;
	}
	for (TVertexListIter it = girls.begin(); it != girls.end(); it++)
	{
		preds[*it] = NIL;
	}

	TVertexQueue q;   /* the queue of vertices */
	q.push(source);   /* push the source vertex into the queue */

	/* While the queue is not empty. */
	while (!q.empty())
	{
		bool found = false; /* Indicates if we have found the sink vertex. */
							/* Pop a vertex from the queue. */
		TVertex vertex = q.front();
		q.pop();
		/* For all the vertices adjacent to the popped vertex. */
		for (TVertexListIter it = adj[vertex].begin(); it != adj[vertex].end(); it++)
		{
			/* If the residual capacity from the popped vertex to the adjacent vertex is not 0 and the adjacent vertex wasn't visited yet */
			if (resCap[TVertexPair(vertex, *it)] != 0 && preds[*it] == NIL && *it != source)
			{
				preds[*it] = vertex; /* set the popped vertex as the predecessor of the adjacent vertex */
				if (*it == sink)     /* if the adjacent vertex is the sink, end the search */
				{
					found = true;
					break;
				}
				q.push(*it); /* push the adjacent vertex into the queue */
			}
		}

		if (found) break; /* We have found the sink vertex, we end the search. */
	}

	/* if the sink wasn't found, return false */
	if (preds[sink] == NIL)
	{
		return false;
	}

	ulong smallest;         /* the smallest residual capacity on the path from source to sink */
	TVertex current = sink; /* current vertex on the backwards path from sink to source */

	/* while the current vertex isn't the source */
	while (current != source)
	{
		TVertex pred = preds[current]; /* get the predecessor of the current vertex */
		ulong value = resCap[TVertexPair(pred, current)]; /* get the residual capacity from predecessor to current vertex */
		/* if it is the first residual capacity examined or its value is smaller than smallest, update smallest */
		if (current == sink || value < smallest)
		{
			smallest = value;
		}

		current = pred; /* move to the predecessor */
	}

	current = sink; /* we go through the same backwards path again to update the flow */

	/* while the current vertex isn't the source */
	while (current != source)
	{
		TVertex pred = preds[current]; /* get the predecessor of the current vertex */
		/* update the flow in both directions between the predecessor and current vertex */
		flow[TVertexPair(pred, current)] += smallest;
		flow[TVertexPair(current, pred)] = -flow[TVertexPair(pred, current)];
		/* update the residual capacity in both directions between the predecessor and current vertex */
		resCap[TVertexPair(pred, current)] = cap[TVertexPair(pred, current)] - flow[TVertexPair(pred, current)];
		resCap[TVertexPair(current, pred)] = cap[TVertexPair(current, pred)] - flow[TVertexPair(current, pred)];
		current = pred; /* move to the predecessor */
	}

	return true; /* we have found a path between source and sink in the residual network */
}

TMatching CNetwork::edmondsKarp()
{
	while (BFS()); /* perform the BFS until no path from source to sink can be found in the residual network */

	return getMatching(); /* return the resulting maximum matching */
}

TMatching CNetwork::getMatching()
{
	TMatching result;

	/* for all the boys */
	for (TVertexListIter it1 = boys.begin(); it1 != boys.end(); it1++)
	{
		/* and all the vertices adjacent to them */
		for (TVertexListIter it2 = adj[*it1].begin(); it2 != adj[*it1].end(); it2++)
		{
			/* if the adjacent vertex isn't source (and therefore is a girl) and the edge between them has non-zero flow */
			if (*it2 != source && flow[TVertexPair(*it1, *it2)] != 0)
			{
				/* Add the edge in both directions to the matching. */
				result.insert(TVertexPair(*it1, *it2));
				result.insert(TVertexPair(*it2, *it1));
			}
		}
	}

	return result;
}

CGraph::CGraph()
{

}

CGraph::~CGraph()
{

}

void CGraph::parseGraph(std::string s)
{
    std::string foundLine;
    std::stringstream lines(s);

    /* Reset from any previous runs */
    graph.adj.clear();
    graph.boys.clear();
    graph.girls.clear();

    /* First row contains vertices from first set */
    if (std::getline(lines, foundLine)) {
        std::stringstream line(foundLine);
        TVertex foundVertex;

        while (std::getline(line, foundVertex, ';')) {
            graph.boys.insert(foundVertex);
        }
    }

    /* Second row contains vertices from second set */
    if (std::getline(lines, foundLine)) {
        std::stringstream line(foundLine);
        TVertex foundVertex;

        while (std::getline(line, foundVertex, ';')) {
            graph.girls.insert(foundVertex);
        }
    }

    /* Rest of rows represent adjacency list */
    while (std::getline(lines, foundLine)) {
        std::stringstream line(foundLine);
        TVertex key;

        /* First vertex v */
        if (std::getline(line, key, ';')) {
            TVertexList list;
            TVertex foundVertex;

            /* Adjacency list adj[v] */
            while (std::getline(line, foundVertex, ';')) {
                list.insert(foundVertex);
            }

            graph.adj[key] = list;
        }
    }
}

std::string CGraph::printGraph()
{
    std::string s = "";
    bool next = false;

    /* First set of vertices */
    for (TVertexListIter it = graph.boys.begin(); it != graph.boys.end(); it++) {
        if (next) {
            s += ";" + *it;
        } else {
            s += *it;
            next = true;
        }
    }

    if (next) {
        s += "\n";
        next = false;
    }

    /* Second set of vertices */
    for (TVertexListIter it = graph.girls.begin(); it != graph.girls.end(); it++) {
        if (next) {
            s += ";" + *it;
        } else {
            s += *it;
            next = true;
        }
    }

    if (next) {
        s += "\n";
    }

    /* Adjacency list */
    for (TAdjListIter it = graph.adj.begin(); it != graph.adj.end(); it++) {
        s += it->first;

        for (TVertexListIter it2 = it->second.begin(); it2 != it->second.end(); it2++) {
            s += ";" + *it2;
        }

        s += "\n";
    }

    return s;
}

std::string CGraph::printMaximumMatching(int method)
{
    std::string s = "";
    TMatching max;
    if (method == HOPCROFT_KARP)
    {
        max = hopcroftKarp();
    }
    else if (method == EDMONDS_KARP)
    {
        max = edmondsKarp();
    }

    /* Edges in maximum matching */
    for (TMatchingIter it = max.begin(); it != max.end(); it++) {
        s += it->first + ";" + it->second + "\n";
    }

    return s;
}

void CGraph::generateBipartiteGraph(ulong boys, ulong girls, ulong edges)
{
    if (boys <= 0 || girls <= 0 || edges <= 0) { /* Incorrect values */
        return;
    }

    if (edges > boys * girls) { /* Edges form a complete graph */
        edges = boys * girls;
    }

    /* Clean initialization */
    graph.adj.clear();
    graph.boys.clear();
    graph.girls.clear();

    for (ulong i = 0; i < boys; i++) { /* Generate boys */
        std::stringstream ss;
        ss << i + 1;
        graph.boys.insert("B" + ss.str());
    }

    std::vector<std::pair<TVertex, TVertex> > allEdges;

    for (ulong i = 0; i < girls; i++) { /* Generate girls */
        std::stringstream ss;
        ss << i + 1;
        graph.girls.insert("G" + ss.str());

        /* Generate all edges */
        for (TVertexListIter it = graph.boys.begin(); it != graph.boys.end(); it++) {
            allEdges.push_back(std::pair<TVertex, TVertex>(*it, "G" + ss.str()));
        }
    }

    /* Generate random edges from all edges */
    for (ulong i = 0; i < edges; i++) {
        ulong edge = rand() % allEdges.size();
        graph.adj[allEdges[edge].first].insert(allEdges[edge].second);
        graph.adj[allEdges[edge].second].insert(allEdges[edge].first);
        allEdges.erase(allEdges.begin() + edge);
    }
}

bool CGraph::BFS()
{
    TVertexQueue queue;

    /* For each vertex in first set */
    for (TVertexListIter it = graph.boys.begin(); it != graph.boys.end(); it++) {
        if (pairBoys[*it] == NIL) { /* If the vertex has no matching yet */
            dist[*it] = 0; /* Set its distance from start to zero */
            queue.push(*it); /* Enqueue the vertex */
        } else { /* If the vertex already has some matching */
            dist[*it] = INF; /* Set its distance from start to infinity */
        }
    }

    dist[NIL] = INF; /* Set distance of Nil to infinity */

    /* Finding the shortest augmented path */
    while (!queue.empty()) { /* Process whole queue */
        TVertex vertex = queue.front(); /* Enqueue a vertex */
        queue.pop();
        if (dist[vertex] < dist[NIL]) { /* If the vertex distance is lesser than infinity */
            /* For each vertex in adjacency list */
            for (TVertexListIter it = graph.adj[vertex].begin(); it != graph.adj[vertex].end(); it++) {
                /* If the vertex pair distance is infinite */
                if (dist[pairGirls[*it]] == INF) {
                    /* Set its distance to its predecessor's distance + 1*/
                    dist[pairGirls[*it]] = dist[vertex] + 1;
                    /* Enqueue the vertex pair */
                    queue.push(pairGirls[*it]);
                }
            }
        }
    }

    /* Finding was a success */
    return dist[NIL] != INF;
}

bool CGraph::DFS(TVertex vertex)
{
    if (vertex != NIL) { /* If the vertex isn't at the end of a path */
        /* For each vertex in adjacency list */
        for (TVertexListIter it = graph.adj[vertex].begin(); it != graph.adj[vertex].end(); it++) {
            /* If two vertices are next to each other */
            if (dist[pairGirls[*it]] == dist[vertex] + 1) {
                /* Run DFS on paired vertex */
                if (DFS(pairGirls[*it])) {
                    /* Pair both vertices */
                    pairGirls[*it] = vertex;
                    pairBoys[vertex] = *it;
                    return true;
                }
            }
        }

        /* No shortest augmented path has been found */
        dist[vertex] = INF;
        return false;
    }

    return true;
}

TMatching CGraph::edmondsKarp()
{
#ifdef MEASURE_TIME
    clock_t begin = clock();
#endif
	CNetwork network = CNetwork(graph);
	TMatching max = network.edmondsKarp();
#ifdef MEASURE_TIME
    clock_t end = clock();
    std::cout << "Time: " << double(end - begin) / CLOCKS_PER_SEC << std::endl;
#endif
  
    return max;
}

TMatching CGraph::hopcroftKarp()
{
    TMatching max;
    TVertex vertex;

    /* Initialization */
    pairBoys.clear();
    pairGirls.clear();
    dist.clear();

#ifdef MEASURE_TIME
    clock_t begin = clock();
#endif

    /* Initialize matching for vertices in first set */
    for (TVertexListIter it = graph.boys.begin(); it != graph.boys.end(); it++) {
        pairBoys[*it] = NIL;
    }

    /* Initialize matching for vertices in second set */
    for (TVertexListIter it = graph.girls.begin(); it != graph.girls.end(); it++) {
        pairGirls[*it] = NIL;
    }

    /* Run BFS */
    while (BFS()) {
        for (TVertexListIter it = graph.boys.begin(); it != graph.boys.end(); it++) {
            if (pairBoys[*it] == NIL) {
                DFS(*it);
            }
        }
    }

#ifdef MEASURE_TIME
    clock_t end = clock();
    std::cout << "Time: " << double(end - begin) / CLOCKS_PER_SEC << std::endl;
#endif

    for (TVertexMapIter it = pairBoys.begin(); it != pairBoys.end(); it++) {
        if (it->first != NIL && it->second != NIL) {
            max.insert(TVertexPair(it->first, it->second));
        }
    }

    for (TVertexMapIter it = pairGirls.begin(); it != pairGirls.end(); it++) {
        if (it->first != NIL && it->second != NIL) {
            max.insert(TVertexPair(it->first, it->second));
        }
    }

    return max;
}
