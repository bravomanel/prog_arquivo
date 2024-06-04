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

struct Consolidada {
    short agencia;
    short conta;
    short qtdTransacoes;
    float movimentacaoEspecie;
    float movimentacaoEletronica;
};

vector<Transacao> lerTransacoesArquivo(const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    vector<Transacao> transacoes;

    if (!arquivo.is_open()) {
        cerr << "ERROR: Erro ao abrir o arquivo!" << endl;
        return transacoes;
    }

    string linha;
    int linhaAtual = 0;
    char virgula;

    while (getline(arquivo, linha)) {
        stringstream streamlinha(linha);
        Transacao transacao;

        char virgula;
        if (streamlinha >> transacao.dia >> virgula >> transacao.mes >> virgula >> transacao.ano >> virgula
               >> transacao.agencia >> virgula >> transacao.conta >> virgula >> transacao.transacao) {
            
            if (!(streamlinha >> virgula >> transacao.agenciaDestino >> virgula >> transacao.contaDestino)) {
                transacao.agenciaDestino = 0;
                transacao.contaDestino = 0;
            }

            transacoes.push_back(transacao);
            linhaAtual++;
        }
    }

    arquivo.close();
    return transacoes;
}

vector<Consolidada> filtrarMovimentacoesPorMesAno(vector<Transacao>& transacoes, short mesDesejado, short anoDesejado) {
    vector<Consolidada> movimentacoesFiltradas;

    for (const auto& t : transacoes) {
        if (t.mes == mesDesejado && t.ano == anoDesejado) {
            Consolidada c;
            bool existeC = false;
            // Verifica se a agência e conta já foram registradas
            for (auto& m : movimentacoesFiltradas) {
                if (m.agencia == t.agencia && m.conta == t.conta) {
                    existeC = true;
                    if (t.agenciaDestino != 0) {
                        m.movimentacaoEletronica += abs(t.transacao);
                    } else {
                        m.movimentacaoEspecie += abs(t.transacao);
                    }
                    m.qtdTransacoes++;
                    break;
                }
            }
            // Primeira ocorrência da agência e conta
            if (!existeC) {
                c.agencia = t.agencia;
                c.conta = t.conta;
                if (t.agenciaDestino != 0) {
                    c.movimentacaoEletronica = abs(t.transacao);
                    c.movimentacaoEspecie = 0;
                } else {
                    c.movimentacaoEspecie = abs(t.transacao);
                    c.movimentacaoEletronica = 0;
                }
                c.qtdTransacoes = 1;
                movimentacoesFiltradas.push_back(c);
            }
        }
    }
    return movimentacoesFiltradas;
}

void salvarConsolidadas(const string& nomeArquivo, vector<Consolidada>& movimentacoes) {
    ofstream arquivo(nomeArquivo, ios::binary);

    if (!arquivo.is_open()) {
        cerr << "ERROR: Erro ao abrir o arquivo!" << endl;
    }

    for (const auto& m : movimentacoes) {
        arquivo.write((char*)&m, sizeof(Consolidada));
    }

    arquivo.close();
}

