#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <queue>
#include <unordered_map>
#include <vector>
#include <limits.h>

#include "vertex.cpp"

using namespace std;

class Graph
{
    private:

    bool directed;
    unsigned int vtx_count;
    int **AdjMat;

    unordered_map<char, int> vertex_map;
    vector<Vertex*> vertices;

    void clearMarks()
    {
        for (int i = 0; i < vtx_count; i++) {
            vertices[i]->setMark(false);
        }
    }
    void resetDist(int d = 0)
    {
        for(int i=0;i<vtx_count;i++)
        {
            vertices[i]->setDist(d);
        }
    }
    int getIndex(char lbl)
    {
        if (vertex_map.find(lbl) == vertex_map.end()) return -1;
        return vertex_map[lbl];
    }

    void dfsHelper(int u)
    {
        cout<<vertices[u]->getLabel()<<" ";
        vertices[u]->setMark(true);
        for (int i = 0; i < vtx_count;i++ )
        {
            if(AdjMat[u][i]==0 || vertices[i]->isMarked()) continue;
            dfsHelper(i);
        }
    }

    void topSortHelper(int u)
    {
        vertices[u]->setMark(true);

        for (int i = 0; i < vtx_count; i++) {
            
            if (AdjMat[u][i]==0 || vertices[i]->isMarked())
                continue;
            topSortHelper(i);
        }
        cout << vertices[u]->getLabel() << " ";
    }

    public:

    bool addVertex(char label)
    {
        if (getIndex(label)!=-1) return false;
        vertex_map[label] = vtx_count++;
        Vertex* newV = new Vertex(label);
        vertices.push_back(newV);
        return true;
    }

    bool addEdge(char u, char v, int w)
    {
        int ui = getIndex(u);
        int vi = getIndex(v) ;
        if( ui == -1 || vi == -1)
        {
            cout<<"Error in edge entry"<<u<<" "<<v<<" "<<w<<endl;
            exit(0);
        }

        AdjMat[ui][vi] = w;
        vertices[ui]->incOutDegree();
        vertices[vi]->incInDegree();

        if(!directed) {
            AdjMat[vi][ui] = w;
            vertices[vi]->incOutDegree();
            vertices[ui]->incInDegree();
        }
        return true;
    }

    Graph(string filename, bool flag)
    {
        directed = flag;
        vtx_count = 0;

        if(filename.size()==0) 
        {
            cout<<"Empty Filename\n";
            exit(0);
        }

        std::ifstream file(filename);
        if (!file.is_open())
        {
            cout<<"Unable to open file\n";
            exit(0);
        }

        int w;
        string line;
        char u,v;
        int ui,vi;

        while (getline(file, line))
        {
            if(line=="Vertices" || line=="vertices" || line=="VERTICES" || line.size()==0 ){
                continue;
            }
            if(line=="Edges" || line=="edges" || line=="EDGES"){
                break;
            }
            v = line[0];
            if (addVertex(v)==false)
                cout<<"Repeated vertex label "<<v<<endl;
        }

        AdjMat = new int*[vtx_count];
        for(int i=0;i<vtx_count;i++) {
            AdjMat[i] = new int[vtx_count];
            for(int j=0;j<vtx_count;j++) AdjMat[i][j]=0;
            
        }
        
        while (getline(file, line))
        {
            if(line.size()==0) continue;
            if(line.size() < 5)
            {
                cout<<"Incorrect edge entry "<<line<<endl;
                exit(0);
            }
            stringstream ss(line);
            ss>>u>>v>>w;
            addEdge(u,v,w);
        }

        file.close();
    }
    
    void bfs()
    {
        clearMarks();
        
        for(int i=0;i<vtx_count;i++)
        {
            if(vertices[i]->isMarked())continue;

            queue<int> q;
            q.push(i);
            vertices[i]->setMark(true);

            while (!q.empty()) {
                int v = q.front();
                q.pop();
                cout << vertices[v]->getLabel() << " ";

                for (int i = 0; i < vtx_count;i++)
                {
                    if(AdjMat[v][i]==0)continue;
                    
                    if (!vertices[i]->isMarked()) 
                    {
                        vertices[i]->setMark(true);
                        q.push(i);
                    }
                }
            }
            cout<<endl;
        }
        clearMarks();
    }

    void dfs()
    {
        clearMarks();
        for (int i = 0; i < vtx_count; i++) {
            if (!vertices[i]->isMarked()) {
                dfsHelper(i);
                cout << endl;
            }
        }
        clearMarks();
    }

    int getInDegree(char vertexLabel)
    {
        int u = getIndex(vertexLabel);
        if(u ==-1) return -1;
        return vertices[u]->getInDegree();
    }
    int getOutDegree(char vertexLabel)
    {
        int u = getIndex(vertexLabel);
        if(u ==-1) return -1;
        return vertices[u]->getOutDegree();
    }

    vector<Vertex*> getMaxInDegree(int& maxDegree)
    {
        vector<Vertex*> res;
        maxDegree = 0;
        int currDeg;
        for(int i=0;i<vtx_count;i++)
        {
            currDeg = vertices[i]->getInDegree();
            if(currDeg > maxDegree) maxDegree=currDeg;
        }
        for(int i=0;i<vtx_count;i++)
        {
            currDeg = vertices[i]->getInDegree();
            if(currDeg == maxDegree) res.push_back(vertices[i]);
        }
        return res;
    }

