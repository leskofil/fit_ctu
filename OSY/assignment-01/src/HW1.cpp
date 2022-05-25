#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <climits>
#include <cfloat>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <string>
#include <utility>
#include <vector>
#include <array>
#include <iterator>
#include <set>
#include <list>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <stack>
#include <deque>
#include <memory>
#include <functional>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <stdexcept>
#include <condition_variable>
#include <pthread.h>
#include <semaphore.h>
#include "progtest_solver.h"
#include "sample_tester.h"
using namespace std;
#endif /* __PROGTEST__ */ 


struct SWorkSheet {
    SWorkSheet(size_t i, size_t i1, shared_ptr<CSheet> sharedPtr) : IDLine(i), IDSheet(i1), sheet(std::move(sharedPtr)) {}

    SWorkSheet(size_t i) : IDSheet(i){}

    bool operator<(const SWorkSheet& w) const {
        return this->IDSheet<w.IDSheet;
    }

    bool operator<(size_t w) const {
        return this->IDSheet<w;
    }

    size_t IDLine{};
    size_t IDSheet{};
    ASheet sheet;
};

class SLine {
public:
    explicit SLine(shared_ptr<CProductionLine> sharedPtr) : line(std::move(sharedPtr)){};

    AProductionLine line;
    std::set<SWorkSheet> doneSheets;
};


//-------------------------------------------------------------------------------------------------------------------------------------------------------------

class CQualityControl
{
  public:
    static void                        checkAlgorithm                          ( ASheet                                sheet );
    void                               addLine                                 ( AProductionLine                       line );
    void                               start                                   ( int                                   workThreads );
    void                               stop                                    ( void );
  private:
    std::queue<SWorkSheet>  m_UndoneSheets;
    std::mutex              m_UndoneSheetsMtx;
    std::condition_variable m_UndoneSheetsCV;

    std::vector<SLine>      m_ProdLines;
    std::mutex              m_DoneSheetsMtx;
    std::condition_variable m_DoneSheetsCV;

    std::vector<std::thread> m_Receivers;
    std::vector<std::thread> m_Senders;
    std::vector<std::thread> m_Workers;

    std::atomic<bool> m_rec = false;
    std::atomic<bool> m_comp= false;

    void Receive(size_t id);
    void Work();
    void Send(size_t idLine);
};

void CQualityControl::addLine(AProductionLine line) {
    m_ProdLines.emplace_back(line);
}

void CQualityControl::start(int workThreads) {
    for (size_t i = 0; i < m_ProdLines.size(); i++) {
        //printf("starting receiver for L %zu \n", i);
        m_Receivers.emplace_back(&CQualityControl::Receive, this, i);
    }
    for (int i = 0; i < workThreads; i++) {
        //printf("starting work T%d \n", i);
        m_Workers.emplace_back(&CQualityControl::Work, this);
    }

    for (size_t i = 0; i < m_ProdLines.size(); i++) {
        //printf("starting sender for L%zu \n", i);
        m_Senders.emplace_back(&CQualityControl::Send, this, i);
    }
}

void CQualityControl::stop() {
    //printf("Stop reached\n");

    for (size_t i = 0; i < m_Receivers.size(); i++) {
      //  printf("joining send thread\n");
        m_Receivers[i].join();
    }
    //printf("Receivers stopped\n");

    m_rec = true;
    m_UndoneSheetsCV.notify_all();

    for (size_t i = 0; i < m_Workers.size(); i++)
        m_Workers[i].join();

    //printf("Workers stopped\n");
    m_comp = true;
    m_DoneSheetsCV.notify_all();

    for (size_t i = 0; i < m_Senders.size(); i++) {
        m_Senders[i].join();
    }
    //printf("Senders stopped\n");
//    fflush(stdout);
}

void CQualityControl::Receive(size_t IDLine) {
    size_t IDSheet = 0;
    while (true) {
        auto sheet = m_ProdLines[IDLine].line->getSheet();
        if (sheet) {
      //      printf("line %zu got sheet\n", IDLine);
            auto ws = SWorkSheet(IDLine, IDSheet, sheet);
            IDSheet++;
            {
                std::lock_guard<std::mutex> lg(m_UndoneSheetsMtx);
                m_UndoneSheets.push(ws);
                m_UndoneSheetsCV.notify_all();
            }
        } else{
//            printf("aaaaaaaaaaaaaaaaaaaaaaa\n");
//            fflush(stdout);
            break;
        }
    }
}

