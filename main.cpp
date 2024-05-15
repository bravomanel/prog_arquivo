#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

#define FILE_NAME "transacoes.csv"
// #define FILE_NAME "tran4k.csv"
// #define FILE_NAME "trank.csv"

struct Transacao {
    int dia;
    int mes;
    int ano;
    int agencia;
    int conta;
    float transacao;
    int agencia_destino;
    int conta_destino;
};

// Faz a conversão de String para Int na leitura do arquivo
bool stringParaInt(string& str, int& result, int linhaAtual) {
    if (str.empty() || str == "NULL" || str == "\r" || str == "\n" || str == "\r\n") {
        str = "0";
    }
    try {
        result = stoi(str);
    } catch (const std::invalid_argument&) {
        std::cerr << "Linha: " << linhaAtual << "Erro de conversão: " << result << " - " << str << std::endl;
        return false;
    } catch (const std::out_of_range&) {
        std::cerr << "Linha: " << linhaAtual << "Número fora do intervalo: " << result << " - " << str << std::endl;
        return false;
    } catch (...) {
        std::cerr << "Linha: " << linhaAtual << "Erro desconhecido: " << result << " - " << str << std::endl;
        return false;
    }
    return true;
}

bool stringParaFloat(string& str, float& result, int linhaAtual) {
    if (str.empty()) {
        str = "0";
    }
    try {
        result = stof(str);
    } catch (const std::invalid_argument&) {
        std::cerr << "Linha: " << linhaAtual << " Erro de conversão: " << result << " - " << str << std::endl;
        return false;
    } catch (const std::out_of_range&) {
        std::cerr << "Linha: " << linhaAtual << "Número fora do intervalo: " << result << " - " << str << std::endl;
        return false;
    } catch (...) {
        std::cerr << "Linha: " << linhaAtual << "Erro desconhecido: " << result << " - " << str << std::endl;
        return false;
    }
    return true;
}

vector<Transacao> filtrarPorMesAno(vector<Transacao>& transacoes, int mesDesejado, int anoDesejado) {
    vector<Transacao> transacoesFiltradas;

    for (const auto& t : transacoes) {
        if (t.mes == mesDesejado && t.ano == anoDesejado) {
            transacoesFiltradas.push_back(t);
        }
    }
    return transacoesFiltradas;
}

vector<Transacao> lerTransacoes(const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    vector<Transacao> transacoes;

    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo!" << endl;
        return transacoes;
    }

    string linha;
    int linhaAtual = 0;
    while (getline(arquivo, linha)) {
        istringstream ss(linha);
        string campo;
        Transacao transf;
        bool dadosValidos = true;
        linhaAtual++;


        std::getline(ss, campo, ',');
        dadosValidos &= stringParaInt(campo, transf.dia, linhaAtual);
        std::getline(ss, campo, ',');
        dadosValidos &= stringParaInt(campo, transf.mes, linhaAtual);
        std::getline(ss, campo, ',');
        dadosValidos &= stringParaInt(campo, transf.ano, linhaAtual);
        std::getline(ss, campo, ',');
        dadosValidos &= stringParaInt(campo, transf.agencia, linhaAtual);
        std::getline(ss, campo, ',');
        dadosValidos &= stringParaInt(campo, transf.conta, linhaAtual);
        std::getline(ss, campo, ',');
        dadosValidos &= stringParaFloat(campo, transf.transacao, linhaAtual);
        std::getline(ss, campo, ',');
        dadosValidos &= stringParaInt(campo, transf.agencia_destino, linhaAtual);
        std::getline(ss, campo, ',');
        dadosValidos &= stringParaInt(campo, transf.conta_destino, linhaAtual);

        // if (transf.mes == mesDesejado && transf.ano == anoDesejado) {
        transacoes.push_back(transf);
        // }
    }

    arquivo.close();
    return transacoes;
}

int main() {
    vector<Transacao> transacoes = lerTransacoes(FILE_NAME);

    vector<Transacao> transacoesFiltradas = filtrarPorMesAno(transacoes, 1, 2019);

    for (const auto& t : transacoesFiltradas) {
        cout << t.dia << "/" << t.mes << "/" << t.ano << " "
            << t.agencia << " " << t.conta << " " << t.transacao << " "
            << t.agencia_destino << " " << t.conta_destino << endl;
    }

    cout << "Programa Finalizado!" << endl;
    return 0;
}
