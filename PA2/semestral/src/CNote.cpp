//
// Created by filip on 6/6/21.
//

#include <iostream>
#include "CNote.h"

using namespace std;

CNote::CNote(string name) : m_Name(move(name)) {}

void CNote::AddTag(const std::string &tag) {
    istringstream iss(tag);
    string t;
    while (getline(iss >> ws, t, ',') || getline(iss >> ws, t, '\n')) {
        Rtrim(t);
        if (m_Tags.find(t) == m_Tags.end() && !t.empty())
            m_Tags.insert(t);
    }
}


const std::set <std::string> &CNote::Tags() const {
    return m_Tags;
}

void CNote::Rtrim(string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}



