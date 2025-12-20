#include "DigFileParser.h"
#include <iostream>
#include <algorithm>
#include <cctype>

DigFileParser::DigFileParser(const std::string &path)
    : filePath(path), isOpen(false)
{
    openFile();
}

DigFileParser::~DigFileParser()
{
    closeFile();
}

bool DigFileParser::openFile()
{
    file.open(filePath);
    isOpen = file.is_open();
    if (!isOpen)
    {
        std::cerr << "Failed to open file: " << filePath << std::endl;
    }
    return isOpen;
}

void DigFileParser::closeFile()
{
    if (file.is_open())
    {
        file.close();
        isOpen = false;
    }
}

bool DigFileParser::readNextEntry(bool &signalValue)
{
    if (!isOpen)
        return false;

    std::string token;
    // Lire le prochain token séparé par ';'
    while (true)
    {
        if (!std::getline(file, token, ';'))
            return false; // EOF ou erreur

        // Enlever espaces
        token.erase(std::remove_if(token.begin(), token.end(), ::isspace), token.end());
        if (token.empty())
            continue; // sauter tokens vides (ex: ";;" ou terminaison ";")

        // Normaliser en minuscule
        std::string lower = token;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

        if (lower == "1" || lower == "true" || lower == "high")
        {
            signalValue = true;
            return true;
        }
        if (lower == "0" || lower == "false" || lower == "low")
        {
            signalValue = false;
            return true;
        }

        // Fallback: interpréter numérique (non-zero = true)
        try
        {
            signalValue = std::stoi(lower) != 0;
        }
        catch (...)
        {
            signalValue = false;
        }
        return true;
    }
}

bool DigFileParser::isFileOpen() const
{
    return isOpen;
}

void DigFileParser::reset()
{
    if (!isOpen)
        return;
    file.clear();
    file.seekg(0, std::ios::beg);
}