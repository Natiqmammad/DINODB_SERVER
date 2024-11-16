#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <map>
#include <fstream>
#include <sstream>

// Struktur adları
std::string structurenames[] = {"INT", "REAL", "TEXT", "BOOLEAN", "Date", "DateTime",
                                 "DECIMAL", "CHAR", "VARCHAR", "NUMERIC", "FLOAT", "DOUBLE",
                                 "SMALLINT", "BIGINT", "BLOB"};

// Siniflər
class Integer {
public:
    int value;
    Integer(int v) : value(v) {}
};

class ReaL {
public:
    double value;
    ReaL(double v) : value(v) {}
};

class TexT {
public:
    std::string value;
    TexT(std::string v) : value(v) {}
};

class BooleaN {
public:
    bool value;
    BooleaN(bool v) : value(v) {}
};

class DatE {
public:
    std::string value;
    DatE(std::string v) : value(v) {}
};

class DateTimE {
public:
    std::string value;
    DateTimE(std::string v) : value(v) {}
};

class DecimaL {
public:
    double value;
    DecimaL(double v) : value(v) {}
    int n = (int)value;
    double p = value - n;
};

class ChaR {
public:
    std::string value;
    ChaR(std::string v) : value(v) {}
};

class VARChaR {
public:
    std::string value;
    VARChaR(std::string v) : value(v) {}
};

class NUMeriC {
public:
    double value;
    NUMeriC(double v) : value(v) {}
    int n = (int)value;
    double p = value - n;
};

class FLoaT {
public:
    float value;
    FLoaT(float v) : value(v) {}
};

class DOUblE {
public:
    double value;
    DOUblE(double v) : value(v) {}
};

class SMALLinT {
public:
    short value;
    SMALLinT(short v) : value(v) {}
};

class BIGinT {
public:
    long long value;
    BIGinT(long long v) : value(v) {}
};

class BLoB {
public:
    std::vector<uint8_t> value;
    BLoB(std::vector<uint8_t> v) : value(v) {}
};

// Columns strukturu
struct Columns {
    std::string name;
    std::string type;

    Columns(const std::string& n, const std::string& t) : name(n), type(t) {}
};

// Sinifləri əlaqələndirmək üçün xəritə
std::map<std::string, std::string> typeToClassMap = {
    {"INT", "Integer"},
    {"REAL", "ReaL"},
    {"TEXT", "TexT"},
    {"BOOLEAN", "BooleaN"},
    {"Date", "DatE"},
    {"DateTime", "DateTimE"},
    {"DECIMAL", "DecimaL"},
    {"CHAR", "ChaR"},
    {"VARCHAR", "VARChaR"},
    {"NUMERIC", "NUMeriC"},
    {"FLOAT", "FLoaT"},
    {"DOUBLE", "DOUblE"},
    {"SMALLINT", "SMALLinT"},
    {"BIGINT", "BIGinT"},
    {"BLOB", "BLoB"}
};

// Helper funksiyaları
std::string trim(const std::string& str) {
    const std::string whitespace = " \t\n\r\f\v";
    size_t start = str.find_first_not_of(whitespace);
    if (start == std::string::npos) return "";
    size_t end = str.find_last_not_of(whitespace);
    return str.substr(start, end - start + 1);
}

std::string mapTypeToClass(const std::string& type) {
    auto it = typeToClassMap.find(type);
    return it != typeToClassMap.end() ? it->second : "Unknown";
}

// Funksiya: `dbName`, `tableName`, `type`, `value` qəbul edir və uyğunluğu yoxlayır
bool checkValueCompatibility(const std::string& dbName, const std::string& tableName,
                              const std::string& columnName, const std::string& value) {
    std::ifstream strk("/home/DinoDB/dinodb/DBS/databases/" + dbName + "/" + tableName + ".strk");
    if (!strk.is_open()) {
        std::cerr << "\33[31mError: Cannot open structure file!" << std::endl;
        return false;
    }

    std::string line;
    std::string type;
    bool found = false;

    while (std::getline(strk, line)) {
        std::istringstream iss(line);
        std::string name, typeName;
        if (iss >> name >> typeName) {
            if (name == columnName) {
                type = typeName;
                found = true;
                break;
            }
        }
    }

    strk.close();

    if (!found) {
        std::cerr << "\33[31mError: Column not found in structure file!" << std::endl;
        return false;
    }

    type = trim(type);
    std::string className = mapTypeToClass(type);

    try {
        if (className == "Integer") {
            int v = std::stoi(value);
            Integer i(v);
        } else if (className == "ReaL") {
            double v = std::stod(value);
            ReaL r(v);
        } else if (className == "TexT") {
            TexT t(value);
        } else if (className == "BooleaN") {
            if (value == "true" || value == "false") {
                BooleaN b(value == "true");
            } else {
                return false;
            }
        } else if (className == "DatE") {
            DatE d(value);
        } else if (className == "DateTimE") {
            DateTimE dt(value);
        } else if (className == "DecimaL") {
            double v = std::stod(value);
            DecimaL d(v);
        } else if (className == "ChaR") {
            if (value.length() == 1) {
                ChaR c(value);
            } else {
                return false;
            }
        } else if (className == "VARChaR") {
            VARChaR v(value);
        } else if (className == "NUMeriC") {
            double v = std::stod(value);
            NUMeriC n(v);
        } else if (className == "FLoaT") {
            float v = std::stof(value);
            FLoaT f(v);
        } else if (className == "DOUblE") {
            double v = std::stod(value);
            DOUblE d(v);
        } else if (className == "SMALLinT") {
            short v = std::stoi(value);
            SMALLinT s(v);
        } else if (className == "BIGinT") {
            long long v = std::stoll(value);
            BIGinT b(v);
        } else if (className == "BLoB") {
            std::vector<uint8_t> blobValue(value.begin(), value.end());
            BLoB b(blobValue);
        } else {
            return false;
        }
    } catch (...) {
        return false;
    }

    return true;
}


