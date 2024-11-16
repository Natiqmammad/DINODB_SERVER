// dbstruktur.h
#ifndef DBSTRUKTUR_H
#define DBSTRUKTUR_H

#include <iostream>
#include <string>

class INT {
public:
    int value;

};

class REAL {
public:
    float value;
};

class Text {
public:
    std::string value;
};

struct Blob {
    std::vector<unsigned char> value;
};

struct Date {
    std::string value; // YYYY-MM-DD formatında
};

struct Time {
    std::string value; // HH:MM:SS formatında
};

struct DateTime {
    std::string value; // YYYY-MM-DD HH:MM:SS formatında
};

struct Boolean {
    bool value;
};

class DECIMAL {
public:

    double value;
    int n=(int)value;
    double p=value-n;
};

class  Char {
public:
    std::string value; // Sabit uzunluqlu char

};

class  Varchar {
public:
    std::string value; // Dəyişən uzunluqlu varchar
};

class  NUMERIC {
public:
    double value;
    int n=(int)value;
    double p=value-n;
};


class FLOAT {
public:
    float value;
};

class DOUBLE {
public:
    double value;
};

class SMALLINT {
public:
    short value;
};

class BIGINT {
public:
    long long value;
};



struct  Columns {

    std::string name;
    std::string type; // SQL type as a string
};

#endif
