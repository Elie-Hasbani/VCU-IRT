#include "CanMessageParser.h"
#include <sstream>
#include <iostream>

/* =======================
 *  Constructor / Destructor
 * ======================= */

CanMessageParser::CanMessageParser(const std::string &filename)
    : filename(filename)
{
    file.open(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: cannot open file " << filename << std::endl;
    }
}

CanMessageParser::~CanMessageParser()
{
    if (file.is_open())
        file.close();
}

/* =======================
 *  Public methods
 * ======================= */

bool CanMessageParser::readNext(Message &msg)
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

        // Expected format: id;value
        std::stringstream ss(line);
        std::string idStr, valueStr;

        if (!std::getline(ss, idStr, ';'))
            continue;

        if (!std::getline(ss, valueStr))
            continue;

        try
        {
            msg.id = std::stoi(idStr);
            msg.value = std::stoi(valueStr);
            return true;
        }
        catch (...)
        {
            // Invalid line → ignore
            continue;
        }
    }

    return false; // EOF
}

void CanMessageParser::reset()
{
    if (file.is_open())
        file.close();

    file.open(filename);
}

/* =======================
 *  Private helpers
 * ======================= */

void CanMessageParser::trim(std::string &s)
{
    // Left trim
    while (!s.empty() && (s.front() == ' ' || s.front() == '\t'))
        s.erase(s.begin());

    // Right trim
    while (!s.empty() && (s.back() == ' ' || s.back() == '\t'))
        s.pop_back();
}
