/*
 * @Author: yejianfei
 * @Date: 2021-02-21 11:55:33
 * @LastEditors: yejianfei
 * @LastEditTime: 2021-02-27 23:01:41
 * @Description: 
 * @Developer: 
 */
#ifndef CFB_ENTRY_H_
#define CFB_ENTRY_H_
#include "typedef.h"
#include "sat.h"
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
