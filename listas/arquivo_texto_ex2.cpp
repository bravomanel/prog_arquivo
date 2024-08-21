#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <cctype>
#include <string>
#include <algorithm>

using namespace std;

bool isInt(string& cell) {
    if (all_of(cell.begin(), cell.end(), ::isdigit)) {
        return true;
    } else {
        return false;
    }
}

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
                    n_int++;
                } else if (isFloat(temp)) {
                    n_float++;
                } else {
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