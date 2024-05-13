#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define MAX_LINES 10000000

int main() {
    string linha;
    ifstream arquivo("transacoes.csv");


    if (arquivo.is_open()) {
        int lines_read = 0;
        while (getline(arquivo, linha) && lines_read < MAX_LINES) {
            cout << "Linha: " << lines_read << " - " << linha << endl;
            lines_read++;
        }
        arquivo.close();
    } else {
        cout << "Não foi possível abrir o arquivo." << endl;
    }

    cout << "Programa Finalizado!" << endl;
    return 0;
}
