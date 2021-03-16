// MIT License

// Copyright (c) 2020 slientjack

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <iostream>
#include <vector>
#include "document.h"

using namespace std;

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
