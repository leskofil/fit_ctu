//
// Created by lesko on 5/9/2021.
//

#ifndef SEM_FITER_H
#define SEM_FITER_H

#include <memory>
#include <map>
#include <string>
#include "CNote.h"
#include "CFilter.h"

class CFilter {
public:
    /**
     * @brief Construct a new CFilter object
     * 
     */
    CFilter();

    /**
     * @brief Destroy the CFilter object
     * 
     */
    virtual ~CFilter();

    /**
     * @brief Find filtrates notes with specific feature
     * @param notes Map of notes that are imported in application
     * @return Filtered map of  notes
     */
    [[nodiscard]] virtual std::map <std::string, std::shared_ptr<CNote>>
    Find(const std::map <std::string, std::shared_ptr<CNote>> &notes) const = 0;
};


#endif //SEM_FITER_H
