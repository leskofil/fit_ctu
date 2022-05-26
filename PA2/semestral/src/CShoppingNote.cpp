//
// Created by filip on 6/6/21.
//

#include "CShoppingNote.h"

using namespace std;

CShoppingNote::CShoppingNote(std::string name) : CNote(std::move(name)), totalAmount(0), isCompleted(false) {}

void CShoppingNote::Complete() {
    isCompleted = true;
}

bool CShoppingNote::ReadInfo(std::istringstream &iss) {
    string tag;
    if (!(iss >> std::boolalpha >> isCompleted >> totalAmount)) {
        return false;
    }
    while (getline(iss >> ws, tag, ',')) {
        if (m_Tags.find(tag) == m_Tags.end())
            m_Tags.insert(tag);
        else
            throw runtime_error("Invalid config structure.");
    }
    return true;
}

void CShoppingNote::Print(std::ostream &os) const {
    os << m_Name << " " << Category() << " " << std::boolalpha << isCompleted << " " << totalAmount;
    for (const auto &t : m_Tags)
        if (t != *(m_Tags.rbegin()))
            os << " " << t + ",";
        else
            os << " " << t;
}

std::string CShoppingNote::Category() const {
    return "ShoppingNote";
}

bool CShoppingNote::Import(const string &fileName, istream &is, ostream &os) {
    string line;
    string item;
    int amount = 0;
    map<string, int> items;  //name -> amount
    while (getline(is, line)) {
        istringstream iss(line);
        if (!(iss >> amount)) {
            throw runtime_error("Invalid format");
        }
        if (!getline(iss >> ws, item))
            throw runtime_error("Invalid format");
        Rtrim(item);
        auto itemIt = items.find(item);
        if (itemIt != items.end()) {
            itemIt->second += amount;
        } else
            items.emplace(item, amount);
    }
    for (auto const &x : items) {
        os << x.second << "x" << " " << x.first + "\n";
        totalAmount += x.second;
    }
    if (os.fail()) {
        throw runtime_error("Error while reading note");
    }
    return true;
}


bool CShoppingNote::IsCompleted() const {
    return isCompleted;
}

void CShoppingNote::ExportHtml(ostream &os) {
    ifstream ifs("./examples/Notes/" + m_Name + ".note");
    if (ifs.fail())
        throw runtime_error("Can not locate note");

    string line;
    string amount;
    string item;
    os << "<!DOCTYPE html><html><body>\n";
    os << "<h1 style=\"text-align:center;\">" << m_Name << "</h1>\n";
    os << "<h2 style=\"text-align:center;\">" << Category() << "</h2>\n";
    while (getline(ifs, line)) {
        istringstream iss(line);
        if (!(iss >> amount))
            throw runtime_error("Invalid format");
        if (!getline(iss, item))
            throw runtime_error("Invalid format");
        os << "<p><b>" << amount << "</b>" << item << "</p>";
    }
    os << "<p><b>" << "Total Amount: " << totalAmount << "</b></p>" << endl;
    os << "</body></html>";
    if (os.fail()) {
        throw runtime_error("Error writing to file");
    }
}

void CShoppingNote::ExportPlainText(ostream &os) {
    ifstream ifs("./examples/Notes/" + m_Name + ".note");
    if (ifs.fail())
        throw runtime_error("Can not locate note");

    string line;
    os << m_Name << "\n";
    os << ifs.rdbuf() << endl;
    os << "totalAmount " << totalAmount << "\n" << endl;
    if (os.fail()) {
        throw runtime_error("Error writing to file");
    }
}
