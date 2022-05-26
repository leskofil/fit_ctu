//
// Created by filip on 6/17/21.
//


#include "CTextFilter.h"

using namespace std;

CTextFilter::CTextFilter() : CFilter() {}

std::map <std::string, std::shared_ptr<CNote>>
CTextFilter::Find(const std::map <std::string, std::shared_ptr<CNote>> &notes) const {
    map <string, shared_ptr<CNote>> textNotes;
    string pattern;
    cout << "Write text to search" << endl;
    while (!(cin >> pattern)) {
        if (cin.eof())
            throw invalid_argument("EOF");
        cout << "Invalid argument. Try again." << endl;
    }

    for (const auto &entry : std::experimental::filesystem::directory_iterator("./examples/Notes"))
        if (SearchOneFile(entry.path(), pattern)) {
            string filename = entry.path();
            TrimPath(filename);
            auto it = notes.find(filename);
            if (it != notes.end()) {
                textNotes.insert({it->first, it->second});
            }
        }
    return textNotes;
}

bool CTextFilter::SearchOneFile(const string &path, const string &pattern) const {
    ifstream ifs(path);
    if (ifs.fail()) {
        throw runtime_error("Cant open file");
    }
    string line;

    while (getline(ifs, line)) {
        if (KMP(pattern, line))
            return true;
    }
    return false;
}

void CTextFilter::lps_func(const string &txt, vector<int> &Lps) {
    Lps[0] = 0;
    int len = 0;
    size_t i = 1;
    while (i < txt.length()) {
        if (txt[i] == txt[len]) {
            len++;
            Lps[i] = len;
            i++;
            continue;
        } else {
            if (len == 0) {
                Lps[i] = 0;
                i++;
                continue;
            } else {
                len = Lps[len - 1];
                continue;
            }
        }
    }
}

bool CTextFilter::KMP(const std::string &pattern, const std::string &text) const {
    int n = text.length();
    int m = pattern.length();
    vector<int> Lps(m);

    lps_func(pattern, Lps);

    int i = 0, j = 0;
    while (i < n) {
        if (pattern[j] == text[i]) {
            i++;
            j++;
        }
        if (j == m) {
            return true;
        } else if (i < n && pattern[j] != text[i]) {
            if (j == 0)
                i++;
            else
                j = Lps[j - 1];
        }
    }
    return false;
}

void CTextFilter::TrimPath(string &path) {
    const size_t last_slash = path.find_last_of("\\/");
    if (string::npos != last_slash) {
        path.erase(0, last_slash + 1);
    }
    const size_t dot = path.rfind('.');
    if (std::string::npos != dot) {
        path.erase(dot);
    }
}