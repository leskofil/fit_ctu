//
// Created by lesko on 5/5/2021.
//

#ifndef SEM_CSHOPPINGNOTE_H
#define SEM_CSHOPPINGNOTE_H

#include <utility>
#include <ios>
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <fstream>

#include "CNote.h"

class CShoppingNote : public CNote {
public:
    /**
     * @brief Construct a new CShoppingNote object
     * 
     * @param name Name of Note
     */
    explicit CShoppingNote(std::string name);

    /**
     * @brief Destroy the CShoppingNote object
     * 
     */
    ~CShoppingNote() override = default;

    /**
     * @brief Function reads config file
     * @param iss Input stream with config file
     * @return Returns true if config file was successfully read
     */
    bool ReadInfo(std::istringstream &iss) override;

    /**
     * @brief Function prints information about note
     * @param os Output stream where information gonna be printed
     */
    void Print(std::ostream &os) const override;

    /**
     * @brief Function checks if note is completed
     * @return Returns true if note is completed
     */
    bool IsCompleted() const override;

    /**
     * @brief Function completes note
     */
    void Complete() override;

    /**
     * @brief Function that reads file and imports actual note based on type of note
     * @param filename Name of note that is gonna be created
     * @param is Input stream of file that user choose
     * @param os Output stream where note will be stored
     * @return Returns true if import was successful false otherwise
     */
    bool Import(const std::string &fileName, std::istream &, std::ostream &) override;

    /**
     * @brief Function exports actual note to plaintext format based on notes type
     * @param os Output stream where note will be printed
     */
    void ExportHtml(std::ostream &) override;

    /**
     * @brief Function exports actual note to plaintext format based on notes type
     * @param os Output stream where note will be printed
     */
    void ExportPlainText(std::ostream &) override;

private:
    /**
     * @brief Function to get category of note
     * 
     * @return Returns category of note
     */
    std::string Category() const;

    size_t totalAmount;

    bool isCompleted;
};

#endif //SEM_CSHOPPINGNOTE_H
