#ifndef GOATEXCEPTIONS_H
#define GOATEXCEPTIONS_H

#include <stdexcept>
#include <string>

class GoatException: public std::runtime_error {
protected:
    GoatException(const std::string& _message) : runtime_error(_message) {}
};

class GoatFileException: public GoatException {
protected:
    std::string filename;
public:
    GoatFileException( const std::string& _filename, const std::string& _message):
        GoatException(_message),
        filename(_filename)
    {}
    const std::string& Filename() const { return filename; }
};

class GoatInputFileError: public GoatFileException {
public:
    GoatInputFileError( const std::string& _filename, const std::string& _message):
        GoatFileException(_filename,_message)
    {}
};

class GoatOutputFileError: public GoatFileException {
public:
    GoatOutputFileError( const std::string& _filename, const std::string& _message):
        GoatFileException(_filename,_message)
    {}
};
#endif
