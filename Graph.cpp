#include "Graph.h"
#include <iostream>
using namespace std;

Graph::Graph(int num, double p, Generator& gen, Sensor* initialSensors) : n(num), p(p), gen(gen), sensors(new Sensor[num]) {
	matrix = initMatrix(n);
	for (int i = 0; i < n; i++) {
		sensors[i] = initialSensors[i];
	}
}

Graph::~Graph() {
	freeMatrix();
	delete[] sensors;
}

void Graph::print() {
	cout << "Matrica susednosti:" << endl;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << matrix[i][j] << "\t";
		}
		cout << endl;
	}
}

void Graph::printSensors() {
	if (n == 0) {
		cout << "Nema senzora." << endl;
		return;
	}

	cout << "Senzori u grafu:" << endl;
	for (int i = 0; i < n; i++) {
		cout << "ID: " << sensors[i].id << ", brzina: " << sensors[i].speed << endl;
	}
}


void Graph::generate() {
	if (!sensors) return;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			
			if (i == j) {
				matrix[i][j] = 0.0;
				continue;
			}
			generateEdge(sensors[i], sensors[j], i, j);
		}
	}
}

void Graph::generateEdge(Sensor s1, Sensor s2, int i, int j) {
	double u = gen.nextReal();
	if (u < p) {
		int d_int = gen.nextIntAB(5, 300);
		double d = d_int / 10.0;
		int minSpeed = s1.speed < s2.speed ? s1.speed : s2.speed;

		matrix[i][j] = d + 100.0 - minSpeed;
	}
	else matrix[i][j] = 0;
}

void Graph::addSensor(Sensor newSensor) {
	if (existsId(newSensor.id)) {
		cout << "Greska. ID vec postoji." << endl;
		return;
	}
	Sensor* newSensors = new Sensor[n + 1];
	for (int i = 0; i < n; i++) {
		newSensors[i] = sensors[i];
	}
	newSensors[n] = newSensor;
	delete[] sensors;

	sensors = newSensors;

	double** newMatrix = initMatrix(n + 1);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			newMatrix[i][j] = matrix[i][j];
		}
	}
	freeMatrix();
	matrix = newMatrix;

	for (int i = 0; i < n; i++) {
		generateEdge(sensors[i], newSensor, i, n);
		generateEdge(newSensor, sensors[i], n, i);
	}
	n++;
	cout << "Senzor ID:" << newSensor.id << " uspesno dodat." << endl;
}

void Graph::removeSensor(int id) {
	int index = getIndexFromID(id);

	if (index == -1) {
		cout << "Greska. ID nije validan." << endl;
		return;
	}
	
	Sensor* newSensors = new Sensor[n - 1];// novi niz senzora
	int si = 0;
	for (int i = 0; i < n; i++) {
		if (i == index) continue;
		newSensors[si++] = sensors[i];
	}

	double** newMatrix = initMatrix(n - 1); //nova matrica

	//matrica bez reda i kolone sa obrisanim senzorom
	int r = 0;
	for (int i = 0; i < n; i++) {
		if (i == index) continue;

		int c = 0;
		for (int j = 0; j < n; j++) {
			if (j == index) continue;
			newMatrix[r][c++] = matrix[i][j];
		}
		r++;
	}

	freeMatrix();
	delete[] sensors;

	matrix = newMatrix;
	sensors = newSensors;
	n--;

	cout << "Senzor ID:" << id << " uspesno uklonjen." << endl;

}

bool Graph::existsId(int id) {
	for (int i = 0; i < n; i++) {
		if (sensors[i].id == id) return true;
	}
	return false;
}

void Graph::BFS(int id) {
	int start = getIndexFromID(id);
	if (start == -1) {
		cout << "Greska. ID nije validan." << endl;
		return;
	}
	bool* visit = new bool[n];
	for (int i = 0; i < n; i++)
		visit[i] = false;

	int* order = sortedIndices();
	Queue Q(n);

	visit[start] = true;
	Q.insert(start);

	cout << "BFS obilazak: ";

	while (!Q.queueEmpty()) {
		int u = Q.popQ();
		cout << sensors[u].id << " ";

		for (int k = 0; k < n; k++) {
			int v = order[k];
			if (matrix[u][v] > 0 && !visit[v]) {
				visit[v] = true;
				Q.insert(v);
			}
		}
	}
	cout << endl;
	delete[] visit;
	delete[] order;
}

void Graph::DFS(int id) {
	int start = getIndexFromID(id);
	if (start == -1) {
		cout << "Greska. ID nije validan." << endl;
		return;
	}
	bool* visit = new bool[n];
	for (int i = 0; i < n; i++) {
		visit[i] = false;
	}
	int* order = sortedIndices();

	Stack S(n);
	S.push(start);

	cout << "DFS obilazak: ";

	while (!S.stackEmpty()) {
		int u = S.popS();
		if (!visit[u]) {
			visit[u] = true;
			cout << sensors[u].id << " ";
		}
		for (int k = n-1; k >= 0; k--) {
			int v = order[k];
			if (matrix[u][v] > 0 && !visit[v]) {
				S.push(v);
			}
		}
	}
	cout << endl;
	delete[] visit;
	delete[] order;
}