    vector<Vertex*> getMaxOutDegree(int& maxDegree)
    {
        vector<Vertex*> res;
        maxDegree = 0;
        int currDeg;
        for(int i=0;i<vtx_count;i++)
        {
            currDeg = vertices[i]->getOutDegree();
            if(currDeg > maxDegree) maxDegree=currDeg;
        }
        for(int i=0;i<vtx_count;i++)
        {
            currDeg = vertices[i]->getOutDegree();
            if(currDeg == maxDegree) res.push_back(vertices[i]);
        }
        return res;
    }

    void topologicalSort(char label)
    {
        clearMarks();
        if(getIndex(label)==-1) return;
        topSortHelper(getIndex(label));
        cout << endl;
        clearMarks();
    }
    
    void dijkstras(char sourceLabel,bool weighted= true)
    {
        clearMarks();
        
        int count = 0;
        resetDist(INT_MAX);
         
        int src = getIndex(sourceLabel);
        if (src == -1) return;

        vertices[src]->setDist(0);

        while (count < vtx_count)
        {
            int smallest = INT_MAX;
            int next;
            for(int i=0;i<vtx_count;i++)
            {
                if(vertices[i]->isMarked() || AdjMat[src][i]==0)
                    continue;

                if(vertices[i]->getDist() > (vertices[src]->getDist() + AdjMat[src][i])){
                    if(weighted)vertices[i]->setDist(vertices[src]->getDist() + AdjMat[src][i]);
                    else vertices[i]->setDist(vertices[src]->getDist() + 1);
                }
            }
            vertices[src]->setMark(true);
            
            for(int i=0;i<vtx_count;i++)
            {
                if(vertices[i]->isMarked()) continue;
                if(vertices[i]->getDist() < smallest)
                {
                    smallest = vertices[i]->getDist();
                    src = i;
                }
            }
            count++;
        }  

        if(!weighted) return;
        for (int i = 0;i<vtx_count;i++)
        {
			if(vertices[i]->getDist()!=INT_MAX)cout << vertices[i]->getLabel() << ": " << vertices[i]->getDist() << endl;
            else cout << vertices[i]->getLabel() << ": INF" << endl;
        }

        clearMarks();
    }

    void prim()
    {
        int parent[vtx_count];
        int count=0;
        for(int i=0;i<vtx_count;i++)parent[i]=-1;
        clearMarks();
        resetDist(INT_MAX);
        int src = 0;
        vertices[src]->setDist(0);
        parent[src] = -1;

        while(count < vtx_count)
        {
            int smallest = INT_MAX;
            for(int i=0;i<vtx_count;i++)
            {
                if(vertices[i]->isMarked() || AdjMat[src][i]==0)
                    continue;

                if(vertices[i]->getDist() > (vertices[src]->getDist() + AdjMat[src][i])){
                    vertices[i]->setDist(vertices[src]->getDist() + AdjMat[src][i]);
                    parent[i]=src;
                }
            }
            vertices[src]->setMark(true);

            for(int i=0;i<vtx_count;i++)
            {
                if(vertices[i]->isMarked()) continue;
                if(vertices[i]->getDist() < smallest)
                {
                    smallest = vertices[i]->getDist();
                    src = i;
                }
            }
            count++;


        }
        for(int i=1;i<vtx_count;i++)
        {
            if(parent[i]==-1)continue;
            cout << parent[i] <<" -> "<< i << endl;
        }
    }

    vector<Vertex*> getNeighbourhood(char vertexLabel, int neighborhoodSize)
    {
        vector<Vertex*> res;
        int u = getIndex(vertexLabel);
        if(u==-1) return res;

        dijkstras(vertexLabel,false);
        res.push_back(vertices[u]);
        
        for(int d=1;d<=neighborhoodSize;d++)
        {
            for(int i=0;i<vtx_count;i++)
                if(vertices[i]->getDist()==d) res.push_back(vertices[i]);
        }
        clearMarks();
        return res;
    }

    vector<Vertex*> getLargestConnnectedComponent()
    {
        clearMarks();
        vector<Vertex*> res;
        int comp[vtx_count],n_comp=0;
        for(int i=0;i<vtx_count;i++) comp[i]=-1;

        for(int i=0;i<vtx_count;i++)
        {
            if(vertices[i]->isMarked())continue;

            queue<int> q;
            q.push(i);
            vertices[i]->setMark(true);
            comp[i]=n_comp;

            while (!q.empty()) {
                int v = q.front();
                q.pop();
                for (int i = 0; i < vtx_count;i++)
                {
                    if(AdjMat[v][i]==0 || vertices[i]->isMarked()) continue;                  
                    vertices[i]->setMark(true);
                    comp[i]=n_comp;
                    q.push(i);
                }
            }
            n_comp++;
        }
        int comp_size[n_comp];
        for(int i=0;i<n_comp;i++) comp_size[i] = 0;
        for(int i=0;i<vtx_count;i++) comp_size[comp[i]]++;
        
        int lgst=-1,lgst_comp=-1;
        for(int i=0;i<n_comp;i++)
        {
            if(comp_size[i]>lgst)
            {
                lgst=comp_size[i];
                lgst_comp=i;
            }
        }

        for(int i=0;i<vtx_count;i++)
        {
            if(comp[i]==lgst_comp)res.push_back(vertices[i]);
            // cout<<comp[i]<<" ";
        }

        clearMarks();
        
        return res;
    }

};