void CQualityControl::Work() {
    while (true) {
        std::unique_lock<std::mutex> ul(m_UndoneSheetsMtx);
        m_UndoneSheetsCV.wait(ul, [&] { return !m_UndoneSheets.empty() || m_rec.load(); });

        if (m_rec.load() && m_UndoneSheets.empty()) {
        //    printf("workers stopping\n");
            ul.unlock();
            break;
        }

        auto sheet = m_UndoneSheets.front();
        m_UndoneSheets.pop();
        ul.unlock();

        //printf("sheet S%zu from L%zu picked up to work\n", sheet.IDSheet, sheet.IDLine);
        checkAlgorithm(sheet.sheet);
        //printf("work on sheet S%zu from L%zu done\n", sheet.IDSheet, sheet.IDLine);

        std::lock_guard<std::mutex> lock_guard(m_DoneSheetsMtx);
        m_ProdLines[sheet.IDLine].doneSheets.insert(sheet);
        //printf("sheet S%zu inserted into L%zu\n", sheet.IDSheet, sheet.IDLine);
        m_DoneSheetsCV.notify_all();
    }
}

void CQualityControl::Send(size_t idLine) {
    size_t lastID = 0;
    //printf("Make it to send\n");
    while(true) {
        std::unique_lock<std::mutex> ul (m_DoneSheetsMtx);
        m_DoneSheetsCV.wait(ul, [&] {return m_ProdLines[idLine].doneSheets.find(lastID) != m_ProdLines[idLine].doneSheets.end() || m_comp.load(); });

        //printf("line %zu got notified for send\n", idLine);
        if (m_comp.load() && m_ProdLines[idLine].doneSheets.empty()) {
      //      printf("line L%zu has send everything\n", idLine);
            ul.unlock();
            break;
        }

        auto ds = m_ProdLines[idLine].doneSheets.find(lastID);
        if (ds == m_ProdLines[idLine].doneSheets.end()) {
        //    printf("donesheets size is %zu\n", m_ProdLines[idLine].doneSheets.size());
            ul.unlock();
            continue;
        }

        auto d = ds->sheet;
        //printf("S%zu was send, id is %zu\n", ds->first, lastID);
        //printf("donesheets size is %zu\n", m_ProdLines[idLine].doneSheets.size());
        //fflush(stdout);

        m_ProdLines[idLine].doneSheets.erase(lastID);
        lastID++;

        m_DoneSheetsMtx.unlock();

        m_ProdLines[idLine].line->doneSheet(d);
    }
}

void CQualityControl::checkAlgorithm(ASheet sheet) {
    int **tmp=  new int*[sheet->m_Length];
    for(int i = 0; i < sheet->m_Length; i++){
        tmp[i] = new int[sheet->m_Width];
        for (int j = 0; j < sheet->m_Width; j++)
            tmp[i][j] = sheet->m_Thickness[i* sheet->m_Width + j];
    }
    for (auto &it : sheet->m_RelDev ) {
        it.second = maxRectByRelDev(tmp, sheet->m_Width, sheet->m_Length, it.first);
    }

    for (auto &it : sheet->m_Volume ) {
        it.second = maxRectByVolume(tmp, sheet->m_Width, sheet->m_Length, it.first);
    }

    for (auto &it : sheet->m_MinMax ) {
        it.second = maxRectByMinMax(tmp, sheet->m_Width, sheet->m_Length, it.first.m_Lo, it.first.m_Hi);
    }

    for (int i = 0; i < sheet->m_Length; i++) {
        delete[] tmp[i];
    }
    delete[] tmp;
}


// TODO: CQualityControl implementation goes here
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef __PROGTEST__
int                main                                    ( void )
{
  CQualityControl control;
  AProductionLineTest line = std::make_shared<CProductionLineTest> ();
  control . addLine ( line );
  control . start ( 4 );
  control . stop  ();
  if ( ! line -> allProcessed () )
    throw std::logic_error ( "(some) sheets were not correctly processsed" );
  return 0;  
}
#endif /* __PROGTEST__ */ 
