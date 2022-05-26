//
// Created by filip on 6/6/21.
//



#include "CToDoNote.h"

using namespace std;

CToDoNote::CToDoNote(std::string name) : CNote(std::move(name)), isCompleted(false), totalTasks(0), completedTasks(0) {}

void CToDoNote::Complete() {
    isCompleted = true;
}

void CToDoNote::AddCompletedTask(int tasks) {
    completedTasks = tasks;
}

bool CToDoNote::ReadInfo(std::istringstream &iss) {
    string tag;
    if (!(iss >> std::boolalpha >> isCompleted >> totalTasks >> completedTasks) || totalTasks < completedTasks) {
        throw runtime_error("Invalid config structure.");
    }
    while (getline(iss >> ws, tag, ',')) {
        if (m_Tags.find(tag) == m_Tags.end())
            m_Tags.insert(tag);
        else
            throw runtime_error("Invalid config structure.");
    }
    return true;
}

void CToDoNote::Print(std::ostream &os) const {
    os << m_Name << " " << Category() << " " << isCompleted << " " << totalTasks << " " << completedTasks;
    for (const auto &t : m_Tags)
        if (t != *(m_Tags.rbegin()))
            os << " " << t + ",";
        else
            os << " " << t;
}

std::string CToDoNote::Category() const {
    return "ToDoNote";
}

bool CToDoNote::Import(const string &fileName, istream &is, ostream &os) {
    std::set <std::string> days = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
    string line;
    string todo;
    string day;
    size_t tasks = 0;
    while (getline(is, line)) {
        istringstream iss(line);
        if (!(iss >> day))
            throw runtime_error("Invalid format");
        if (!getline(iss, todo))
            throw runtime_error("Invalid format");
        Rtrim(todo);
        if (days.find(day) == days.end())
            throw runtime_error("Invalid format");
        os << day << " " << todo << endl;

        tasks++;
    }
    if (os.fail()) {
        throw runtime_error("Error while reading note");
    }
    totalTasks = tasks;
    return true;
}


bool CToDoNote::IsCompleted() const {
    return isCompleted;
}

void CToDoNote::ExportHtml(ostream &os) {
    string line;
    string day;
    string todo;
    ifstream ifs("./examples/Notes/" + m_Name + ".note");
    if (ifs.fail())
        throw runtime_error("Can not locate note");
    os << "<!DOCTYPE html><html><head></head><body>\n";
    os << "<h1 style=\"text-align:center;\">" << m_Name << "</h1>\n";
    os << "<h2 style=\"text-align:center;\">" << Category() << "</h2>\n";
    while (getline(ifs, line)) {
        if (line.empty())
            continue;
        istringstream iss(line);
        if (!(iss >> day))
            throw runtime_error("Invalid format");
        if (!getline(iss, todo))
            throw runtime_error("Invalid format");
        os << "<p><b>" << day << "</b>" << todo << "</p>";
    }
    os << "<p><b>" << completedTasks << "/" << totalTasks << "</b></p>";
    os << "</body></html>";
    if (os.fail())
        throw runtime_error("Error writing to file");
}

void CToDoNote::ExportPlainText(ostream &os) {
    ifstream ifs("./examples/Notes/" + m_Name + ".note");
    if (ifs.fail())
        throw runtime_error("Can not locate note");

    string line;
    string day;
    os << m_Name << "\n";
    os << ifs.rdbuf() << endl;
    os << completedTasks << "/" << totalTasks << endl;
    if (os.fail()) {
        throw runtime_error("Error writing to file");
    }
}
