//
// Created by lesko on 5/7/2021.
//

#ifndef SEM_CFILEEXPORTER_H
#define SEM_CFILEEXPORTER_H

#include <string>
#include <map>
#include <memory>

#include "CNote.h"
#include "CSearcher.h"

class CFileExporter {
public:
    /**
     * @brief Function that exports notes to html format
     * @param notes Map of notes to be exported
     */
    void ExportHtml(const std::map <std::string, std::shared_ptr<CNote>> &notes) const;

    /**
     * @brief Function that exports notes to html format
     * @param notes Map of notes to be exported
     */
    void ExportPlainText(std::map <std::string, std::shared_ptr<CNote>> &notes);

private:
    /**
     * @brief Function prints export menu to standard output
     */
    static void PrintExportMenu();

    /**
    * @brief Function exports one note based on context
    * @param note Single note that is going to be exported
    * @param context Parameter differentiate if notes format is plaintext or html
     */
    void ExportSingleNote(const std::shared_ptr <CNote> &note, char context) const;

    CSearcher m_Searcher;
};


#endif //SEM_CFILEEXPORTER_H