void Graph::shortestRoute(int startId, int endId) {
	int start = getIndexFromID(startId);
	int end = getIndexFromID(endId);
	if (start == -1 || end == -1) {
		cout << "Greska. Neki ID nije validan." << endl;
		return;
	}
	double* dist = new double[n];
	bool* visit = new bool[n];
	int* prev = new int[n];
	
	Dijkstra(dist, visit, prev, start); //Dijkstra algoritam za nalazenje najkraceg puta od cvora start
	
	if (dist[end] == 1e10) {
		cout << "Putanja ne postoji." << endl;
		delete[] prev;
		delete[] dist;
		delete[] visit;
		return;
	}

	int* path = new int[n];
	int len = 0;
	for (int v = end; v != -1; v = prev[v]) {
		path[len++] = v;
	}

	cout << "Najkraca putanja: ";
	for (int i = len - 1; i >= 0; i--) {
		cout << sensors[path[i]].id;
		if (i > 0) cout << " -> ";
	}

	cout << "\nUkupno vreme: " << dist[end] << "ms" << endl;

	delete[] path;
	delete[] dist;
	delete[] visit;
	delete[] prev;


}

void Graph::Dijkstra(double* dist, bool* visit, int* prev, int start) {
	for (int i = 0; i < n; i++) {
		if (matrix[start][i] == 0) {
			dist[i] = 1e10;
			prev[i] = -1;
		}
		else {
			dist[i] = matrix[start][i];
			prev[i] = start;
		}
		visit[i] = false;

	}
	visit[start] = true;
	dist[start] = 0.0;

	for (int k = 0; k < n; k++) {
		int i = minDistance(dist, visit);
		if (i == -1) break;
		visit[i] = true;

		for (int j = 0; j < n; j++) {
			if (!visit[j] && matrix[i][j] > 0) {
				if (dist[i] + matrix[i][j] < dist[j]) {
					dist[j] = dist[i] + matrix[i][j];
					prev[j] = i;
				}
			}
		}
	}
}

void Graph::eccentricity() {
	if (n == 0) {
		cout << "Graf je prazan." << endl;
		return;
	}
	double* ecc = new double[n];
	double** distances = new double* [n];
	int** prev = new int* [n];
	bool** visit = new bool* [n];

	for (int i = 0; i < n; i++) {
		distances[i] = new double[n];
		prev[i] = new int[n];
		visit[i] = new bool[n];
	}

	for (int i = 0; i < n; i++) {
		Dijkstra(distances[i], visit[i], prev[i], i); //isti kao Flojdov
	}

	for (int i = 0; i < n; i++) {
		double maxDist = 0.0;
		for (int j = 0; j < n; j++) {
			if (distances[j][i] < 1e18 && distances[j][i] > maxDist)
				maxDist = distances[j][i];
		}
		ecc[i] = maxDist;
	}

	cout << "Ekscentricnosti senzora:\n";
	for (int i = 0; i < n; i++) {
		cout << "ID: " << sensors[i].id << " | ecc = " << ecc[i] << endl;
	}

	double minEcc = ecc[0];
	for (int i = 1; i < n; i++)
		if (ecc[i] < minEcc)
			minEcc = ecc[i];

	cout << "Centralni senzor: ";
	for (int i = 0; i < n; i++) {
		if (ecc[i] == minEcc) {
			cout << "ID: " << sensors[i].id << endl;
			break;
		}
	}


	for (int i = 0; i < n; i++) {
		delete[] visit[i];
		delete[] prev[i];
		delete[] distances[i];
	}
	delete[] visit;
	delete[] prev;
	delete[] distances;
	delete[] ecc;
}

int Graph::minDistance(double* dist, bool* visited) {
	double min = 1e10;
	int minIndex = -1;
	for (int i = 0; i < n; i++) {
		if (!visited[i] && dist[i] < min) {
			min = dist[i];
			minIndex = i;
		}
	}
	return minIndex;
}

int Graph::getIndexFromID(int id) {
	for (int i = 0; i < n; i++) {
		if (sensors[i].id == id) return i;
	}
	return -1;
}

int Graph::getN() const
{
	return n;
}

int* Graph::sortedIndices()
{
	int* order = new int[n];
	for (int i = 0; i < n; i++)
		order[i] = i;

	for (int i = 0; i < n - 1; i++) {
		for (int j = i + 1; j < n; j++) {
			if (sensors[order[i]].id > sensors[order[j]].id) {
				int tmp = order[i];
				order[i] = order[j];
				order[j] = tmp;
			}
		}
	}
	return order;
}


double** Graph::initMatrix(int n) {
	double** matrix = new double* [n];
	for (int i = 0; i < n; i++) {
		matrix[i] = new double[n];
		for (int j = 0; j < n; j++) {
			matrix[i][j] = 0.0;
		}
	}
	return matrix;
}

void Graph::freeMatrix() const {
	for (int i = 0; i < n; i++) {
		delete[] matrix[i];
	}
	delete[] matrix;
}

