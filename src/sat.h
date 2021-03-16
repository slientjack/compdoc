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

#ifndef CFB_SAT_H_
#define CFB_SAT_H_
#include "typedef.h"

namespace cfbf {

    class AbstractSectorAllocationTable;
    class Sector {
        public:
            Sector(Int32 index, AbstractSectorAllocationTable& sat);
            Byte* data();
            Int32 index();
            Int32 next();
            Int32 size();
            Sector& operator++ (Int32);
        private:
            Int32 index_;
            Int32 next_;
            AbstractSectorAllocationTable&  sat_;
    };

    class AbstractSectorAllocationTable {
        public:
            AbstractSectorAllocationTable(Byte* buffer, Int32 len, CFBFHeader& header);
            virtual ~AbstractSectorAllocationTable();
            virtual Sector operator[](Int32 index);
            virtual Int32 GetNext(Int32 index) = 0;
            virtual Byte* GetData(Int32 index) = 0;
            virtual Int32 sector_size() = 0;
        protected:
            std::vector<Int32> table_;
            Byte* buffer_;
            Int32 len_;
            CFBFHeader& header_;
    
    };

    class MastSectorAllocationTable: public AbstractSectorAllocationTable {
        public:
            MastSectorAllocationTable(Byte* buffer, Int32 len, CFBFHeader& header);
            virtual ~MastSectorAllocationTable();
            virtual Int32 GetNext(Int32 index);
            virtual Byte* GetData(Int32 index);
            virtual Int32 sector_size();
    };

    class SectorAllocationTable: public AbstractSectorAllocationTable {
        public:
            SectorAllocationTable(Byte* buffer, Int32 len, CFBFHeader& header, MastSectorAllocationTable& msat);
            virtual ~SectorAllocationTable();
            virtual Int32 GetNext(Int32 index);
            virtual Byte* GetData(Int32 index);
            virtual Int32 sector_size();
        private:
            MastSectorAllocationTable& msat_;
    };


    class ShortSectorAllocationTable: public AbstractSectorAllocationTable {
        public:
            ShortSectorAllocationTable(Byte* buffer, Int32 len, CFBFHeader& header, SectorAllocationTable& sat);
            virtual ~ShortSectorAllocationTable();
            virtual Int32 GetNext(Int32 index);
            virtual Byte* GetData(Int32 index);
            virtual Int32 sector_size();
        private:
            SectorAllocationTable& sat_;
            CFBFDirectoryEntry* root_;
    };


}
#endif // CFB_SAT_H_
