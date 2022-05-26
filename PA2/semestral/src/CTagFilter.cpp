//
// Created by filip on 6/17/21.
//


#include "CTagFilter.h"

using namespace std;

CTagFilter::CTagFilter() : CFilter() {}

std::map <std::string, std::shared_ptr<CNote>>
CTagFilter::Find(const map <std::string, std::shared_ptr<CNote>> &notes) const {
    map <string, shared_ptr<CNote>> tagNotes;
    string tag;
    cout << "Write tag to search for" << endl;
    while (!(cin >> tag)) {
        if (cin.eof())
            throw invalid_argument("EOF");
        cout << "Invalid argument. Try again." << endl;
    }

    for (const auto &n : notes) {
        if (n.second->Tags().find(tag) != n.second->Tags().end()) {
            tagNotes.insert({n.first, n.second});
        }
    }
    return tagNotes;
}
