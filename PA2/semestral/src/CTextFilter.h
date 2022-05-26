//
// Created by filip on 6/17/21.
//

#ifndef SEMESTRAL_CTEXTFILTER_H
#define SEMESTRAL_CTEXTFILTER_H

#include <memory>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>

#include <experimental/filesystem>
#include "CNote.h"
#include "CFilter.h"

class CTextFilter : public CFilter {
public:
    /**
     * @brief Construct a new CTextFilter object
     * 
     */
    CTextFilter();

    /**
     * @brief Destroy the CTextFilter object
     * 
     */
    ~CTextFilter() override = default;

    /**
    * @brief Find filtrates notes with specific text
    * @param notes Map of notes that are imported in application
    * @return Filtered map of  notes
    */
    [[nodiscard]] std::map <std::string, std::shared_ptr<CNote>>
    Find(const std::map <std::string, std::shared_ptr<CNote>> &notes) const override;

private:
    /**
     * @brief Function searches one file for
     * @param path Path to the file
     * @param pattern Text to be searched for
     * @return Returns true if text is contained in file otherwise false
     */
    bool SearchOneFile(const std::string &path, const std::string &pattern) const;

    /**
     * @brief Function performs Knuth Morris Pratt algorithm
     * 
     * @param pattern Pattern user is looking for in notes
     * @param text Line of note to be searched
     * @return Returns true if patter is found otherwise false
     */
    bool KMP(const std::string &pattern, const std::string &text) const;

    /**
     * @brief Preprocess line of text
     * 
     * @param txt Line of text
     * @param Lps Array of ints that stores longest prefix which is also suffix
     */
    static void lps_func(const std::string &txt, std::vector<int> &Lps);

    /**
     * @brief Function trims absolute path only to file name
     * 
     * @param path Path of file
     */
    static void TrimPath(std::string &path);
};


#endif //SEMESTRAL_CTEXTFILTER_H
