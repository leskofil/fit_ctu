//
// Created by filip on 6/17/21.
//

#include "CCompletedFilter.h"

using namespace std;

map <std::string, std::shared_ptr<CNote>>
CCompletedFilter::Find(const map <string, shared_ptr<CNote>> &notes) const {
    map <string, shared_ptr<CNote>> completedNotes;
    for (const auto &n : notes) {
        if (n.second->IsCompleted()) {
            completedNotes.insert({n.first, n.second});
        }
    }
    return completedNotes;
}
