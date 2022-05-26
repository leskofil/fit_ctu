//
// Created by lesko on 6/4/2021.
//

#include "CApplication.h"

using namespace std;

void CApplication::Run() {
    int choice;
    while (true) {
        PrintMenu();
        while (!(cin >> choice)) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (cin.eof())
                throw invalid_argument("EOF");
            cout << "Invalid argument. Try again." << endl;
        }
        switch (choice) {
            case 1 : {
                m_FileHandler.ImportFile(m_NotesByName);
                break;
            }
            case 2 : {
                m_FileHandler.ExportFile(m_NotesByName);
                break;
            }
            case 3 : {
                m_Searcher.Search(m_NotesByName);
                break;
            }
            case 4 : {
                m_FileHandler.OpenNote(m_NotesByName);
                break;
            }
            case 5 : {
                m_FileHandler.CompleteNotes(m_Searcher.Search(m_NotesByName));
                break;
            }
            case 6 : {
                SaveConfig();
                break;
            }
            case 7 : {
                SaveConfig();
                return;
            }
            default:
                cout << "Invalid input. Try again" << endl;
                continue;
        }
    }
}

void CApplication::PrintMenu() {
    cout << "Write number to pick an operation\n";
    cout << "1. Import note\n";
    cout << "2. Export note\n";
    cout << "3. Find note\n";
    cout << "4. Open note\n";
    cout << "5. Complete notes\n";
    cout << "6. Save\n";
    cout << "7. Exit and save" << endl;
}

bool CApplication::SaveConfig() {
    ofstream ofs("./examples/Config.txt", ios::out | ios::trunc);
    if (ofs.fail()) {
        throw runtime_error("Can not open config file");
    }
    for (const auto &x : m_NotesByName) {
        ofs << *(x.second) << endl;
    }
    if (ofs.fail()) {
        throw runtime_error("Can not open config file");
    }
    ofs.close();
    return true;
}

bool CApplication::ReadConfig(ifstream &ifs) {
    string line;
    string noteName;
    string category;
    while (getline(ifs, line)) {
        istringstream iss(line);
        shared_ptr <CNote> note;
        if (!(iss >> noteName >> category))
            return false;
        if (m_NotesByName.find(noteName) != m_NotesByName.end() ||
            !(std::experimental::filesystem::exists("./examples/Notes/" + noteName + ".note")) ||
            !(CFileHandler::isValidFileName(noteName))) {
            cout << noteName << endl;
            return false;
        }
        if (category == TEXT) {
            note = make_shared<CTextNote>(noteName);
        } else if (category == TODO) {
            note = make_shared<CToDoNote>(noteName);
        } else if (category == SHOPPING) {
            note = make_shared<CShoppingNote>(noteName);
        } else
            return false;
        note->ReadInfo(iss);
        m_NotesByName[noteName] = (note);
    }
    return true;
}

CApplication::CApplication() {
    if (!FileStructureGood())
        throw runtime_error("Wrong file structure");
    ifstream ifs("./examples/Config.txt", ios::in);
    if (ifs.fail()) {
        ofstream ofs("./examples/Config.txt");
        if (ofs.fail()) {
            ofs.close();
            throw runtime_error("Can not create file");
        }
    }
    if (!ReadConfig(ifs))
        throw runtime_error("Can not read config.");
    ifs.close();

}

namespace fs = std::experimental::filesystem;

bool CApplication::FileStructureGood() {
    return (fs::is_directory("./examples/Imports") && fs::is_directory("./examples/Exports") &&
            fs::is_directory("./examples/Notes") && fs::is_regular_file("./examples/Config.txt"));
}


