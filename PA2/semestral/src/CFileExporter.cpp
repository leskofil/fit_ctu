//
// Created by filip on 6/17/21.
//

#include <iostream>
#include <fstream>
#include <limits>
#include<string>
#include <memory>
#include <map>
#include "CFileExporter.h"

using namespace std;

void CFileExporter::ExportHtml(const map <std::string, std::shared_ptr<CNote>> &notes) const {
    int choice;
    PrintExportMenu();
    while (!(cin >> choice)) {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (cin.eof())
            throw invalid_argument("EOF");
        cout << "Invalid argument. Try again." << endl;
    }

    switch (choice) {
        case 1 : {
            cout << "Write the name of note you want to export" << endl;
            string note;
            while (!(cin >> note)) {
                if (cin.eof())
                    throw invalid_argument("EOF");
                cout << "Invalid argument. Try again." << endl;
            }
            if (notes.find(note) != notes.end()) {
                ExportSingleNote(notes.find(note)->second, 'h');
            } else
                throw invalid_argument("Invalid note");
            return;
        }
        case 2 : {
            for (const auto &n : m_Searcher.Search(notes))
                ExportSingleNote(n.second, 'h');
            return;
        }
        case 3 : {
            for (const auto &n : notes)
                ExportSingleNote(n.second, 'h');
            return;
        }
        default: {
            cout << "Inv choice" << endl;
            return;
        }
    }
}

void CFileExporter::ExportPlainText(std::map <std::string, std::shared_ptr<CNote>> &notes) {
    int choice;
    PrintExportMenu();
    while (!(cin >> choice)) {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (cin.eof())
            throw invalid_argument("EOF");
        cout << "Invalid argument. Try again." << endl;
    }
    switch (choice) {
        case 1 : {
            cout << "Write the name of note you want to export" << endl;
            string note;
            while (!(cin >> note)) {
                if (cin.eof())
                    throw invalid_argument("EOF");
                cout << "Invalid argument. Try again." << endl;
            }
            if (notes.find(note) != notes.end()) {
                ExportSingleNote(notes.find(note)->second, 'p');
            } else
                throw invalid_argument("Invalid note");
            return;
        }
        case 2 : {
            for (const auto &n : m_Searcher.Search(notes))
                ExportSingleNote(n.second, 'p');
            return;
        }
        case 3 : {
            for (const auto &n : notes)
                ExportSingleNote(n.second, 'p');
            return;
        }
        default: {
            cout << "Inv choice" << endl;
            return;
        }
    }
}

void CFileExporter::ExportSingleNote(const std::shared_ptr <CNote> &note, char context) const {
    string filename;
    cout << "Write name of file where note will be exported" << endl;

    while (true) {
        while (!(cin >> filename)) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (cin.eof())
                throw invalid_argument("EOF");
            cout << "Invalid argument. Try again." << endl;
        }

        switch (context) {
            case 'h': {
                ofstream ofs("./examples/Exports/" + filename + ".html");
                if (ofs.fail()) {
                    throw runtime_error("File can not be created");
                }
                note->ExportHtml(ofs);
                return;
            }
            case 'p' : {
                ofstream ofs("./examples/Exports/" + filename + ".txt");
                if (ofs.fail()) {
                    throw runtime_error("File can not be created");
                }
                note->ExportPlainText(ofs);
                return;
            }
            default:
                cout << "Invalid input. Try again" << endl;
                continue;
        }
    }

}

void CFileExporter::PrintExportMenu() {
    cout << "1. Export single file\n";
    cout << "2. Search for files to export\n";
    cout << "3. Export all" << endl;
}



