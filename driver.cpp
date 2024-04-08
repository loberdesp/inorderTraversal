// main.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib> // for rand()
#include "mod.h"

struct arrEl {
    public:
        std::string s;
        int index;
};

int main()
{
    std::ifstream file("t.txt", std::ios::binary | std::ios::ate);

    if (!file.is_open())
    {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::string text((std::istreambuf_iterator<char>(file)),
                     (std::istreambuf_iterator<char>()));

    file.close();

    int N = 8; // Number of parts
    int partSize = fileSize / N; // Size of each part
    int remainder = fileSize % N; // Remainder for the last part

    // Allocate memory for an array of strings to store parts of the text
    arrEl *parts = new arrEl[N];

    // Divide the text into parts
    int start = 0;
    for (int i = 0; i < N; ++i)
    {
        int size = partSize;
        if (i == N - 1)
        {
            size += remainder; // Add the remainder to the last part
        }

        parts[i].s = text.substr(start, size); // Extract the part of the text
        parts[i].index = i;

        start += size; // Move to the next part
    }

    // swap parts
    for (int i = 0; i < N; ++i)
    {
        // Swap current part with a random part
        int random_index = rand() % N;
        std::string tmp = parts[i].s;
        int in = parts[i].index;
        parts[i] = parts[random_index];
        parts[random_index].s = tmp;
        parts[random_index].index = in;
    }

    tree_module mod;

    for(int i=0; i < N; ++i) {
        mod.send_packet(parts[i].s, parts[i].index);
    }

    mod.out_complete_message();

    // Free memory for the array of strings
    delete[] parts;

    return 0;
}
