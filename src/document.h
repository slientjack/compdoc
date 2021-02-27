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
