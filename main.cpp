#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <climits>

using namespace std;

#define FILE_NAME_F "transacoes.csv"
#define FILE_NAME_10 "tran10k.csv"
#define FILE_NAME_1 "trank.csv"


void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #elif defined(__linux__)
        system("clear");
    #else
        // std::cout << "Unsupported OS" << std::endl;
    #endif
}

struct Transacao {
    short dia;
    short mes;
    short ano;
    short agencia;
    short conta;
    short agenciaDestino;
    short contaDestino;
    float transacao;
};

struct Cliente {
    short agencia;
    short conta;
    short qtdTransacoes;
    float movimentacaoEspecie;
    float movimentacaoEletronica;
};

// Faz a conversão de String para Short na leitura do arquivo, mostrando erros caso ocorram
bool stringParaShort(string& str, short& result, int linhaAtual) {
    int temp;
    if (str.empty() || str == "\r") {
        str = "0";
    }
    try {
        temp = stoi(str);
        if (temp < SHRT_MIN || temp > SHRT_MAX) {
            throw std::out_of_range("Número fora do intervalo de uma short");
        } else {
            result = temp;
            return true;
        }

    } catch (const std::invalid_argument&) {
        std::cerr << "Linha: " << linhaAtual << "Erro de conversão: " << result << " - " << str << std::endl;
        return false;
    } catch (const std::out_of_range&) {
        std::cerr << "Linha: " << linhaAtual <<" - número fora do intervalo, resultado: " << result << " - esperado: " << str << std::endl;
        return false;
    } catch (...) {
        std::cerr << "Linha: " << linhaAtual << "Erro desconhecido: " << result << " - " << str << std::endl;
        return false;
    }
    return true;
}

// Faz a conversão de String para Float na leitura do arquivo, mostrando erros caso ocorram
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

vector<Cliente> filtrarMovimentacoesPorMesAno(vector<Transacao>& transacoes, short mesDesejado, short anoDesejado) {
    vector<Cliente> movimentacoesFiltradas;

    for (const auto& t : transacoes) {
        if (t.mes == mesDesejado && t.ano == anoDesejado) {
            Cliente c;
            bool existe = false;
            for (auto& m : movimentacoesFiltradas) {
                if (m.agencia == t.agencia && m.conta == t.conta) {
                    existe = true;
                    if (t.agenciaDestino != 0) {
                        m.movimentacaoEletronica += t.transacao;
                    } else {
                        m.movimentacaoEspecie += t.transacao;
                    }
                    m.qtdTransacoes++;
                    break;
                }
            }
            if (!existe) {
                c.agencia = t.agencia;
                c.conta = t.conta;
                if (t.agenciaDestino != 0) {
                    c.movimentacaoEletronica = t.transacao;
                    c.movimentacaoEspecie = 0;
                } else {
                    c.movimentacaoEspecie = t.transacao;
                    c.movimentacaoEletronica = 0;
                }
                c.qtdTransacoes = 1;
                movimentacoesFiltradas.push_back(c);
            }
        }
    }
    return movimentacoesFiltradas;
}

vector<Transacao> lerTransacoes(const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    vector<Transacao> transacoes;

    if (!arquivo.is_open()) {
        cerr << "ERROR: Erro ao abrir o arquivo!" << endl;
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
        dadosValidos &= stringParaShort(campo, transf.dia, linhaAtual);
        std::getline(ss, campo, ',');
        dadosValidos &= stringParaShort(campo, transf.mes, linhaAtual);
        std::getline(ss, campo, ',');
        dadosValidos &= stringParaShort(campo, transf.ano, linhaAtual);
        std::getline(ss, campo, ',');
        dadosValidos &= stringParaShort(campo, transf.agencia, linhaAtual);
        std::getline(ss, campo, ',');
        dadosValidos &= stringParaShort(campo, transf.conta, linhaAtual);
        std::getline(ss, campo, ',');
        dadosValidos &= stringParaFloat(campo, transf.transacao, linhaAtual);
        std::getline(ss, campo, ',');
        dadosValidos &= stringParaShort(campo, transf.agenciaDestino, linhaAtual);
        std::getline(ss, campo, ',');
        dadosValidos &= stringParaShort(campo, transf.contaDestino, linhaAtual);

        transacoes.push_back(transf);
    }

    arquivo.close();
    return transacoes;
}