vector<Consolidada> carregarConsolidadas(const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    vector<Consolidada> movimentacoes;

    if (!arquivo.is_open()) {
        cerr << "ERROR: Erro ao abrir o arquivo!" << endl;
        return movimentacoes;
    }

    Consolidada c;
    while (arquivo.read((char*)&c, sizeof(Consolidada))) {
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

    ofstream log ("log.txt", ios::app);
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
    cout << "Lendo arquivo de transações..." << endl;

    // vector<Transacao> transacoes = lerTransacoesArquivo(FILE_NAME_10);
    vector<Transacao> transacoes = lerTransacoesArquivo(FILE_NAME_F);

    cout << "Arquivo lido com sucesso!" << endl;

    vector<Transacao> transacoesFiltradas;
    vector<Consolidada> movimentacoes;

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

                Consolidada c;
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
            short tipoConsulta;
            short mes, ano;
            int qtdOperacoes = 0;

            cout << "Digite o mês e o ano desejado (ex: 1 2019):";
            cin >> mes >> ano;

            cout << "Escolha o tipo de consulta" << endl;
            cout << "1 - Apenas em Espécie" << endl;
            cout << "2 - Apenas Eletrônica" << endl;
            cout << "3 - Espécie OU Eletrônica" << endl;
            cout << "4 - Espécie E Eletrônica" << endl;
            cin >> tipoConsulta;


            // cout << "Escolha o tipo de filtro" << endl;
            // cout << "1 - x OU y" << endl;
            // cout << "2 - x E y" << endl;
            // cin >> tipoFiltro;


            clearScreen();

            if (tipoConsulta == 1) {
                cout << "Insira o valor mínimo de movimentação em espécie" << endl;
                cin >> valorMinimoEspecie;

                cout << "Filtrando a partir de : " << valorMinimoEspecie << " espécie " << (tipoConsulta == 1 ? "OU " : "E ") << valorMinimoEletronica << " eletrônica" << endl;
            } else if (tipoConsulta == 2) {
                cout <<  "Insira o valor mínimo de movimentação eletrônica" << endl;
                cin >> valorMinimoEletronica;

                cout << "Filtrando a partir de : " << valorMinimoEspecie << " espécie " << (tipoConsulta == 1 ? "OU " : "E ") << valorMinimoEletronica << " eletrônica" << endl;
            } else if (tipoConsulta == 3) {
                cout << "Insira o valor mínimo de movimentação em espécie" << endl;
                cin >> valorMinimoEspecie;
                cout <<  "Insira o valor mínimo de movimentação eletrônica" << endl;
                cin >> valorMinimoEletronica;

                cout << "Filtrando a partir de : " << valorMinimoEspecie << " espécie " << "OU " << valorMinimoEletronica << " eletrônica" << endl;
            } else if (tipoConsulta == 4) {
                cout << "Insira o valor mínimo de movimentação em espécie" << endl;
                cin >> valorMinimoEspecie;
                cout <<  "Insira o valor mínimo de movimentação eletrônica" << endl;
                cin >> valorMinimoEletronica;

                cout << "Filtrando a partir de : " << valorMinimoEspecie << " espécie " << "E " << valorMinimoEletronica << " eletrônica" << endl;
            } else {
                cout << "Opção inválida!" << endl;
                continue;
            }


            string nomeDados = (mes < 10 ? "0"  : "mesAno") + to_string(mes) + to_string(ano);
            
            ifstream file("consolidadas" + nomeDados + ".bin");
            if (file) {
                file.close();
                movimentacoes = carregarConsolidadas("consolidadas" + nomeDados + ".bin");
            } else {
                movimentacoes = filtrarMovimentacoesPorMesAno(transacoes, mes, ano);
                salvarConsolidadas("consolidadas" + nomeDados + ".bin", movimentacoes);
            }



            if (tipoConsulta == 1) {
                cout << "agencia - conta - especie - qtdTransacoes" << endl;
                for (const auto& m : movimentacoes) {
                    if ((m.movimentacaoEspecie) >= valorMinimoEspecie) {
                        cout << m.agencia << "        " << m.conta << "     " << m.movimentacaoEspecie << "   " << m.qtdTransacoes << endl;
                        qtdOperacoes++;
                    }
                }
            } else if (tipoConsulta == 2) {
                cout << "agencia - conta - eletronica - qtdTransacoes" << endl;
                for (const auto& m : movimentacoes) {
                    if ((m.movimentacaoEletronica) >= valorMinimoEletronica) {
                        cout << m.agencia << "        " << m.conta << "     " << m.movimentacaoEletronica << "   " << m.qtdTransacoes << endl;
                        qtdOperacoes++;
                    }
                }
            } else if (tipoConsulta == 3) {
                cout << "agencia - conta - especie - eletronica - qtdTransacoes" << endl;
                for (const auto& m : movimentacoes) {
                    if ((m.movimentacaoEspecie) >= valorMinimoEspecie || (m.movimentacaoEletronica) >= valorMinimoEletronica) {
                        cout << m.agencia << "        " << m.conta << "     "
                            << m.movimentacaoEspecie << "      " << m.movimentacaoEletronica 
                            << "       " << m.qtdTransacoes << endl;
                        qtdOperacoes++;
                    }
                }
            } else if (tipoConsulta == 4) {
                cout << "agencia - conta - especie - eletronica - qtdTransacoes" << endl;
                for (const auto& m : movimentacoes) {
                    if ((m.movimentacaoEspecie) >= valorMinimoEspecie && (m.movimentacaoEletronica) >= valorMinimoEletronica) {
                        cout << m.agencia << "        " << m.conta << "     " << m.movimentacaoEspecie << "      " << m.movimentacaoEletronica << "         " << m.qtdTransacoes << endl;
                        qtdOperacoes++;
                    }
                }
            }

            string mensagem;
            if (tipoConsulta == 1) {
                mensagem = to_string(mes) + "/" + to_string(ano) + " - " + to_string(valorMinimoEspecie) + " espécie - " + to_string(qtdOperacoes) + " operações";
            } else if (tipoConsulta == 2) {
                mensagem = to_string(mes) + "/" + to_string(ano) + " - " + to_string(valorMinimoEletronica) + " Eletrônica - " + to_string(qtdOperacoes) + " operações";
            } else if (tipoConsulta == 3) {
                mensagem = to_string(mes) + "/" + to_string(ano) + " - " + to_string(valorMinimoEspecie) + " Espécie OU " + to_string(valorMinimoEletronica) + " Eletrônica - " + to_string(qtdOperacoes) + " operações";
            } else if (tipoConsulta == 4) {
                mensagem = to_string(mes) + "/" + to_string(ano) + " - " + to_string(valorMinimoEspecie) + " Espécie E " + to_string(valorMinimoEletronica) + " Eletrônica - " + to_string(qtdOperacoes) + " operações";
            }
            atualizaLog(mensagem);

        } else {
            cout << "Opção inválida!" << endl;
        }
    }

    clearScreen();
    cout << "Programa Finalizado!" << endl;
    return 0;
}