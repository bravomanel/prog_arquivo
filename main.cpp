#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <climits>
#include <ctime>

#define FILE_NAME_F "transacoes.csv"
#define FILE_NAME_10 "tran10k.csv"
#define FILE_NAME_1 "trank.csv"

using namespace std;

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

vector<Transacao> lerArquivoTransacoes(const string& nomeArquivo) {
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

void salvarConsolidadas(const string& nomeArquivo, vector<Cliente>& movimentacoes) {
    ofstream arquivo(nomeArquivo, ios::binary);

    if (!arquivo.is_open()) {
        cerr << "ERROR: Erro ao abrir o arquivo!" << endl;
    }

    for (const auto& m : movimentacoes) {
        arquivo.write((char*)&m, sizeof(Cliente));
    }

    arquivo.close();
}

vector<Cliente> carregarConsolidadas(const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    vector<Cliente> movimentacoes;

    if (!arquivo.is_open()) {
        cerr << "ERROR: Erro ao abrir o arquivo!" << endl;
        return movimentacoes;
    }

    Cliente c;
    while (arquivo.read((char*)&c, sizeof(Cliente))) {
        movimentacoes.push_back(c);
    }

    arquivo.close();
    return movimentacoes;
}

void atualizaLog(string mensagem) {
    // pega a data e o horário atual
    time_t agora = time(0);
    tm *ltm = localtime(&agora);
    string tempoLocal = to_string(ltm->tm_mday) + "/" + to_string(1 + ltm->tm_mon) + "/" + to_string(1900 + ltm->tm_year) + " " + to_string(ltm->tm_hour) + ":" + to_string(ltm->tm_min) + ":" + to_string(ltm->tm_sec);

    ofstream log ("log.txt", std::ios::app);
    log << tempoLocal << " - " << mensagem << endl;
    log.close();
}

int menu() {
    int opcao;
    cout << endl;
    cout << "Menu" << endl;
    cout << "1 - Consultar por mês e ano" << endl;
    cout << "2 - Filtrar consulta por valor" << endl;
    cout << "0 - Sair" << endl;
    cout << "Digite a opção desejada: ";
    cin >> opcao;
    return opcao;
}

int main() {

    clearScreen();
    cout << "Bem-vindo ao sistema de auditoria de transações!" << endl << endl;


    int opcaoArquivo = 2;
    cout << "Qual arquivo deseja ler?" << endl;
    cout << "1 - 1k transações" << endl;
    cout << "2 - 10k transações" << endl;
    cout << "3 - Todas as transações" << endl;
    cout << "Digite a opção desejada: ";
    cin >> opcaoArquivo;


    cout << "Lendo arquivo de transações..." << endl;
    vector<Transacao> transacoes = lerArquivoTransacoes(opcaoArquivo == 1 ? FILE_NAME_1 : opcaoArquivo == 2 ? FILE_NAME_10 : FILE_NAME_F);
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

            string mesAno = to_string(mes) + to_string(ano);
            string line;

            string nomeDados;
            nomeDados = mes < 10 ? "0" + mesAno : mesAno;

            clearScreen();
            cout << "Filtrando por: " << mes << "/" << ano << endl;
            
            ifstream file("consolidadas" + nomeDados + ".bin");

            if (file) {
                file.close();
                movimentacoes = carregarConsolidadas("consolidadas" + nomeDados + ".bin");

                Cliente c;
                cout << "agencia - conta - especie - eletronica - total - qtdTransacoes" << endl;
                for (const auto& c : movimentacoes) {
                    cout << c.agencia << " " << c.conta << " "
                        << c.movimentacaoEspecie << " " << c.movimentacaoEletronica 
                        << " " << c.qtdTransacoes << endl;
                }

            } else {
                movimentacoes = filtrarMovimentacoesPorMesAno(transacoes, mes, ano);
                salvarConsolidadas("consolidadas" + nomeDados + ".bin", movimentacoes);

                cout << "agencia - conta - especie - eletronica - total - qtdTransacoes" << endl;
                for (const auto& m : movimentacoes) {
                    cout << m.agencia << " " << m.conta << " "
                        << m.movimentacaoEspecie << " " << m.movimentacaoEletronica 
                        << " " << m.qtdTransacoes << endl;
                }
            }
        } else if (opcao == 2) {
            float valorMinimoEspecie;
            float valorMinimoEletronica;
            short tipoFiltro;
            short mes, ano;
            int qtdOperacoes = 0;

            cout << "Digite o mês e o ano desejado (ex: 1 2019):";
            cin >> mes >> ano;

            cout << "Insira o valor mínimo de movimentação em espécie" << endl;
            cin >> valorMinimoEspecie;

            cout <<  "Insira o valor mínimo de movimentação eletrônica" << endl;
            cin >> valorMinimoEletronica;

            cout << "Escolha o tipo de filtro" << endl;
            cout << "1 - x OU y" << endl;
            cout << "2 - x E y" << endl;
            cin >> tipoFiltro;

            clearScreen();

            cout << "Filtrando a partir de : " << valorMinimoEspecie << " em espécie " << (tipoFiltro == 1 ? "OU " : "E ") << valorMinimoEletronica << " eletrônica" << endl;

            string nomeDados = (mes < 10 ? "0"  : "mesAno") + to_string(mes) + to_string(ano);;

            ifstream file("consolidadas" + nomeDados + ".bin");
            if (file) {
                file.close();
                movimentacoes = carregarConsolidadas("consolidadas" + nomeDados + ".bin");
            } else {
                movimentacoes = filtrarMovimentacoesPorMesAno(transacoes, mes, ano);
                salvarConsolidadas("consolidadas" + nomeDados + ".bin", movimentacoes);
            }

            cout << "agencia - conta - especie - eletronica - total - qtdTransacoes" << endl;
            for (const auto& m : movimentacoes) {
                if (tipoFiltro == 1) {
                    if (abs(m.movimentacaoEspecie) >= valorMinimoEspecie || abs(m.movimentacaoEletronica) >= valorMinimoEletronica) {
                        cout << m.agencia << " " << m.conta << " "
                            << m.movimentacaoEspecie << " " << m.movimentacaoEletronica 
                            << " " << m.qtdTransacoes << endl;
                        qtdOperacoes++;
                    }
                } else if (tipoFiltro == 2) {
                    if (abs(m.movimentacaoEspecie) >= valorMinimoEspecie && abs(m.movimentacaoEletronica) >= valorMinimoEletronica) {
                        cout << m.agencia << " " << m.conta << " "
                            << m.movimentacaoEspecie << " " << m.movimentacaoEletronica 
                            << " " << m.qtdTransacoes << endl;
                        qtdOperacoes++;
                    }
                }
            }

            atualizaLog(to_string(mes) + "/" + to_string(ano) + " - " + to_string(valorMinimoEspecie) + " - " + to_string(valorMinimoEletronica) + " - " + (tipoFiltro == 1 ? "OU" : "E") + " - " +to_string(qtdOperacoes));

        } else {
            cout << "Opção inválida!" << endl;
        }
    }

    clearScreen();
    cout << "Programa Finalizado!" << endl;
    return 0;
}
