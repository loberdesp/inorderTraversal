// main.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "mod.h"

struct arrEl {          //struktura do trzymania i mieszania pakietow w tablicy
    public:
        std::string s;
        int index;
};

std::string readText(const std::string& filename, int offset, int msg_size) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        exit(1);
    }

    std::string text;
    std::getline(file, text, '\0'); // wczytaj caly plik do stringa
    file.close();

    if (offset == -1) {
        std::srand(std::time(nullptr));
        offset = std::rand() % text.length(); // wygeneruj losowy offset
    }
    offset %= text.length(); // zawin jesli offset jest wiekszy niz dlugosc tekstu

    std::string message;
    int remaining_size = msg_size;
    while (remaining_size > 0) {
        int chunk_size = std::min(remaining_size, static_cast<int>(text.length()) - offset);        //kopiuj tekst tak dlugo dopoki message.size() == msg_size
        message += text.substr(offset, chunk_size);
        remaining_size -= chunk_size;
        offset = (offset + chunk_size) % text.length();
    }
    return message;                 //zwroc string z wiadomoscia odpowiedniej dlugosci
}



int main(int argc, char* argv[])
{
        if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " filename offset msg_size pach_size" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    int offset = std::stoi(argv[2]);
    int msg_size = std::stoi(argv[3]);
    int pack_size = std::stoi(argv[4]);

    //wczytaj tekst z pliku
    std::string message = readText(filename, offset, msg_size);

    //policzenie rozmiaru tablicy
    int arrSize = message.length()/pack_size;

    // alokacja pamieci dla tablicy do podzielenie i mieszania tekstu
    arrEl *parts = new arrEl[arrSize];

    // podziel tekst na pakiety
    int start = 0;
    for (int i = 0; i < arrSize; ++i)
    {
        int size = pack_size;


        parts[i].s = message.substr(start, size); // wez zapisz pakiet do tablicy
        parts[i].index = i;

        start += size; // nastepny pakiet
    }

    // zamiana pakietow
    for (int i = 0; i < arrSize; ++i)
    {
        // zamiana obecnego pakietu z losowo wybranym pakietem
        int random_index = rand() % pack_size;
        std::string tmp = parts[i].s;
        int in = parts[i].index;
        parts[i] = parts[random_index];
        parts[random_index].s = tmp;
        parts[random_index].index = in;
    }

    //inicjalizacja drzewa
    tree_module mod;

    //wyslanie pakietow do drzewa
    for(int i=0; i < arrSize; ++i) {
        mod.send_packet(parts[i].s, parts[i].index);
    }

    //wyswietlenie gotowej posortowanej wiadomosci
    mod.out_complete_message();

    // zwolnienie pamieci tablicy
    delete[] parts;

    return 0;
}
