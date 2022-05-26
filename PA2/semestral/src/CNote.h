//
// Created by lesko on 5/5/2021.
//

#ifndef SEM_CNOTE_H
#define SEM_CNOTE_H

#include <set>
#include <memory>
#include <utility>
#include <algorithm>
#include <sstream>
#include <string>

class CNote {
public:
    /**
     * @brief Construct a new CNote object
     * 
     * @param name Name of note
     */
    explicit CNote(std::string name);

    /**
     * @brief Destroy the CNote object
     * 
     */
    virtual ~CNote() = default;

    /**
     * @brief Function adds tags to note
     * @param tag Tags that user wants to add
     */
    void AddTag(const std::string &tag);

    /**
     * @brief Function that reads file and imports actual note based on type of note
     * @param filename Name of note that is gonna be created
     * @param is Input stream of file that user choose
     * @param os Output stream where note will be stored
     * @return Returns true if import was successful false otherwise
     */
    virtual bool Import(const std::string &filename, std::istream &is, std::ostream &os) = 0;

    /**
     * @brief Function exports actual note to plaintext format based on notes type
     * @param os Output stream where note will be imported
     */
    virtual void ExportPlainText(std::ostream &os) = 0;

    /**
     * @brief Function exports actual note to plaintext format based on notes type
     * @param os Output stream where note will be exported
     */
    virtual void ExportHtml(std::ostream &os) = 0;

    /**
     * @brief Function checks if note is completed
     * @return Returns true if note is completed
     */
    virtual bool IsCompleted() const = 0;

    /**
     * @brief Function completes note
     */
    virtual void Complete() = 0;

    /**
     * @brief Function returns tags
     * @return Return set of tags
     */
    [[nodiscard]] const std::set <std::string> &Tags() const;

    /**
     * @brief Function reads config file
     * @param iss Input stream with config file
     * @return Returns true if config file was successfully read
     */
    virtual bool ReadInfo(std::istringstream &iss) = 0;

    /**
     * @brief Function prints information about note
     * @param os Output stream where information gonna be printed
     */
    virtual void Print(std::ostream &os) const = 0;

    /**
     * @brief Function prints information about note
     * @param os Output stream where information gonna be printed
     * @param note Note that information gonna be printed
     * @return Returns stream
     */
    friend std::ostream &operator<<(std::ostream &os, const CNote &note) {
        note.Print(os);
        return os;
    }

protected:
    /**
     * @brief Function erases whitespace from left
     * @param s String that is parsed
     */
    void Rtrim(std::string &s);

    std::string m_Name;
    std::set <std::string> m_Tags;
};

#endif //SEM_CNOTE_H
