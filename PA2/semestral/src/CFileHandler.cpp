//
// Created by filip on 6/15/21.
//

#include "CFileHandler.h"

using namespace std;

void CFileHandler::ImportFile(std::map <std::string, std::shared_ptr<CNote>> &notes) {
    string fileName;
    string newFileName;

    cout << "Write name of file/folder you want to import." << endl;
    while (!(cin >> fileName)) {
        if (cin.eof())
            throw invalid_argument("EOF");
        cout << "Invalid argument. Try again." << endl;
    }

    if (std::experimental::filesystem::is_directory("./examples/Imports/" + fileName)) {
        for (const auto &entry : std::experimental::filesystem::directory_iterator("./examples/Imports/" + fileName)) {
            ImportSingleFile(entry.path(), notes);
        }
    } else {
        ImportSingleFile("./examples/Imports/" + fileName, notes);
    }

}

void CFileHandler::ImportSingleFile(const string &path, std::map <std::string, std::shared_ptr<CNote>> &notes) {
    int choice;
    string newFileName;
    cout << "Write name of note to be imported. (max 64 char and only alphanumerical with _)" << endl;
    while (!(cin >> newFileName) || !isValidFileName(newFileName) ||
           notes.find(newFileName)->second != notes.end()->second) {
        if (cin.eof())
            throw invalid_argument("EOF");
        cout << "Invalid argument. Try again." << endl;
    }
    ifstream ifs(path, ios::in);
    ofstream ofs("./examples/Notes/" + newFileName + ".note", ios::out);
    if (ifs.fail() || ofs.fail()) {
        cout << "Can not open file" << endl;
        return;
    }
    shared_ptr <CNote> newNote;
    PrintMenuImport();

    while (!(cin >> choice)) {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (cin.eof())
            throw invalid_argument("EOF");
        cout << "Invalid argument. Try again." << endl;
    }

    switch (choice) {
        case 1 : {
            newNote = make_shared<CTextNote>(newFileName);
            break;
        }
        case 2 : {
            newNote = make_shared<CToDoNote>(newFileName);
            break;
        }
        case 3 : {
            newNote = make_shared<CShoppingNote>(newFileName);
            break;
        }
        case 4: {
            return;
        }
        default: {
            cout << "Invalid argument." << endl;
            return;
        };
    }
    string tags;

    newNote->Import(newFileName, ifs, ofs);

    cout << "Add tags to file (divide with ,)" << endl;
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    while (getline(cin, tags)) {
        if (cin.eof())
            throw invalid_argument("EOF");
        newNote->AddTag(tags);
        break;
    }
    notes.insert({newFileName, newNote});
}

void CFileHandler::CompleteNotes(std::map <std::string, std::shared_ptr<CNote>> notes) {
    for (const auto &n : notes)
        n.second->Complete();
}

void CFileHandler::OpenNote(std::map <std::string, std::shared_ptr<CNote>> &notes) {
    string fileName;
    cout << "Write name of note you want to open." << endl;
    while (!(cin >> fileName)) {
        if (cin.eof())
            throw invalid_argument("EOF");
        cout << "Invalid argument. Try again." << endl;
    }
    auto note = notes.find(fileName);
    if (note == notes.end()) {
        cout << "Note does not exists" << endl;
        return;
    } else {
        note->second->ExportPlainText(cout);
    }
}

bool CFileHandler::isValidFileName(const std::string &fileName) {
    return (find_if(fileName.begin(), fileName.end(), [](char c) { return !(isalnum(c) || (c == '_')); }) ==
            fileName.end() &&
            fileName.size() <= 64);
}

bool CFileHandler::ExportFile(map <std::string, std::shared_ptr<CNote>> &notes) {
    int choice;
    string fileName;
    shared_ptr <CNote> note;
    PrintMenuExport();
    while (true) {
        while (!(cin >> choice)) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (cin.eof())
                throw invalid_argument("EOF");
            cout << "Invalid argument. Try again." << endl;
        }

        switch (choice) {
            case 1 : {
                fileExporter.ExportHtml(notes);
                return true;
            }
            case 2 : {
                fileExporter.ExportPlainText(notes);
                return true;
            }
            case 3 : {
                return false;
            }
            default:
                cout << "Invalid input. Try again" << endl;
                continue;
        }
    }
}


void CFileHandler::PrintMenuExport() {
    cout << "1. Export to HTML.\n";
    cout << "2. Export to plain text.\n";
    cout << "3. Back." << endl;
}

void CFileHandler::PrintMenuImport() {
    cout << "Choose note type.\n";
    cout << "1. Text note.\n";
    cout << "2. TODO note.\n";
    cout << "3. Shopping note.\n";
    cout << "4. Back." << endl;
}