#include <iostream>
#include <fstream>
#include <map>
#include <cctype>

using namespace std;

void print_ordered_map(map<char, int> map_characters) {
    
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
    string file_content;
    // fstream file(text_file_name);
    fstream file(text_file_name);

    if (file.is_open()) {
        while (getline (file, file_content)) {}
        file.close();
    }

    return file_content;
}

int main() {
    // string text_file_name;
    string text_file_name = "teste.txt";
    string file_content;
    map<char, int> characters_result;

    cout << "Digite o nome do arquivo de texto (com sua extensão):" << endl;
    // cin >> text_file_name;

    file_content = read_file(text_file_name);
    cout << file_content << endl;

    characters_result = map_characters(file_content);

    print_ordered_map(characters_result);


    cout << "My map contain x elemnts: " << characters_result.size();
    for (map<char,int>::iterator it=characters_result.begin(); it!=characters_result.end(); ++it)
        std::cout << '\n' << it->first << " => " << it->second;

    cout << endl;
    return 0;
}