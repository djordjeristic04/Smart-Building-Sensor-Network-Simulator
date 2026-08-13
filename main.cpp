#include <iostream>
#include <fstream>
#include "Generator.h"
#include "Graph.h"

using namespace std;
int nextSensorId = 0;

Sensor* generateSensors(int n, Generator& gen) {
    if (n <= 0) return nullptr;

    Sensor* arr = new Sensor[n];
    for (int i = 0; i < n; i++) {
        arr[i].id = nextSensorId++;
        arr[i].speed = gen.nextIntAB(1, 100);
    }
    return arr;
}

void printSensors(Sensor* sensors, int n) {
    if (!sensors || n <= 0) {
        cout << "Nema senzora." << endl;
        return;
    }

    cout << "Senzori:" << endl;
    for (int i = 0; i < n; i++) {
        cout << "ID: " << sensors[i].id
            << ", brzina: " << sensors[i].speed << endl;
    }
}

Sensor createSensor(Generator& gen, int id) {
    Sensor s;
    s.id = id;
    s.speed = gen.nextIntAB(1, 100);
    return s;
}

int main() {
    int seed;
    Graph* graph = nullptr;
    int n = 0;
    int input = -1;
    Sensor* sensors = nullptr;
    double prag;
    int mod = -1;
    ifstream fin;
    istream* in = nullptr;

    cout << "0.Standardni ulaz" << endl;
    cout << "1.Tekstualna datoteka" << endl;
    cout << "Izaberite nacin unosa: ";
    cin >> mod;
    if (cin.fail()) return 0;

    if (mod) {
        string fajl;
        cout << "Unesite ime tekstualne datoteke: ";
        cin >> fajl;
        fin.open(fajl);
        if (!fin) {
            cout << "Greska pri otvaranju datoteke." << endl;
            return 0;
        }
        in = &fin;
    }
    else in = &cin;

    cout << endl;
    cout << "Unesite pocetnu vrednost generatora: ";
    (*in) >> seed;
    if (mod) cout << seed << endl;

    Generator gen(seed);

    while (input != 0) {
        cout << endl;
        cout << "1. Generisi niz senzora\n";
        cout << "2. Prikazi niz senzora\n";
        cout << "3. Kreiraj graf\n";
        cout << "4. Dodaj senzor u graf\n";
        cout << "5. Ukloni senzor iz grafa\n";
        cout << "6. Prikazi reprezentaciju grafa\n";
        cout << "7. BFS obilazak\n";
        cout << "8. DFS obilazak\n";
        cout << "9. Najkrace vreme izmedju dva senzora\n";
        cout << "10. Ekscentricnost i centralni senzor sistema\n";
        cout << "11. Obrisi graf iz memorije\n";
        cout << "0. Prekid programa\n";

        cout << endl;

        cout << "Unesite redni broj komande koje zelite da izvrsite: ";
        if (in->eof()) break;
        (*in) >> input;
        if (in->fail()) break;
        if (mod) cout << input << endl;
        cout << endl;
        switch (input) {

        case 1: {
            
            if (n == 0 || graph && !graph->getN()) {
                if (sensors) {
                    delete[] sensors;
                    sensors = nullptr;
                }
                cout << "Unesite broj senzora: ";
                (*in) >> n;
                if (mod) cout << n << endl;
                sensors = generateSensors(n, gen);
                break;
            }
            if (sensors) {
                cout << "Niz senzora vec postoji." << endl;
                break;
            }

        }
        case 2: {
            if (!graph) {
                cout << "Graf nije kreiran." << endl;
                break;
            }
            graph->printSensors();
            break;
        }
        case 3:
        {  
            if (!sensors) {
                cout << "Generisite prvo niz senzora." << endl;
                break;
            }
            if (graph) {
                cout << "Graf vec postoji. Prvo ga obrisite." << endl;
                break;
            }

            cout << "Unesite prag verovatnoce u opsegu [" << 1.0 / n << "," << log(n) / n << "]: ";
            (*in) >> prag;
            if (mod) cout << prag << endl;
            
            while (prag < 1.0 / n || prag > log(n) / n) {
                cout << "Neispravan prag p. Treba biti u opsegu [" << 1.0 / n << "," << log(n) / n << "]. Pokusajte ponovo: ";
                (*in) >> prag;
                if (mod) cout << prag << endl;
                if (in->eof()) {
                    input = 0;
                    break;
                }
            }

            graph = new Graph(n, prag, gen, sensors);
            graph->generate();
            if (!graph) cout << "Desila se greska. Graf nije kreiran" << endl;
            else cout << "Graf je uspesno kreiran." << endl;
            break;

        }
        case 4:
        {
            if (!graph) {
                cout << "Graf nije kreiran." << endl;
                break;
            }
            int index;
            cout << "Unesite ID cvora koji zelite da ubacite: ";
            (*in) >> index;
            if (mod) cout << index << endl;
            Sensor s = createSensor(gen, index);
            graph->addSensor(s);
            break;
        }
        case 5:
        {
            if (!graph) {
                cout << "Graf nije kreiran." << endl;
                break;
            }
            int index;
            cout << "Unesite ID senzora koji zelite da uklonite: ";
            (*in) >> index;
            if (mod) cout << index << endl;
            graph->removeSensor(index);

            break;
        }
        case 6:
        {
            if (!graph) {
                cout << "Graf nije kreiran." << endl;
                break;
            }
            graph->print();
            break;
        }
        case 7:
        {
            if (!graph) {
                cout << "Graf nije kreiran." << endl;
                break;
            }
            int start;
            cout << "Unesite ID pocetnog cvora: ";
            (*in) >> start;
            if (mod) cout << start << endl;
            graph->BFS(start);
            break;
        }
        case 8:
        {
            if (!graph) {
                cout << "Graf nije kreiran." << endl;
                break;
            }
            int start;
            cout << "Unesite ID pocetnog cvora: ";
            *in >> start;
            if (mod) cout << start << endl;
            graph->DFS(start);
            break;
        }
        case 9:
        {
            if (!graph) {
                cout << "Graf nije kreiran." << endl;
                break;
            }
            int start, end;
            cout << "Unesite ID pocetnog cvora: ";
            *in >> start;
            if (mod) cout << start << endl;
            cout << "Unesite ID krajnjeg cvora: ";
            *in >> end;
            if (mod) cout << end << endl;
            cout << endl;
            graph->shortestRoute(start, end);
            break;
        }
        case 10:
        {
            if (!graph) {
                cout << "Graf nije kreiran." << endl;
                break;
            }
            graph->eccentricity();
            break;
        }
        case 11:
        {
            if (!graph) {
                cout << "Graf nije kreiran." << endl;
                break;
            }
            delete graph;
            graph = nullptr;
            cout << "Graf je uspesno obrisan." << endl;
            break;
        }
        }
        if (mod  && input != 0) {
            char c;
            cout << endl << "Pritisnite taster N da ucitate sledecu komandu: ";
            cin >> c;
        }

    }

    delete graph;
    delete[] sensors;
    return 0;
}