//
// Created by lesko on 5/7/2021.
//

#ifndef SEM_CSEARCHER_H
#define SEM_CSEARCHER_H

#include <memory>
#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <limits>
#include "CNote.h"
#include "CFilter.h"
#include "CCompletedFilter.h"
#include "CTagFilter.h"
#include "CTextFilter.h"

class CSearcher {
public:
    /**
    * @brief Fuction gives user options with atributes to search for
    * 
    * @param notes Map of imported notes
    * @return Filtered map of notes
    */
    std::map <std::string, std::shared_ptr<CNote>>
    Search(const std::map <std::string, std::shared_ptr<CNote>> &notes) const;
};


#endif //SEM_CSEARCHER_H
