#include <vector>
#include <utility>

using namespace std;

class Vertex
{
	private:
		char label;
		bool mark;
		unsigned int inDeg;
		unsigned int outDeg;
		int dist;
	public:
		Vertex(char lbl)
		{
			label = lbl;
			mark = false;
			inDeg = 0;
			outDeg = 0;
			dist = 0;
		}

		char getLabel()
		{
			return label;
		}

		unsigned int getInDegree()
		{
			return inDeg;
		}

		unsigned int getOutDegree()
		{
			return outDeg;
		}

		void incInDegree()
		{
			inDeg++;
		}

		void incOutDegree()
		{
			outDeg++;
		}

		bool isMarked()
		{
			return mark;
		}

		void setMark(bool flag=false)
		{
			mark = flag;
		}

		int getDist()
		{
			return dist;
		}

		void setDist(int x)
		{
			dist = x;
		}

		void incrDist(int x)
		{
			dist+=x;
		}

		// void clearMark() {
		// 	mark = false;
		// }
};
/*
class Vertex 
{
	private:
		char label;
		vector <pair<Vertex*, int> > neighbors;
		bool mark;

	public:
		Vertex(char label) {
			this->label = label;
			mark = false;
		}

		char getLabel() {
			return label;
		}

		void addNeighbor(Vertex* v) {
			neighbors.push_back(make_pair(v, 1));
		}

		void addNeighbor(Vertex* v, int edge_weight) {
			neighbors.push_back(make_pair(v, edge_weight));
		}

		int getNumNeighbors() {
			return neighbors.size();
		}

		Vertex* getNeighbor(int i) {
			return neighbors[i].first;
		}

		int getNeighborWeight(int i) {
			return neighbors[i].second;
		}

		bool isMarked() {
			return mark;
		}

		void setMark() {
			mark = true;
		}

		void clearMark() {
			mark = false;
		}
};
*/
