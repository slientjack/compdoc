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

#ifndef CFB_TYPE_DEF_H_
#define CFB_TYPE_DEF_H_

#include <vector>
#include <string>

#define LEN(array)(sizeof(array) / sizeof(array[0]))

namespace cfbf {
    
    const unsigned char kDocumentHeaderSignature[8] = {0xD0, 0xCF, 0x11, 0xE0, 0xA1, 0xB1, 0x1A, 0xE1};

    typedef u_int8_t    Byte;           // 1 Byte
    typedef bool        Boolean;        // 1 Byte
    typedef int8_t      Int8;           // 1 Byte
    typedef u_int8_t    UInt8;          // 1 Byte
    typedef int16_t     Int16;          // 2 Bytes
    typedef u_int16_t   UInt16;         // 2 Bytes
    typedef int32_t     Int32;          // 4 Bytes
    typedef u_int32_t   UInt32;         // 4 Bytes
    typedef int64_t     Int64;          // 8 Bytes
    typedef u_int64_t   UInt64;         // 8 Bytes
    typedef void *      Handle;         // 8 Bytes
    
    typedef struct _GUID {
        UInt32  data1;
        UInt16  data2;
        UInt16  data3;
        UInt8   data4[8];
    } GUID;                             // 16 Bytes
    typedef GUID CLSID;                 // 16 Bytes

    typedef struct _CFBFHeader {    // [offset from start in bytes, length in bytes]
        Byte    ab_sig[8];              // [000H,08] {0xd0, 0xcf, 0x11, 0xe0, 0xa1, 0xb1, 0x1a, 0xe1} for current version,
                                        // was {0x0e, 0x11, 0xfc, 0x0d, 0xd0, 0xcf, 0x11, 0xe0} on old, beta 2 files (late ’92)
                                        // which are also supported by the reference implementation
        CLSID   clsid;                  // [008H,16] class id (set with WriteClassStg, retrieved with GetClassFile/ReadClassStg)
        UInt16  minor_version;          // [018H,02] minor version of the format: 33 is written by reference implementation
        UInt16  major_version;          // [01AH,02] major version of the dll/format: 3 is written by reference implementation
        UInt16  byte_order;             // [01CH,02] 0xFFFE: indicates Intel byte-ordering
        Int16   sector_shift;           // [01EH,02] size of sectors in power-of-two (typically 9, indicating 512-byte sectors)
        Int16   mini_sector_shift;      // [020H,02] size of mini-sectors in power-of-two (typically 6, indicating 64-byte mini-sectors)
        UInt16  reserved;               // [022H,02] reserved, must be zero
        UInt32  reserved1;              // [024H,04] reserved, must be zero
        Int32   sector_dir;             // [028H,04] must be zero for 512-byte sectors, number of SECTs in directory chain for 4 KB sectors
        Int32   sector_fat_count;       // [02CH,04] number of SECTs in the FAT chain
        Int32   sector_dir_start;       // [030H,04] first SECT in the Directory chain
        Int32   signature;              // [034H,04] signature used for transactionin: must be zero. The reference implementation
                                        // does not support transactioning
        Int32   mini_sector_cutoff;     // [038H,04] maximum size for mini-streams: typically 4096 bytes
        Int32   sector_mini_fat_start;  // [03CH,04] first SECT in the mini-FAT chain
        Int32   sector_mini_fat_count;  // [040H,04] number of SECTs in the mini-FAT chain
        Int32   sector_dif_start;       // [044H,04] first SECT in the DIF chain
        Int32   sector_dif_count;       // [048H,04] number of SECTs in the DIF chain
        Int32   sector_fats[109];       // [04CH,436] the SECTs of the first 109 FAT sectors
    } CFBFHeader;

    typedef struct _CFBFDirectoryEntry {    // [offset from start in bytes, length in bytes]
        Byte    name[64];               // [000H,64] 64 bytes. The Element name in Unicode, padded with zeros to
                                        // fill this byte array
        Int16   name_length;            // [040H,02] Length of the Element name in characters, not bytes
        Int8    type;                   // [042H,01] Type of object: value taken from the STGTY enumeration
        Int8    flag;                   // [043H,01] Value taken from DECOLOR enumeration.
        Int32   left_sibling;           // [044H,04] SID of the left-sibling of this entry in the directory tree
        Int32   right_sibling;          // [048H,04] SID of the right-sibling of this entry in the directory tree
        Int32   child;                  // [04CH,04] SID of the child acting as the root of all the children of this
                                        // element (if _mse=STGTY_STORAGE)
        CLSID   clsid;                  // [050H,16] CLSID of this storage (if _mse=STGTY_STORAGE)
        Int32   user_flag;              // [060H,04] User flags of this storage (if _mse=STGTY_STORAGE)
        Byte    time[16];               // [064H,16] Create/Modify time-stamps (if _mse=STGTY_STORAGE)
        Int32   sector_start;           // [074H,04] starting SECT of the stream (if _mse=STGTY_STREAM)
        Int32   size;                   // [078H,04] size of stream in bytes (if _mse=STGTY_STREAM)
        Int32   reserved;               // [07CH,04] Reserved for future use. Must be zero.
    } CFBFDirectoryEntry;

    typedef enum _CFBFSectorStatusCodes {
        kFreeSector     = 0xFFFFFFFF,   // -1
        kEndOfChain     = 0xFFFFFFFE,   // -2
        kFatSector      = 0xFFFFFFFD,   // -3
        kDifSector      = 0xFFFFFFFC,   // -4
        kNaSector       = 0xFFFFFFFB,   // -5
        kMaxRegSector   = 0xFFFFFFFA,   // -6

    } CFBFSectorStatusCodes;

    typedef enum _CFBFDirectoryEntryTypes { 
        kInvalid    = 0,
        kStorage    = 1,
        kStream     = 2,
        kLockBytes  = 3,
        kPropPerty  = 4,
        kRoot       = 5,
    } CFBFDirectoryEntryTypes;

    typedef enum _CFBFDirectoryEntryFlags {
        kRed = 0,
        kBlack = 1,
    } CFBFDirectoryEntryFlags;
}
#endif // CFB_TYPE_DEF_H_
