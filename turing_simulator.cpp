
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "input_handler.cpp"

#ifdef _WIN64
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

// global stuff
string filename = "istruzioni.txt";
bool verbose = false;
int sleeptime = 0;
//

// trim
void ltrim(string &s, int &counter) {
    int size = 0;
    for (int i = 0; i < s.length() && i < counter; i++) {
        if (s[i] == ' ')
            size++;
        else
            break;
    }

    s.erase(0, size);
    counter -= size;
}

void rtrim(string &s) {
    int size = 0;
    for (int i = s.length(); i > 0; i--) {
        if (s[i] == ' ')
            size++;
        else
            break;
    }
    s.erase(s.length() - size, size);
}

void trim(string &s, int &counter) {
    rtrim(s);
    ltrim(s, counter);
}
//

bool is_digit(const string &str) {
    return str.find_first_not_of(".0123456789") == string::npos;
}

int replace_things(string &stringa) {  // a[0..3]b
    int index = stringa.find("..");

    while (index != -1) {
        int start = stringa[index - 1] - '0';  // trova n
        int end = stringa[index + 2] - '0';    // trova m

        stringa.erase(index - 2, 6);  // cancella [n..m]

        for (int i = start; i <= end; i++) {
            char tmp = i + '0';
            stringa += tmp;  // a0123b
        }

        index = stringa.find("..");
    }
    return stringa.length();
}

void write_error(int linea, string errore) {
    cout << "Errore alla linea: " << linea << endl
         << errore << endl;

    exit(1);
}

void check_input(vector<string> &vettore, int &mamt) {
    if (vettore.size() != 5) {  // (0, 1, 2, 3)
        write_error(mamt, "Il formato delle istruzioni fa schifo");
    }

    for (int i = 0; i < 5; i++) {
        if (vettore[i] == "") {  // (, 1, 2, 3, <)
            write_error(mamt, "Il " + to_string(i + 1) + " parametro della riga è vuoto.");
        }
    }

    int da_leggere = replace_things(vettore[1]);
    int da_scrivere = replace_things(vettore[3]);

    if ((da_leggere != da_scrivere) && da_scrivere > 1) {  // (0, 123, 0, 12, >)
        write_error(mamt, "I dati da leggere e scrivere fanno schifo");
    }

    if (vettore[4] != "<" && vettore[4] != ">" && vettore[4] != "-") {  //(0, 0, 0, 0, 0)
        write_error(mamt, "L'ultima istruzione fa schifo");
    }
}

vector<string> split(const string &s, char delim) {
    vector<string> result;
    stringstream ss(s);
    string item;

    while (getline(ss, item, delim)) {
        result.push_back(item);
    }

    return result;  // "a,b,c,d" -> [a, b, c, d]
}

class istruzione {
   public:
    string stato_iniziale;
    char leggi_carattere;

    string stato_finale;
    char scrivi_carattere;

    char direzione;

    string to_string() {
        return stato_iniziale + ", " + leggi_carattere + ", " + stato_finale + ", " + scrivi_carattere + ", " + direzione;
    }

    bool exist() {
        return !(stato_iniziale == "");
    }
};

unordered_map<string, unordered_map<char, istruzione>> get_instruction() {
    ifstream cin(filename);

    unordered_map<string, unordered_map<char, istruzione>>
        istruzioni;
    string input_read;

    int linea = 0;
    while (getline(cin, input_read)) {
        linea++;
        if (linea == 1 || input_read.empty()) continue;  // la prima linea contiene il nastro iniziale

        // toglie parentesi e spazi dall'istruzione
        input_read.erase(
            remove_if(input_read.begin(), input_read.end(), [](auto carattere) {
                return carattere == ' ' || carattere == '(' || carattere == ')';
            }),
            input_read.end()  //
        );

        vector<string> istruzioni_singole = split(input_read, ',');  // "a,b,c,d" -> [a, b, c, d]
        check_input(istruzioni_singole, linea);

        replace(istruzioni_singole[1].begin(), istruzioni_singole[1].end(), '-', ' ');
        replace(istruzioni_singole[3].begin(), istruzioni_singole[3].end(), '-', ' ');

        for (int i = 0; i < istruzioni_singole[1].length(); i++) {
            int index = istruzioni_singole[3].length() > 1 ? i : 0;  //(0, 10, 0, 1, >) o (0, 10, 0, 21, >)

            istruzione _istruzione = {istruzioni_singole[0], istruzioni_singole[1][i], istruzioni_singole[2], istruzioni_singole[3][index], istruzioni_singole[4][0]};
            istruzioni[_istruzione.stato_iniziale][_istruzione.leggi_carattere] = _istruzione;
        }
    }
    return istruzioni;
}

void print_info(string nastro, int passi, string stato, int posizione) {
    // #ifdef _WIN64
    //     system("cls");
    // #else
    //     cout << "\033c";
    // #endif

    cout << "Passi eseguiti: " << passi << endl
         << "Stato: " << stato << endl
         << nastro << endl
         << string(posizione, ' ') + "^" << endl;
}

void actually_sleep(int seconds) {
#ifdef _WIN64
    Sleep(seconds / 1000);
#else
    usleep(seconds);
#endif
}

int main(int argc, char **argv) {
    InputParser arg_handler(argc, argv);

    if (arg_handler.cmdOptionExists("-t")) {
        verbose = true;

        string value = arg_handler.getCmdOption("-t");
        sleeptime = !is_digit(value) || value.empty() ? 0.2 * 1000000 : stof(value) * 1000000;
    }

    string possibile_file = arg_handler.getCmdOption("-f");
    filename = possibile_file == "" ? filename : possibile_file;

    ifstream cin(filename);

    string input;
    getline(cin, input);

    if (input == "") input = " ";

    unordered_map<string, unordered_map<char, istruzione>>
        istruzioni = get_instruction();

    string state = "0";
    int position = 0;
    int counter = 0;
    bool stopped = false;
    char carattere_letto;

    while (!stopped) {
        while (position < 0) {
            input = ' ' + input;
            position++;
        }

        int size = input.size();
        while (position >= size) {
            input += ' ';
            size++;
        }

        actually_sleep(sleeptime);

        if (verbose) {
            print_info(input, counter, state, position);
        }

        carattere_letto = input.at(position);
        auto istruzione = istruzioni[state][carattere_letto];

        if (istruzione.exist()) {
            state = istruzione.stato_finale;
            input.at(position) = istruzione.scrivi_carattere;

            switch (istruzione.direzione) {
                case '>':
                    position++;
                    break;

                case '<':
                    position--;
                    break;

                case '-':
                    break;

                default:
                    cout << "Cioè?" << endl;
                    break;
            }
            counter++;
        } else {
            trim(input, position);
            print_info(input, counter, state, position);
            stopped = true;
        }
    }

    return 0;
}