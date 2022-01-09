#include "graph.cpp"

int main() {

	Graph g("mine.graph",true);
	cout << "BFS"<<endl;
	g.bfs();
	cout << "DFS"<<endl;
	g.dfs();
	cout << "Topo"<<endl;
	g.topologicalSort('B');

	cout<< "Djikstra"<<endl;
	g.dijkstras('A');

	cout<< "Prim"<<endl;
	g.prim();

}
