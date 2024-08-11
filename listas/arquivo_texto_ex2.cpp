#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <cctype>
#include <string>
#include <algorithm>


//  2. Faça um programa que leia o arquivo texto “valores.txt” que está no formato CSV (commaseparated values). Nesse formato, cada linha possui diversos valores (inteiros, reais ou strings) que
// estão separadas uns dos outros apenas por uma vírgula (não há espaços em brancos entre os
// valores; espaços em branco só existem entre duas palavras da mesma string). O programa deve
// mostrar quantos números inteiros, reais e strings existem em cada linha do arquivo. Um número
// só será considerando como real se possuir “.” (ex: 5.478). Um número só será considerado inteiro
// se possuir apenas algarismos de “0..9”. Qualquer outro tipo de informação é considerado string.

using namespace std;

bool isInt(string& cell) {
    if (all_of(cell.begin(), cell.end(), ::isdigit)) {
        return true;
    } else {
        return false;
    }
}

// bool isInt(const std::string& s)
// {
//     std::string::const_iterator it = s.begin();
//     while (it != s.end() && std::isdigit(*it)) ++it;
//     return !s.empty() && it == s.end();
// }

bool isFloat(string& cell) {
    istringstream iss(cell);
    float f;
    
    if (iss >> f && iss.eof()) {
        return true;
    } else {
        return false;
    }
}


int main() {
    ifstream file("valores.txt");

    if (file.is_open()) {
        string file_line;
        int n_line = 0;
        while (getline (file, file_line)) {
            vector<string> line_vector;
            stringstream ss(file_line);
            string cell;

            while (getline(ss, cell, ',')) {
                cell.erase(remove_if(cell.begin(), cell.end(), ::isspace), cell.end());
                cell.erase(remove(cell.begin(), cell.end(), '\r'), cell.end());
                line_vector.push_back(cell);
            }

            int n_int = 0, n_float = 0, n_string = 0;
            n_line++;

            for (int i = 0; i < line_vector.size(); i++) {
                string temp = line_vector[i];
                if (isInt(temp)) {
                    // cout << temp << " is int" << endl;
                    n_int++;
                } else if (isFloat(temp)) {
                    // cout << temp << " is float" << endl;
                    n_float++;
                } else {
                    // cout << temp << " is string" << endl;
                    n_string++;
                }
            }

            cout << "Linha: " << n_line << "; ";
            cout << "Inteiros: " << n_int << "; ";
            cout << "Floats: " << n_float << "; ";
            cout << "Strings: " << n_string << "; ";
            cout << endl;
            
        }
        file.close();
    }


    cout << "Programa finalizado com sucesso!" << endl;
    return 0;
}