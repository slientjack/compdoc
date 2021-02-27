/*
 * @Author: yejianfei
 * @Date: 2021-02-21 11:47:27
 * @LastEditors: yejianfei
 * @LastEditTime: 2021-02-21 22:19:21
 * @Description: 
 * @Developer: 
 */
#include <iostream>
#include "sat.h"



namespace cfbf {

// struct A {
//     Int32 data[128];
// };

AbstractSectorAllocationTable::AbstractSectorAllocationTable(Byte* buffer, Int32 len, CFBFHeader& header):
    buffer_(buffer), 
    len_(len), 
    header_(header) {
}
AbstractSectorAllocationTable::~AbstractSectorAllocationTable() {}

Sector AbstractSectorAllocationTable::operator[](Int32 index) {
    return Sector(index, *this);
}

MastSectorAllocationTable::MastSectorAllocationTable(Byte* buffer, Int32 len, CFBFHeader& header):
    AbstractSectorAllocationTable(buffer, len, header) {    
}

Int32 MastSectorAllocationTable::GetNext(Int32 index) {
    if (index < 0) {
        return index;
    }

    return index + 1;
}

Byte* MastSectorAllocationTable::GetData(Int32 index) {
    if (index < 0) {
        return nullptr;
    }

    Int32 sec_id = kEndOfChain;
    if (index < 109) {
        sec_id =  header_.sector_fats[index];
    } else {
        int count = sector_size() / sizeof(Int32);
        int seeks = (index - 109) / count;
        
        sec_id = header_.sector_dif_start;
        for (int i = 0; i < seeks; i++) {
            int offset = sizeof(header_) + (sec_id * sector_size());
            sec_id = reinterpret_cast<Int32*>(&buffer_[offset])[count - 1];
        }

        Int32* sector = reinterpret_cast<Int32*>(&buffer_[sizeof(header_) + (sec_id * sector_size())]);
        sec_id = sector[(index - 109) % (count - 1)];
    }

    if (sec_id < 0) {
        return nullptr;
    }

    return &buffer_[sizeof(header_) + (sec_id * sector_size())];
}

Int32 MastSectorAllocationTable::sector_size() {
    return 1 << header_.sector_shift;
}

MastSectorAllocationTable::~MastSectorAllocationTable() {}

SectorAllocationTable::SectorAllocationTable(Byte* buffer, Int32 len, CFBFHeader& header, MastSectorAllocationTable& msat): 
    AbstractSectorAllocationTable(buffer, len, header), msat_(msat)  {
}

SectorAllocationTable::~SectorAllocationTable() {}

Int32 SectorAllocationTable::GetNext(Int32 index) {
    if (index < 0) {
        return index;
    }

    Int32* sat = reinterpret_cast<Int32*>(msat_.GetData(index / (sector_size() / sizeof(Int32))));    
    if (sat == nullptr) {
        return kEndOfChain;
    }

    int offset = index % (sector_size() / sizeof(Int32));
    return sat[offset];
}

Byte* SectorAllocationTable::GetData(Int32 index) {
    if (index < 0) {
        return nullptr;
    }

    int offset = sizeof(header_);
    return &buffer_[offset + (index * sector_size())];
}

Int32 SectorAllocationTable::sector_size() {
    return 1 << header_.sector_shift;
}

ShortSectorAllocationTable::ShortSectorAllocationTable(Byte* buffer, Int32 len, CFBFHeader& header, SectorAllocationTable& sat):
    AbstractSectorAllocationTable(buffer, len, header), sat_(sat) {
    
    Int32* sector = reinterpret_cast<Int32*>(sat_.GetData(header_.sector_mini_fat_start));
    for (int i = 0; i < sat_.sector_size() / sizeof(Int32); i++) {
        table_.push_back(sector[i]);
    }

    root_ = reinterpret_cast<CFBFDirectoryEntry*>(sat_.GetData(header_.sector_dir_start));    
}

ShortSectorAllocationTable::~ShortSectorAllocationTable() {}

Int32 ShortSectorAllocationTable::GetNext(Int32 index) {
    return index < 0 ? index : table_[index];
}

Byte* ShortSectorAllocationTable::GetData(Int32 index) {
    if (index < 0) {
        return nullptr;
    }

    int offset = sizeof(header_) + (root_->sector_start * sat_.sector_size());
    return &buffer_[offset + (index * sector_size())];
}

Int32 ShortSectorAllocationTable::sector_size() {
    return 1 << header_.mini_sector_shift;
}


Sector::Sector(Int32 index, AbstractSectorAllocationTable& sat):
    index_(index), next_(kEndOfChain), sat_(sat) {
    next_ = sat.GetNext(index_);
}

Byte* Sector::data() {
    return sat_.GetData(index_);
}

Int32 Sector::index() {
    return index_;
}

Int32 Sector::next() {
    return next_;
}

Int32 Sector::size() {
    return sat_.sector_size();
}

Sector& Sector::operator++ (Int32) {
    index_ = next();
    next_ = sat_.GetNext(index_);
    return *this;
}

}