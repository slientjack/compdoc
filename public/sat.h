/*
 * @Author: yejianfei
 * @Date: 2021-02-21 11:45:49
 * @LastEditors: yejianfei
 * @LastEditTime: 2021-02-21 18:59:14
 * @Description: 
 * @Developer: 
 */
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
            vector<Int32> table_;
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