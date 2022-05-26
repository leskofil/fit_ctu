//
// Created by filip on 6/17/21.
//


#include "CSearcher.h"


using namespace std;

map <string, shared_ptr<CNote>> CSearcher::Search(const map <string, shared_ptr<CNote>> &notes) const {
    int choice;
    map <string, shared_ptr<CNote>> foundNotes;
    unique_ptr <CFilter> filter;

    while (true) {
        cout << "Choose searching filter\n";
        cout << "1. Completed filter\n";
        cout << "2. Tag Filter\n";
        cout << "3. Text filter" << endl;

        while (!(cin >> choice)) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (cin.eof())
                throw invalid_argument("EOF");
            cout << "Invalid argument. Try again." << endl;
        }


        switch (choice) {
            case 1 : {
                filter = make_unique<CCompletedFilter>();
                break;
            }
            case 2 : {
                filter = make_unique<CTagFilter>();
                break;
            }
            case 3 : {
                filter = make_unique<CTextFilter>();
                break;
            }
            default : {
                cout << "Invalid input." << endl;
                continue;
            }
        }
        break;
    }
    auto result = filter->Find(notes);
    for (const auto &n : result) {
        cout << *(n.second) << endl;
    }
    return result;

}
