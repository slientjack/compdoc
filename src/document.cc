/*
 * @Author: yejianfei
 * @Date: 2021-02-17 12:24:51
 * @LastEditors: yejianfei
 * @LastEditTime: 2021-02-27 22:38:23
 * @Description: 
 * @Developer: 
 */
#include <iostream>
#include <vector>
#include "document.h"
namespace cfbf {


Document::Document(Byte* buffer, Int32 len): 
    buffer_(buffer), 
    len_(len), 
    header_(reinterpret_cast<CFBFHeader&>(*buffer_)),
    msat_(buffer, len, header_),
    sat_(buffer, len, header_, msat_),
    ssat_(buffer, len, header_, sat_) {
    if (memcmp(header().ab_sig, kDocumentHeaderSignature, sizeof(kDocumentHeaderSignature)) != 0) {
        throw "It's not mscfb file";
    }

    for (Sector sector = (*this)[header_.sector_dir_start]; sector.index() >= 0; sector++) {
        CFBFDirectoryEntry* dirs = reinterpret_cast<CFBFDirectoryEntry*>(sector.data());        
        for(int i = 0; i < sat_.sector_size()  / sizeof(CFBFDirectoryEntry); i++) {
            entries_.push_back(&dirs[i]);
        }
    }
}

CFBFHeader& Document::header() {
    return header_;
}

Sector Document::operator[](Int32 index) {
    return sat_[index];
}

DirectoryEntry Document::operator[](string path) {
    return DirectoryEntry(path, header_, entries_, sat_, ssat_);
}

Sector Document::GetSector(Int32 index) {
    return sat_[index];
}

Sector Document::GetShortSector(Int32 index) {
    return ssat_[index];
}

DirectoryEntry Document::Find(std::string path) {
    return DirectoryEntry(path, header_, entries_, sat_, ssat_);
}

}
