//
// Created by filip on 6/17/21.
//

#ifndef SEMESTRAL_CTAGFILTER_H
#define SEMESTRAL_CTAGFILTER_H


#include <memory>
#include <map>
#include <iostream>
#include <string>

#include "CNote.h"
#include "CFilter.h"

class CTagFilter : public CFilter {
public:
    /**
     * @brief Construct a new CTagFilter object
     * 
     */
    CTagFilter();

    /**
     * @brief Destroy the CTagFilter object
     * 
     */
    ~CTagFilter() override = default;

    /**
     * @brief Find filtrates notes with specific tags
     * @param notes Map of notes that are imported in application
     * @return Filtered map of  notes
     */
    [[nodiscard]] std::map <std::string, std::shared_ptr<CNote>>
    Find(const std::map <std::string, std::shared_ptr<CNote>> &notes) const override;
};


#endif //SEMESTRAL_CTAGFILTER_H
