#ifndef GOATEXCEPTIONS_H
#define GOATEXCEPTIONS_H

#include <exception>
#include <string>

class GoatException: public std::exception {
public:
    virtual const char *what() const throw() =0;
};

class GoatFileException: public GoatException {
protected:
    std::string filename;
    std::string messge;
public:
    GoatFileException( const std::string& _filename, const std::string& _message):
        filename(_filename),
        messge(_message) {}

    virtual ~GoatFileException() {}

    const char *what() const throw() { return messge.c_str(); }
    const std::string& Filename() const { return filename; }
};

class GoatInputFileError: public GoatFileException {
public:
    GoatInputFileError( const std::string& _filename, const std::string& _message):
        GoatFileException(_filename,_message)
    {}

    virtual ~GoatInputFileError() {}

};

class GoatOutputFileError: public GoatFileException {
public:
    GoatOutputFileError( const std::string& _filename, const std::string& _message):
        GoatFileException(_filename,_message)
    {}

    virtual ~GoatOutputFileError() {}

};
#endif
