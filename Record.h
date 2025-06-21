#ifndef Record_h
#define Record_h

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

class Record {
private:
    std::vector<std::string> cols;
    int len;
public:
    Record(const std::vector<std::string> &arr) : len(arr.size()), cols(arr) {}
    Record(const std::string &s) : len(1), cols(1, s) {}
    
    void writeToFile(std::ostream &os) const {
        for (int i = 0; i < len; i++) {
            os << cols[i] << ",";
        }
        os << "\n";
    }
};

#endif