int menu() {
    int opcao;
    cout << endl;
    cout << "Menu" << endl;
    cout << "1 - Filtrar por mês e ano" << endl;
    cout << "0 - Sair" << endl;
    cout << "Digite a opção desejada: ";
    cin >> opcao;
    return opcao;
}

int main() {

    clearScreen();
    cout << "Bem-vindo ao sistema de auditoria de transações!" << endl << endl;


    cout << "Qual arquivo deseja ler?" << endl;
    cout << "1 - 1k transações" << endl;
    cout << "2 - 10k transações" << endl;
    cout << "3 - Todas as transações" << endl;
    cout << "Digite a opção desejada: ";
    int opcaoArquivo;
    cin >> opcaoArquivo;


    cout << "Lendo arquivo de transações..." << endl;
    vector<Transacao> transacoes = lerTransacoes(opcaoArquivo == 1 ? FILE_NAME_1 : opcaoArquivo == 2 ? FILE_NAME_10 : FILE_NAME_F);
    cout << "Arquivo lido com sucesso!" << endl;


    vector<Transacao> transacoesFiltradas;
    vector<Cliente> movimentacoes;

    while (true) {
        int opcao = menu();
        if (opcao == 0) {
            break;
        }

        if (opcao == 1) {

            short mes, ano;
            cout << "Digite o mês e o ano desejado (ex: 1 2019):";
            cin >> mes >> ano;

            string logDados = to_string(mes) + to_string(ano);
            string line;
            bool existe = false;

            clearScreen();
            cout << "Filtrando por: " << mes << "/" << ano << endl;
            
            ifstream arquivoLog ("log.txt");
            if (arquivoLog.is_open()) {
                while ( getline(arquivoLog, line)) {
                    if (logDados == line) {
                        existe = true;
                        cout << "Já existe um log para esse mês e ano" << endl;
                        break;
                    }
                }
                arquivoLog.close();
            }

            string nomeDados;
            if (mes < 10) {
                nomeDados = "0" + to_string(mes) + to_string(ano);
            } else {
                nomeDados = to_string(mes) + to_string(ano);
            }

            if (existe) {
                ifstream dadosBin ("consolidadas" + nomeDados + ".bin", ios::binary);

                if (dadosBin.is_open()) {
                    Cliente c;
                    while (dadosBin.read((char*)&c, sizeof(Cliente))) {
                        cout << c.agencia << " " << c.conta << " "
                            << c.movimentacaoEspecie << " " << c.movimentacaoEletronica 
                            << " " << c.qtdTransacoes << endl;
                    }
                    dadosBin.close();
                } else {
                    cout << "Erro ao abrir arquivo de dados!" << endl;
                }
            } else {
                ofstream arquivoLog ("log.txt", std::ios::app);
                arquivoLog << logDados << endl;
                arquivoLog.close();

                movimentacoes = filtrarMovimentacoesPorMesAno(transacoes, mes, ano);

                cout << "agencia - conta - especie - eletronica - total - qtdTransacoes" << endl;
                for (const auto& m : movimentacoes) {
                    cout << m.agencia << " " << m.conta << " "
                        << m.movimentacaoEspecie << " " << m.movimentacaoEletronica 
                        << " " << m.qtdTransacoes << endl;
                }

                ofstream dadosBin ("consolidadas" + nomeDados + ".bin", ios::binary);
                for (const auto& m : movimentacoes) {
                    dadosBin.write((char*)&m, sizeof(Cliente));
                }
                dadosBin.close();
            }
        }
    }

    cout << "Programa Finalizado!" << endl;
    return 0;
}
