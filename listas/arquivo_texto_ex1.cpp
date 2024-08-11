#include <iostream>
#include <fstream>
#include <map>
#include <cctype>
#include <algorithm>

using namespace std;

bool compare_pairs(pair<char, int>& a, pair<char, int>& b) {
    return a.second > b.second;
}

vector<pair<char, int>> sort(map<char, int>& unordered_map) 
{ 
    vector<pair<char, int> > temp_vector; 
 
    for (auto& i : unordered_map) { 
        temp_vector.push_back(i); 
    } 
 
    sort(temp_vector.begin(), temp_vector.end(), compare_pairs); 

    return temp_vector;
} 

map<char, int> map_characters(string content) {
    map<char, int> characters_table;
    for (int i = 0; i < content.length(); i++) {
        char tmp = content[i];

        if (islower(tmp) || isupper(tmp) || isdigit(tmp)) {
            if (characters_table[tmp]) {
                characters_table[tmp]++;
            } else {
                characters_table[tmp] = 1;
            }
        }
    }
    return characters_table;
}

string read_file(string text_file_name) {
    fstream file(text_file_name);
    string file_content;

    if (file.is_open()) {
        while (getline (file, file_content)) {}
        file.close();
    }

    return file_content;
}

int main() {
    string text_file_name;
    string file_content;

    cout << "Digite o nome do arquivo de texto (com sua extensão):" << endl;
    cin >> text_file_name;

    file_content = read_file(text_file_name);
    cout << file_content << endl;

    
    map<char, int> counted_characters = map_characters(file_content);
    
    vector<pair<char, int>> ordered_characters = sort(counted_characters);

    for (auto& i : ordered_characters) { 
        cout << i.first << ": " << i.second << endl; 
    } 

    cout << "Programa finalizado com sucesso!" << endl;
    return 0;
}