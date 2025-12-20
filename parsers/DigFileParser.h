#ifndef DIG_FILE_PARSER_H
#define DIG_FILE_PARSER_H

#include <string>
#include <fstream>

class DigFileParser
{
private:
    std::ifstream file;
    std::string filePath;
    bool isOpen;

public:
    DigFileParser(const std::string &path);
    ~DigFileParser();

    bool openFile();
    void closeFile();
    bool readNextEntry(bool &signalValue);
    bool isFileOpen() const;
    void reset();
};

#endif