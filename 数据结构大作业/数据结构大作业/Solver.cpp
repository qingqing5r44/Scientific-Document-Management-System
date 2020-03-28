#include "pch.h"
#include "Solver.h"
#include "xmlhelper.h"
#include "CommUtils.h"

#include <fstream>

F3Solver::F3Solver(const TCHAR* xmlfile)
{
    // Ĭ��ֻ����article
    ImportDataWrapper(xmlfile, this);
}

F3Solver::F3Solver(const TCHAR* xmlfile, DWORD parseInfo)
{
    ImportDataWrapperEx(xmlfile, this, parseInfo);
}

void F3Solver::ExportToFile(const TCHAR* filename)
{
    // 1936-2020 868.157s 6G
    std::vector<STR> a = ImportData.f3_pFrequencyRanking->GetYears();
    wofstream ofs;
    ofs.open(filename, ios::out);

    for (auto i : a) {
        ofs << (wchar_t*)i << L"\n";
        for (auto j : ImportData.f3_pFrequencyRanking->Get(i)) {
            ofs << (wchar_t*)j.first <<L"\t"<< j.second << endl;
        }
        ofs << L"---" <<endl;
    }

    ofs.close();
}

std::map < STR, std::map<STR, ULONG64> > F3Solver::ImportFromFile(const TCHAR* filename)
{
    wifstream ifs;
    std::map < STR, std::map<STR, ULONG64> > result;
    ifs.open(filename, ios::in);
    wchar_t buffer[0x200]{0};
    while (ifs >> buffer) {
        STR year(buffer);
        std::map<STR, ULONG64> result_peryear;
        while(STR(buffer)!=STR()) {
            ifs >> buffer;
            STR word(buffer);
            if (STR(buffer) == STR(L"---")) {
                break;
            }
            ULONG64 times;
            ifs >> times;
            result_peryear.insert(std::make_pair(word,times));
        }
        result.insert(std::make_pair(year, result_peryear));
    }
    ifs.close();
    return result;
}

void F3Solver::InitMemory() {
    ImportData.f3_pFrequencyRanking = new FrequencyRanking(ignoresWords());
}

void F3Solver::InsertObject(Info temp)
{
    ImportData.f3_pFrequencyRanking->Insert(temp);
}

