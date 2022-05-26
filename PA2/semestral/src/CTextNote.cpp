//
// Created by filip on 6/6/21.
//

#include "CTextNote.h"


using namespace std;

CTextNote::CTextNote(std::string name) : CNote(std::move(name)) {}

bool CTextNote::ReadInfo(std::istringstream &iss) {
    string tag;
    while (getline(iss >> ws, tag, ',')) {
        if (m_Tags.find(tag) == m_Tags.end())
            m_Tags.insert(tag);
        else
            throw runtime_error("Invalid config structure.");
    }
    return true;
}

void CTextNote::Print(std::ostream &os) const {
    os << m_Name << " " << Category();
    for (const auto &t : m_Tags)
        if (t != *(m_Tags.rbegin()))
            os << " " << t + ",";
        else
            os << " " << t;
}

std::string CTextNote::Category() const {
    return "TextNote";
}

bool CTextNote::Import(const std::string &fileName, std::istream &ifs, std::ostream &ofs) {
    string line;
    ofs << ifs.rdbuf();
    if (ifs.fail() || ofs.fail()) {
        throw runtime_error("Error while reading note");
    }
    return true;
}

bool CTextNote::IsCompleted() const {
    return false;
}

void CTextNote::Complete() {
    cout << "Text note can not be completed" << endl;
}

void CTextNote::ExportHtml(ostream &os) {
    ifstream ifs("./examples/Notes/" + m_Name + ".note");
    if (ifs.fail())
        throw runtime_error("Can not locate note");
    os << "<!DOCTYPE html><html><body>\n";
    os << "<h1 style=\"text-align:center;\">" << m_Name << "</h1>\n";
    os << "<h2 style=\"text-align:center;\">" << Category() << "</h2>\n";
    os << "<p>" << ifs.rdbuf() << "</p>\n";
    os << "</body></html>";
}

void CTextNote::ExportPlainText(ostream &os) {
    ifstream ifs("./examples/Notes/" + m_Name + ".note");
    if (ifs.fail())
        throw runtime_error("Can not locate note");

    os << m_Name << "\n";
    os << ifs.rdbuf();
    if (os.fail()) {
        throw runtime_error("Error writing to file");
    }
}

