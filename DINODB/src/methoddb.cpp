//
// Created by Natiq on 8/23/2024.
//methods.cpp
#include <cmath>
#include <ctime>
#include <sstream>
#include<iostream>
#include <iomanip>
using  namespace std;
class SQLFunctions {
public:
    // CONCAT
    static std::string CONCAT(const std::string& str1, const std::string& str2) {
        return str1 + str2;
    }

    // SUBSTRING
    static std::string SUBSTRING(const std::string& str, int start, int length) {
        return str.substr(start, length);
    }

    // LENGTH
    static int LENGTH(const std::string& str) {
        return str.length();
    }

    // REPLACE
    static std::string REPLACE(const std::string& str, const std::string& oldSubstr, const std::string& newSubstr) {
        std::string result = str;
        size_t pos = result.find(oldSubstr);
        while (pos != std::string::npos) {
            result.replace(pos, oldSubstr.length(), newSubstr);
            pos = result.find(oldSubstr, pos + newSubstr.length());
        }
        return result;
    }

    // TRIM
    static std::string TRIM(const std::string& str) {
        size_t first = str.find_first_not_of(' ');
        size_t last = str.find_last_not_of(' ');
        return str.substr(first, last - first + 1);
    }
};
class DateFunctions {
public:
    // NOW
    static std::string NOW() {
        time_t t = time(0);
        tm* now = localtime(&t);
        std::stringstream ss;
        ss << (now->tm_year + 1900) << '-'
           << (now->tm_mon + 1) << '-'
           << now->tm_mday << ' '
           << now->tm_hour << ':'
           << now->tm_min << ':'
           << now->tm_sec;
        return ss.str();
    }

    // DATE_ADD (adds days)
    static std::string DATE_ADD(const std::string& date, int days) {
        tm t = {};
        std::stringstream ss(date);
        ss >> std::get_time(&t, "%Y-%m-%d");
        t.tm_mday += days;
        mktime(&t); // Normalize the time structure
        std::stringstream result;
        result << (t.tm_year + 1900) << '-'
               << (t.tm_mon + 1) << '-'
               << t.tm_mday;
        return result.str();
    }

    // DATEDIFF
    static int DATEDIFF(const std::string& date1, const std::string& date2) {
        tm t1 = {}, t2 = {};
        std::stringstream ss1(date1);
        std::stringstream ss2(date2);
        ss1 >> std::get_time(&t1, "%Y-%m-%d");
        ss2 >> std::get_time(&t2, "%Y-%m-%d");
        time_t time1 = mktime(&t1);
        time_t time2 = mktime(&t2);
        return difftime(time1, time2) / (60 * 60 * 24);
    }
};
class NumericFunctions {
public:
    // ROUND
    static double ROUND(double value, int places) {
        double scale = pow(10.0, places);
        return round(value * scale) / scale;
    }

    // ABS
    static double ABS(double value) {
        return fabs(value);
    }

    // FLOOR
    static double FLOOR(double value) {
        return floor(value);
    }

    // CEIL
    static double CEIL(double value) {
        return ceil(value);
    }
};
class AutoIncrement {
private:
    int currentValue;
public:
    AutoIncrement(int start = 1) : currentValue(start) {}
    int next() {
        return currentValue++;
    }
};

