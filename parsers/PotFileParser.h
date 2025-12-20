#ifndef POTFILEPARSER_H
#define POTFILEPARSER_H

#include <string>
#include <fstream>
#include <vector>

class FileParser
{
public:
    // Constructor: opens the file
    explicit FileParser(const std::string &filename);

    // Destructor: closes the file
    ~FileParser();

    // Reads potval1 from the current line
    // Returns false if end of file reached
    bool readPotVal1(int &value);

    // Reads potval2 from the current line
    // Returns false if end of file reached
    bool readPotVal2(int &value);

    // Resets reading to the beginning of the file
    void reset();

private:
    std::ifstream file;
    std::string currentLine;
    bool hasLine;
    bool pot1Read; // whether potval1 was already read for this line

    // Reads the next line from file into currentLine
    bool readNextLine();

    // Extracts integer values from currentLine (positions 1 and 3)
    bool parseLine(int &pot1, int &pot2);
};

#endif
