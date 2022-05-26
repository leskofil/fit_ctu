//
// Created by lesko on 5/5/2021.
//

#ifndef SEM_CTEXTNOTE_H
#define SEM_CTEXTNOTE_H

#include <utility>
#include <ios>
#include <iostream>
#include <fstream>
#include <string>

#include <sstream>
#include "CNote.h"

class CTextNote : public CNote {
public:
    /**
     * @brief Construct a new CTextNote object
     * 
     * @param name Name of note
     */
    explicit CTextNote(std::string name);

    /**
     * @brief Destroy the CTextNote object
     * 
     */
    ~CTextNote() override = default;

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
    [[nodiscard]] bool IsCompleted() const override;

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
    [[nodiscard]] std::string Category() const;

};

#endif //SEM_CTEXTNOTE_H
