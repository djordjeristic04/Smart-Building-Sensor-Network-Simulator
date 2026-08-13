#ifndef GRAPH_H
#define GRAPH_H
#include "Generator.h"

struct Sensor {
	int id;
	int speed;
};

struct Queue {
public:
	Queue(int cap) : front(0), rear(0), capacity(cap) {
		info = new int[capacity];
	}
	~Queue() {
		delete[] info;
	}
	bool queueEmpty() {
		return front == rear;
	}
	void insert(int x) {
		info[rear++] = x;
	}
	int popQ() {
		return info[front++];
	}

private:
	int* info;
	int front, rear, capacity;
	
};

struct Stack {
public:
	Stack(int cap) : capacity(cap), top(-1) {
		info = new int[capacity];
	};
	~Stack() {
		delete[] info;
	}
	bool stackEmpty() {
		return top == -1;
	};

	int popS() {
		return info[top--];
	}
	void push(int x) {
		info[++top] = x;
	}
private:
	int* info;
	int top, capacity;

};
class Graph {
public:
	Graph(int num, double p, Generator& gen, Sensor* initialSensors);
	~Graph();
	void print();
	void printSensors();
	void generate();
	void generateEdge(Sensor s1, Sensor s2,int i, int j);
	void addSensor(Sensor sensor);
	void removeSensor(int index);
	bool existsId(int id);

	void BFS(int start);
	void DFS(int start);
	void shortestRoute(int start, int end);
	void Dijkstra(double* dist, bool* visit, int* prev, int id);
	void eccentricity();
	int minDistance(double* dist, bool* visit);
	int getIndexFromID(int id);
	int getN() const;

	int* sortedIndices();

private:

	double** initMatrix(int n);
	void freeMatrix() const;

	int n;
	double p;
	double** matrix;

	Sensor* sensors;
	Generator& gen;
};


#endif