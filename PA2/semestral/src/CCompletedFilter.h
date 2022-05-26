//
// Created by filip on 6/17/21.
//

#ifndef SEMESTRAL_CCOMPLETEDFILTER_H
#define SEMESTRAL_CCOMPLETEDFILTER_H

#include <memory>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include "CFilter.h"
#include "CNote.h"

class CCompletedFilter : public CFilter {
public:
    /**
    * @brief Construct a new CCompletedFilter object
    * 
    */
    CCompletedFilter() = default;

    /**
     * @brief Destroy the CCompletedFilter object
     * 
     */
    ~CCompletedFilter() override = default;

    /**
     * @brief Function to filter notes based on parameter isCompleted
     * @param std::map<std::string, std::shared_ptr<CNote>>  Map of notes that are imported in application
     * @return Returns filtered notes
     */
    [[nodiscard]] std::map <std::string, std::shared_ptr<CNote>>
    Find(const std::map <std::string, std::shared_ptr<CNote>> &) const override;

};


#endif //SEMESTRAL_CCOMPLETEDFILTER_H
