/*
 * @Author: yejianfei
 * @Date: 2021-02-17 12:24:51
 * @LastEditors: yejianfei
 * @LastEditTime: 2021-02-27 22:38:43
 * @Description: 
 * @Developer: 
 */
#ifndef CFB_H_
#define CFB_H_

#include <vector>
#include <string>
#include "typedef.h"
#include "sat.h"
#include "entry.h"
using namespace std;

namespace cfbf {
    class Document {
        public:
            Document(Byte* buffer, Int32 len);
            CFBFHeader& header();
            Byte* buffer();
            Sector operator[](Int32 index);
            DirectoryEntry operator[](string path);
            Sector GetSector(Int32 index);
            Sector GetShortSector(Int32 index);
            DirectoryEntry Find(std::string path);
        private:
            Byte* buffer_;
            Int32 len_;
            CFBFHeader& header_;
            MastSectorAllocationTable msat_;
            SectorAllocationTable sat_;
            ShortSectorAllocationTable ssat_;
            vector<CFBFDirectoryEntry*> entries_;
    };
}
#endif // CFB_H_
