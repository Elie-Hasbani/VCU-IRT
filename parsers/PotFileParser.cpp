#include "PotFileParser.h"
#include <sstream>
#include <iostream>
#include <vector>

/* =======================
 *  Constructor / Destructor
 * ======================= */

FileParser::FileParser(const std::string &filename)
    : hasLine(false), pot1Read(false)
{
    file.open(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: could not open file " << filename << std::endl;
    }
}

FileParser::~FileParser()
{
    if (file.is_open())
    {
        file.close();
    }
}

/* =======================
 *  Private helpers
 * ======================= */

static void trim(std::string &s)
{
    // Left trim
    while (!s.empty() && (s.front() == ' ' || s.front() == '\t'))
        s.erase(s.begin());

    // Right trim
    while (!s.empty() && (s.back() == ' ' || s.back() == '\t'))
        s.pop_back();
}

/* =======================
 *  Core logic
 * ======================= */

bool FileParser::readNextLine()
{
    std::string line;

    while (std::getline(file, line))
    {
        trim(line);

        // Ignore empty lines
        if (line.empty())
            continue;

        // Ignore comments
        if (line[0] == '#')
            continue;

        // Valid line found
        currentLine = line;
        hasLine = true;
        pot1Read = false;
        return true;
    }

    hasLine = false;
    return false;
}

bool FileParser::parseLine(int &pot1, int &pot2)
{
    std::stringstream ss(currentLine);
    std::string token;
    std::vector<std::string> parts;

    while (std::getline(ss, token, ';'))
    {
        trim(token);
        parts.push_back(token);
    }

    // Expected format: potval1;XXXX;potval2;YYYY
    if (parts.size() < 4)
        return false;

    try
    {
        pot1 = std::stoi(parts[1]);
        pot2 = std::stoi(parts[3]);
    }
    catch (...)
    {
        return false;
    }

    return true;
}

bool FileParser::readPotVal1(int &value)
{
    if (!hasLine)
    {
        if (!readNextLine())
            return false;
    }

    int pot1, pot2;
    if (!parseLine(pot1, pot2))
        return false;

    value = pot1;
    pot1Read = true;
    return true;
}

bool FileParser::readPotVal2(int &value)
{
    if (!hasLine)
    {
        if (!readNextLine())
            return false;
    }

    int pot1, pot2;
    if (!parseLine(pot1, pot2))
        return false;

    value = pot2;

    // Move to next valid line for next cycle
    readNextLine();
    return true;
}

void FileParser::reset()
{
    if (file.is_open())
    {
        file.clear();
        file.seekg(0, std::ios::beg);
        hasLine = false;
        pot1Read = false;
    }
}
