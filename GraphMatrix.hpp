#ifndef DEF_GRAPHMATRIX
#define DEF_GRAPHMATRIX


#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <map>
#include "unistd.h"

//Vertex node ---------------------------------------------- -------------------------
struct Node {
	std::string name;								//Vertex name
	int id;                                     //Vertex number (starting at 0)
	std::string node_type;							//Vertex type (u=user, i=item, c=content)
	Node()
	{
		this->name = "";
		this->node_type = "";
		this->id = -1;
	}
	Node(std::string name, std::string node_type, int id)
	{
		this->name = name;
		this->node_type = "";
		this->id = id;
	}
	void set_(std::string name, std::string node_type, int id)
	{
		this->name = name;
		this->node_type = node_type;
		this->id = id;
	}
};
//Vertex node ---------------------------------------------- -------------------------

//Arc node ---------------------------------------------- -------------------------
struct Edge
{
    int w_init;                                 //
    int time;                                   //
	int weight;									//The weight of the arc
	Node* tail;									//The tail node of the arc
	Node* head;									//Arc big head node

	Edge()
	{
		this->weight = 0;
		this->w_init = 0;
		this->time = 0;
		this->tail = NULL;
		this->head = NULL;
	}
	Edge(int weight, int w_init, int time, Node* tail, Node* head)
	{
		this->w_init = w_init;
		this->time = time;
		this->weight = weight;
		this->tail = tail;
		this->head = head;
	}
	void set_(int weight, int w_init, int time, Node* tail, Node* head)
	{
	    this->w_init = w_init;
		this->time = time;
		this->weight = weight;
		this->tail = tail;
		this->head = head;
	}
};
//Arc node ---------------------------------------------- -------------------------


//Figure----------------------------------------------- ----------------------
class Graph {						//Number of sides

public:
    int nodeNum;								//Vertex number
	int edgeNum;

    //std::vector<std::vector<double>> matrix;				// Adjacency matrix, store the connection relationship of each vertex
    std::vector<Node*> nodes;						//Vertex array
	std::vector<Edge*> edges;						//side array

	enum DefaultNodes {			// Constants that can be used to indicate the type of model diagram (single node, three nodes, four nodes and diagonal)
		SINGLE, TERNARY
	};

public:
	//Construction method
	Graph(DefaultNodes kind, int nodeMaxNum, int edgeMaxNum) {
		// Initialize the property
		nodeNum = 0;
		edgeNum = 0;
		// Create a model of the graph
		this->setDefaultGraph(kind);
	}
	// Initialize the model of the graph (one node | | three nodes)
	void setDefaultGraph(DefaultNodes kind) {
		srand(time(0));
		if (kind == SINGLE) {															// only 1 vertex 0 arcs
			this->nodeNum = 1;
			this->edgeNum = 0;
			Node* A = new Node("A", "c", 0);
			nodes.push_back(A);
		}
	}
	// Add a node
	void add_node(std::string name, std::string node_type) {
		Node* node = new Node(name, node_type, nodeNum);
		nodes.push_back(node);
		this->nodeNum++;
	}
	//Add arc
	void add_edge(Node* tail, Node* head, double weight, int w_init, int time) {
		Edge* edge = new Edge(weight, w_init, time, tail, head);
		edges.push_back(edge);
		this->edgeNum++;
	}
	// Remove the node
	void removeNode(std::string name) {
		Node* node = this->getNode(name);
		if (!node) {
			return;
		}
		for (int i = 0; i < nodeNum; i++) {							// Delete all arcs
			this->removeEdge(node, nodes[i]);
		}
		for (int i = 0; i < nodeNum; i++) {							// Delete all the arcs
			this->removeEdge(nodes[i], node);
		}
		for (int i = 0; i < nodeNum; i++) {							// Move out of the array
			if (nodes[i] == node) {
				nodes.erase(nodes.begin() + i);
				break;
			}
		}
		// release the memory
		delete node;
		this->nodeNum--;
	}
	// Remove the tail to the arc of the head
	void removeEdge(Node * tail, Node * head) {
		for (int i = 0; i < edgeNum; i++) {
			//1. Find this arc
			if ((edges[i]->tail == tail) && (edges[i]->head == head)) {
				Edge* temp = edges[i];
				//2. Remove from array (use iterator to remove the elements of the specified table)
				edges.erase(edges.begin() + i);
				this->edgeNum--;
				delete temp;
			}
		}
	}
	//getter......
	Node* getNode(std::string name) {
		for (int i = 0; i < nodeNum; i++) {
			if (nodes[i]->name == name) {
				return nodes[i];
			}
		}
		return NULL;
	}
	Node* getNode(int id) {
		for (int i = 0; i < nodeNum; i++) {
			if (nodes[i]->id == id) {
				return nodes[i];
			}
		}
		return NULL;
	}

	Edge* getEdge(Node * tail, Node * head) {
		for (int i = 0; i < edgeNum; i++) {
			if (edges[i]->tail == tail && edges[i]->head == head) {
				return edges[i];
			}
		}
		return NULL;
	}
	//print
	void display() {
		//Print arc
		std::cout << "Arc tail" << " --> " << "arc head" << "   " << "weight" << "time" << "   " << std::endl;
		for (int i = 0; i < edgeNum; i++) {
			std::cout << edges[i]->tail->name << "    --> " << edges[i]->head->name << "      " << edges[i]->weight <<
			"       " << edges[i]->time << std::endl;
		}
		std::cout << std::endl;
		std::cout << std::endl;
	}
	//@Override
	void to_string() {
		std::cout << "nodeNum = " << this->nodeNum << std::endl;
		std::cout << "edgeNum = " << this->edgeNum << std::endl;
		this->display();
	}
};



struct Element {
    int row, col;
    double value;

    Element(int row, int col, double value) {
		this->row = row;
		this->col = col;
		this->value = value;
	}
};

class SparseMatrix{

    public:
    SparseMatrix(int n);                   // Square Matrix
    SparseMatrix(int rows, int cols);     // Generic Matrix

    // Get and Set
    double get(int row, int col) const;
    bool set_(int row, int col, double value);

    // Operator overloads
    SparseMatrix operator*(const SparseMatrix& mat) const;

    // Get information about matrix
    int rows() const;
    int cols() const;
    int size_() const;

    // Other functions
    void print(bool full = false) const;

    void transpose();                       // transpose of matrix
    SparseMatrix copy_() const;              // deep copy of matrix
    std::vector<Element*> data;          // data of matrix
private:
    int m_rows, m_cols;

};

SparseMatrix to_sparse_matrix(Graph* G);

#endif // !GraphMatrix





