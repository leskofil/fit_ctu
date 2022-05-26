//
// Created by lesko on 5/5/2021.
//

#ifndef SEM_CFILEHANDLER_H
#define SEM_CFILEHANDLER_H

#include <memory>
#include <map>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <experimental/filesystem>
#include <string>

#include "CNote.h"
#include "CFileExporter.h"
#include "CTextNote.h"
#include "CToDoNote.h"
#include "CShoppingNote.h"

class CFileHandler {
public:

    /**
     * @brief Function imports note to type that user chooses
     * @param notes Map of notes imported in application
     */
    void ImportFile(std::map <std::string, std::shared_ptr<CNote>> &notes);

    /**
     *
     * @param notes
     * @return Map of notes imported in application
     */
    bool ExportFile(std::map <std::string, std::shared_ptr<CNote>> &notes);

    /**
     * @brief Function opens one note
     * 
     * @param notes Map of notes imported in application
     */
    void OpenNote(std::map <std::string, std::shared_ptr<CNote>> &notes);

    /**
     * @brief Function sets atribute completed to true
     * 
     * @param notes Map of notes returned by searcher in application
     */
    void CompleteNotes(std::map <std::string, std::shared_ptr<CNote>> notes);

    /**
     * @brief Function validates name of note given by user
     * @param filename Name of note given by user
     * @return Returns true if note name is valid false otherwise
     */
    static bool isValidFileName(const std::string &filename);

protected:
    CFileExporter fileExporter;

    /**
     * @brief Function prints export menu  standard output
     */
    static void PrintMenuExport();

    /**
     * @brief Function prints import menu on standard output
     */
    static void PrintMenuImport();

    /**
     * @brief Function imports single file to application
     * @param filename Name of file that user chooses to import
     * @param notes Map of notes imported in application
     */
    void ImportSingleFile(const std::string &filename, std::map <std::string, std::shared_ptr<CNote>> &notes);
};

#endif //SEM_CFILEHANDLER_H
