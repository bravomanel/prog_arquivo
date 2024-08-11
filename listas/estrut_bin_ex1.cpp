#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

using namespace std;

struct Meeting {
    int day;
    int month;
    int year;
    int hour;
    int minute;
    char subject[20];
};

void registerMeeting() {
    Meeting meeting;

    cout << "Digite o dia: ";
    cin >> meeting.day;
    cout << "Digite o mês: ";
    cin >> meeting.month;
    cout << "Digite o ano: ";
    cin >> meeting.year;
    cout << "Digite a hora: ";
    cin >> meeting.hour;
    cout << "Digite o minuto: ";
    cin >> meeting.minute;
    cout << "Digite o assunto da reunião (até 20 caracteres): ";
    cin.ignore();
    cin.getline(meeting.subject, 20);

    ofstream file("agenda.bin", ios::binary | ios::app);
    if (!file) {
        cerr << "Erro ao abrir o arquivo!" << endl;
        return;
    }

    file.write(reinterpret_cast<char*>(&meeting), sizeof(Meeting));
    file.close();

    cout << "Reunião registrada com sucesso!" << endl;
}

void showMeetings() {
    int day, month, year;
    cout << "Digite o dia: ";
    cin >> day;
    cout << "Digite o mês: ";
    cin >> month;
    cout << "Digite o ano: ";
    cin >> year;

    ifstream file("agenda.bin", ios::binary);
    if (!file) {
        cerr << "Erro ao abrir o arquivo!" << endl;
        return;
    }

    Meeting meeting;
    bool found = false;
    while (file.read(reinterpret_cast<char*>(&meeting), sizeof(Meeting))) {
        if (meeting.day == day && meeting.month == month && meeting.year == year) {
            cout << "Hora: " << meeting.hour << ":" << (meeting.minute < 10 ? "0" : "") << meeting.minute
                 << " - Assunto: " << meeting.subject << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "Nenhuma reunião encontrada na data especificada." << endl;
    }

    file.close();
}

int main() {
    int choice;

    do {
        cout << "Menu:\n";
        cout << "1- Registrar uma reunião\n";
        cout << "2- Mostrar reuniões agendadas\n";
        cout << "0- Sair\n";
        cout << "Escolha uma opção: ";
        cin >> choice;

        switch (choice) {
            case 1:
                registerMeeting();
                break;
            case 2:
                showMeetings();
                break;
            case 0:
                cout << "Saindo..." << endl;
                break;
            default:
                cout << "Opção inválida. Tente novamente." << endl;
        }
    } while (choice != 0);

    return 0;
}
