#include "FileParser.h"
#include <sstream>
#include <iostream>

FileParser::FileParser(const std::string& filename)
    : hasLine(false), pot1Read(false)
{
    file.open(filename);
    if (!file.is_open()) {
        std::cerr << "Error: could not open file " << filename << std::endl;
    }
}

FileParser::~FileParser() {
    if (file.is_open()) {
        file.close();
    }
}

bool FileParser::readNextLine() {
    if (!std::getline(file, currentLine)) {
        hasLine = false;
        return false;
    }
    hasLine = true;
    pot1Read = false;
    return true;
}

bool FileParser::parseLine(int& pot1, int& pot2) {
    std::stringstream ss(currentLine);
    std::string token;
    std::vector<std::string> parts;

    while (std::getline(ss, token, ';')) {
        parts.push_back(token);
    }

    if (parts.size() < 4) return false;

    try {
        pot1 = std::stoi(parts[1]);
        pot2 = std::stoi(parts[3]);
    } catch (...) {
        return false;
    }

    return true;
}

bool FileParser::readPotVal1(int& value) {
    if (!hasLine) {
        if (!readNextLine()) return false;
    }

    int pot1, pot2;
    if (!parseLine(pot1, pot2)) return false;

    value = pot1;
    pot1Read = true;
    return true;
}

bool FileParser::readPotVal2(int& value) {
    if (!hasLine) {
        if (!readNextLine()) return false;
    }

    int pot1, pot2;
    if (!parseLine(pot1, pot2)) return false;

    value = pot2;

    // after reading pot2, we move to the next line for next call
    readNextLine();
    return true;
}

void FileParser::reset() {
    if (file.is_open()) {
        file.clear();
        file.seekg(0, std::ios::beg);
        hasLine = false;
        pot1Read = false;
    }
}
