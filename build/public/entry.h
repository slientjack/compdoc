/*
 * @Author: yejianfei
 * @Date: 2021-02-21 11:55:33
 * @LastEditors: yejianfei
 * @LastEditTime: 2021-02-21 18:14:58
 * @Description: 
 * @Developer: 
 */
#ifndef CFB_ENTRY_H_
#define CFB_ENTRY_H_
#include "typedef.h"
#include "sat.h"

namespace cfbf {

    class Stream {
        public:
            Stream(Int32 sector_start, Int32 length, AbstractSectorAllocationTable& sat);
            ~Stream();
            Int32 sector_start();
            Int16 sector_size();
            Byte* data();
            Int32 length();
            void Close();
        private:
            Int32 sector_start_;
            Int32 length_;
            AbstractSectorAllocationTable& sat_;
            Byte* data_;
    };

    class DirectoryEntry {
        public:
            DirectoryEntry(
                string path,
                CFBFHeader& header,
                vector<CFBFDirectoryEntry*>& entries, 
                SectorAllocationTable& sat, 
                ShortSectorAllocationTable& ssat
            );
            CFBFDirectoryEntry* node();
            std::string name();
            std::string path();
            Boolean valid();
            Boolean root();
            Boolean storage();
            Boolean stream();
            Stream OpenStream();
        private:
            std::string path_;
            CFBFHeader& header_;
            vector<CFBFDirectoryEntry*>& entries_;
            SectorAllocationTable& sat_;
            ShortSectorAllocationTable& ssat_;
            CFBFDirectoryEntry* node_;
            
    };


}
#endif // CFB_ENTRY_H_
