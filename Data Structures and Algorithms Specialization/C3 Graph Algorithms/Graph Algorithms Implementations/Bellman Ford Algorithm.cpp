#include <iostream>
#include <algorithm>
#include <vector>
#include <stack>
#include <queue>
#include <unordered_map>
#define deb(x) cout << #x << " : " << x << '\n'
#define logn(x) cout << x << '\n'
#define logs(x) cout << x << ' '
#define log(x) cout << x
#define INF (Int)1000000000001
#define uInt unsigned long long int
#define Int long long int

using namespace std;

typedef pair<Int, Int> iPair;
typedef pair<Int, vector<Int>> RoutePair;

template <typename T, typename U>
void debArr(T arr[], U n)
{
	for (U i = 0; i <= n - 1; i++)
	{
		logs(arr[i]);
	}
	log(endl);
}

template <typename T>
void debVect(vector<T> arr)
{
	for (auto i : arr)
	{
		logs(i);
	}
	log(endl);
}

class DirGraph
{
  private:
	class Vertex
	{
	  public:
		Int key;
		vector<Int> adjList;
		Vertex(const Int &k)
		{
			key = k;
		}
		Vertex(const Int &k, const Int &s)
		{
			key = k;
			adjList.reserve(s);
		}
	};
	class Edge
	{
	  public:
		Int v1, v2;
		Edge()
		{
			v1 = v2 = 0;
		}
		Edge(const Int &x, const Int &y)
		{
			v1 = x;
			v2 = y;
		}
		static string toString(const Edge &e)
		{
			string s = to_string(e.v1) + " " + to_string(e.v2);
			return s;
		}
	};
	Int size;
	vector<Vertex> vertices;
	vector<Edge> edges;
	unordered_map<string, Int> eWeights;

	static vector<Int> retracePath(Int v, vector<Int> prev)
	{
		vector<Int> path;
		v = prev[v];
		while (v != -1)
		{
			path.push_back(v);
			v = prev[v];
		}
		reverse(path.begin(), path.end());
		return path;
	}

  public:
	DirGraph(const Int &n, const Int &m)
	{
		size = n;
		vertices.resize(size, Vertex(0, size));
		edges.resize(m);
		for (Int i = 0; i <= size - 1; i++)
		{
			vertices[i].key = i;
		}
	}
	const vector<Vertex> &getVertices() const
	{
		return vertices;
	}
	void markEdge(const Int &i, const Int &j, const Int &k)
	{
		vertices[i].adjList.push_back(j);
		Edge e(i, j);
		edges.push_back(e);
		eWeights[Edge::toString(e)] = k;
	}
	void bellmanFord_noCycle(const Int &u)
	{
		// For Graph withouth -VE Weight Cycle
		vector<Int> dist(size, INF);
		vector<Int> prev(size, -1);
		dist[u] = 0;
		bool flag = false;
		for (Int i = 1; i <= size - 1; i++)
		{
			flag = false;
			for (auto e : edges)
			{
				if (dist[e.v1] < INF)
				{
					Int tDist = dist[e.v1] + eWeights[Edge::toString(e)];
					if (dist[e.v2] > tDist)
					{
						flag = true;
						dist[e.v2] = max(-1*INF,tDist);
						prev[e.v2] = e.v1;
					}
				}
			}
			if (!flag)
				break;
		}
		log("Source : ");
		logn(u);
		for (Int i = 0; i <= size - 1; i++)
		{
			log("Point ");
			log(i + 1);
			log(" has min path of ");
			logn(dist[i]);
		}
		/*
		NOTE :
			1. We can retrace path using prev vector.
		*/
	}
	void bellmanFord_withCycle(const Int &u)
	{
		// For Graph with -VE Weight Cycle
		vector<Int> dist(size, INF);
		vector<Int> prev(size, -1);
		dist[u] = 0;
		queue<Int> lastRelaxed;
		for (Int i = 1; i <= size; i++)
		{
			bool relaxedAny = false;
			for (auto e : edges)
			{
				if (dist[e.v1] < INF)
				{
					Int tDist = dist[e.v1] + eWeights[Edge::toString(e)];
					if (dist[e.v2] > tDist)
					{
						relaxedAny = true;
						dist[e.v2] = max(-1*INF,tDist);
						prev[e.v2] = e.v1;
						if (i == size)
							lastRelaxed.push(e.v2);
					}
				}
			}
			if (!relaxedAny)
				break;
		}
		if (!lastRelaxed.empty())
		{
			logn("Negative Cycle Exists!");
			vector<bool> visited(size, false);
			while (!lastRelaxed.empty())
			{
				Int x = lastRelaxed.front();
				lastRelaxed.pop();
				visited[x] = true;
				dist[x] = -1 * INF;
				for (auto i : vertices[x].adjList)
				{
					if (!visited[i])
						lastRelaxed.push(i);
				}
			}
		}
		log("Source : ");
		logn(u);
		for (Int i = 0; i <= size - 1; i++)
		{
			log("Point ");
			log(i + 1);
			log(" has min path of ");
			logn(dist[i]);
		}
		/*
		NOTES : 
			1. To get the Path of -VE Weight Cycle, we can use 
			   last element in lastRelaxed Queue. Use prev vector
			   to trace parent xSIZE times, then we will be in Cycle
			   and the just go round the cycle to get all elements.
			   
			   Int y = lastRelaxed.back();
			   for (Int i = 1; i <= size; i++)
				   y = prev[y];
			   vector<Int> cyclePath;
			   for (Int i = y;; i = prev[i])
			   {
					cyclePath.push_back(i);
					if (i == y and cyclePath.size() > 1)
						break;
			   }
			   log("Cycle Path : ");
			   debVect(cyclePath);
		*/
	}
};

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	// Input Vertices are 1-Based

	Int n, m;
	cin >> n >> m;
	DirGraph g(n, m);
	while (m--)
	{
		Int i, j, k;
		cin >> i >> j >> k;
		g.markEdge(--i, --j, k);
	}
	Int x;
	cin >> x;
	g.bellmanFord_withCycle(--x);
	return 0;
}
