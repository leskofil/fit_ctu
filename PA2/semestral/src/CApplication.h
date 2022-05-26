//
// Created by lesko on 5/5/2021.
//

#ifndef SEM_CAPPLICATION_H
#define SEM_CAPPLICATION_H

#include <vector>
#include <map>
#include <memory>
#include <iostream>
#include <fstream>
#include <limits>
#include <string>
#include <experimental/filesystem>

#include "CNote.h"
#include "CTextNote.h"
#include "CToDoNote.h"
#include "CShoppingNote.h"
#include "CFileHandler.h"
#include "CSearcher.h"

class CApplication {
public:
    /**
     * @brief Construct a new CApplication object
     * 
     */
    CApplication();

    /**
     * @brief 
     * 
     * @param a 
     * @return CApplication& 
     */

    CApplication &operator=(const CApplication &a) = delete;

    CApplication(const CApplication &a) = delete;

    ~CApplication() = default;

    /**
     * @brief Function to run the application
     */
    void Run();

protected:
    std::map <std::string, std::shared_ptr<CNote>> m_NotesByName;

    CFileHandler m_FileHandler;
    CSearcher m_Searcher;
private:
    /**
     * @brief Function to print main menu
     */
    static void PrintMenu();

    /**
     * Function to safe configuration of application
     * @return Returns true if operation was successful false otherwise
     */
    bool SaveConfig();

    /**
     * @brief Function reads configuration file and loads data
     * @param ifs Input strem with configuration file
     * @return Returns true if operation was successful false otherwise
     */
    bool ReadConfig(std::ifstream &ifs);

    /**
     * @brief Function to check if structure of files is acceptable
     * @return  Returns true if file structure is correct false otherwise
     */
    bool FileStructureGood();

    inline const static std::string TEXT = "TextNote";
    inline static const std::string TODO = "ToDoNote";
    inline static const std::string SHOPPING = "ShoppingNote";
};

#endif //SEM_CAPPLICATION_H
