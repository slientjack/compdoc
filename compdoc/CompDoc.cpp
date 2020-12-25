//
//  CompDoc.cpp
//  compdoc
//
//************************************************************************************
//
//  Copyright (c) 2020 slientjack
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.
//
//************************************************************************************

#include "CompDoc.h"
#include "Util.h"

using namespace std;
using namespace slient::compdoc;

CompDoc::CompDoc(const string& filePath, bool create)
{
    printf("CompDoc 构造函数，路径:%s\n", filePath.c_str());
    
    _fileStream.open(filePath, ios::in | ios::binary);
    if (!_fileStream.is_open()) {
        printf("Read Compound Document File Fail\n");
        _status = FileOpenFail;
        return;
    }
    
    // ------------------------------------
    // 1.解析Header
    // ------------------------------------
    char *header = new char[kCompDocHeaderSize];
    _fileStream.read(header, kCompDocHeaderSize);
    if (!(header[0] == (char)0xD0 &&
          header[1] == (char)0xCF &&
          header[2] == (char)0x11 &&
          header[3] == (char)0xE0 &&
          header[4] == (char)0xA1 &&
          header[5] == (char)0xB1 &&
          header[6] == (char)0x1A &&
          header[7] == (char)0xE1)) {
        printf("Not a Compound Document File\n");
        _status = FileInvalid;
        return;
    }
    
    // UID
    memcpy(&_uid, header + 8, 16);
    
    // 字节序(0xFEFF：小端，0xFFFE：大端)
    memcpy(&_byteOrder, header + 28, 2);
    
    // Revision
    getBytes(&_revision, header, 24, 2);
    
    // Version
    getBytes(&_version, header, 26, 2);
    
    // sector的大小（字节）
    unsigned short ssz;
    getBytes(&ssz, header, 30, 2);
    _secSize = 1 << ssz; // 2^ssz
    
    // short-sector的大小（字节）
    unsigned short sssz;
    getBytes(&sssz, header, 32, 2);
    _shortSecSize = 1 << sssz; // 2^sssz
    
    // SAT使用的sector数量
    getBytes(&_satSecNum, header, 44, 4);
    
    // Directory Stream的第一个SecID
    getBytes(&_dirSecID, header, 48, 4);
    
    // 最小的标准Stream大小（字节）
    getBytes(&_minStreamSize, header, 56, 4);
    
    // SSAT的第一个SecID
    getBytes(&_ssatSecID, header, 60, 4);
    
    // SSAT使用的sector数量
    getBytes(&_ssatSecNum, header, 64, 4);
    
    // MSAT的第一个SecID
    getBytes(&_msatSecID, header, 68, 4);
    
    // MSAT使用的sector数量
    getBytes(&_msatSecNum, header, 72, 4);
    
    // ------------------------------------
    // 2.构建MSAT
    // ------------------------------------
    
    size_t msatDataLen = 436 + _msatSecNum * _secSize;
    // header第76到最后的字节
    char *msatData = new char[msatDataLen];
    memcpy(msatData, header + 76, 436);
    // 额外存放在sector的数据
    SEC_ID nextMsatSecID = _msatSecID;
    int offset = 436;
    while (nextMsatSecID != kEndOfChainSecID) {
        char secData[_secSize];
        getSectorBytes(secData, nextMsatSecID);
        memcpy(msatData + offset, secData, _secSize - 4);
        offset += _secSize - 4;
        // 下一个SecID
        getBytes(&nextMsatSecID, secData, _secSize - 4, 4);
    }
    for (int i = 0; i < msatDataLen / 4; ++i) {
        SEC_ID secID;
        getBytes(&secID, msatData, i * 4, 4);
        if (secID != kFreeSecID) {
            _msat.push_back(secID);
        } else {
            break;
        }
    }
    // 释放内存
    delete [] msatData;
    delete [] header;
    
    // ------------------------------------
    // 3.构建SAT
    // ------------------------------------
    
    for (int i = 0; i < _msat.size(); ++i) {
        SEC_ID secID = _msat[i];
        char satData[_secSize];
        getSectorBytes(satData, secID);
        for (int j = 0; j < _secSize; j += 4) {
            SEC_ID satSecID;
            getBytes(&satSecID, satData, j, 4);
            _sat.push_back(satSecID);
        }
    }
    
    // ------------------------------------
    // 4.构建SSAT
    // ------------------------------------
    
    SEC_ID nextSsatSecID = _ssatSecID;
    while (nextSsatSecID != kEndOfChainSecID) {
        char ssatData[_secSize];
        getSectorBytes(ssatData, nextSsatSecID);
        for (int j = 0; j < _secSize; j += 4) {
            SEC_ID ssatSecID;
            getBytes(&ssatSecID, ssatData, j, 4);
            _ssat.push_back(ssatSecID);
        }
        nextSsatSecID = _sat[nextSsatSecID];
    }
    
    // ------------------------------------
    // 5.构建Directory
    // ------------------------------------
    
    SEC_ID nextDirSecID = _dirSecID;
    while (nextDirSecID != kEndOfChainSecID) {
        char dirData[_secSize];
        getSectorBytes(dirData, nextDirSecID);
        for (int j = 0; j < _secSize; j += kCompDocDirEntrySize) {
            char *entryData = dirData + j;
            // 转换成Entry对象
            DirectoryEntry entry(entryData, _byteOrder);
            entry.dirID = (DIR_ID)_directory.size();
            _directory.push_back(entry);
        }
        nextDirSecID = _sat[nextDirSecID];
    }
    
    // ------------------------------------
    // 6.提取Short-Stream Container Stream
    // ------------------------------------
    
    if (_directory.size() > 0) {
        DirectoryEntry& rootEntry = _directory[0];
        if (rootEntry.type == RootStorage) {
            _sscStreamSize = rootEntry.size;
            if (_sscStreamSize > 0) {
                _sscStream = new char[_sscStreamSize];
                getStandardStreamData(_sscStream, rootEntry.secID, _sscStreamSize);
            }
        }
    }
    
    printf("File opened!\n");
    _status = Success;
}

CompDoc::~CompDoc()
{
    printf("CompDoc 析构函数\n");
    _fileStream.close();
    if (_sscStream) {
        delete [] _sscStream;
        _sscStream = NULL;
    }
}

bool CompDoc::isOpen()
{
    return _status == Success;
}

Result CompDoc::getHeader(Header* header)
{
    if (!isOpen()) {
        return _status;
    }
    if (!header) {
        return ParametersInvalid;
    }
    
    header->uid = _uid;
    header->revision = _revision;
    header->version = _version;
    header->byteOrder = _byteOrder;
    header->secSize = _secSize;
    header->shortSecSize = _shortSecSize;
    header->satSecNum = _satSecNum;
    header->dirSecID = _dirSecID;
    header->minStreamSize = _minStreamSize;
    header->ssatSecID = _ssatSecID;
    header->ssatSecNum = _ssatSecNum;
    header->msatSecID = _msatSecID;
    header->msatSecNum = _msatSecNum;
    return Success;
}

Result CompDoc::getDirectory(vector<DirectoryEntry>* directory)
{
    if (!isOpen()) {
        return _status;
    }
    if (!directory) {
        return ParametersInvalid;
    }
    *directory = _directory;
    return Success;
}

Result CompDoc::getEntry(const std::string& path, DirectoryEntry* entry)
{
    if (!isOpen()) {
        return _status;
    }
    if (!entry) {
        return ParametersInvalid;
    }
    
    // TODO:
    DirectoryEntry& rootEntry = _directory[0];
    Storage *rootStorage = (Storage *)entry;
    rootStorage->dirID = rootEntry.dirID;
    rootStorage->parentID = 0;
    rootStorage->name = rootEntry.name;
    rootStorage->createTime = rootEntry.createTime;
    rootStorage->modifyTime = rootEntry.modifyTime;
    getStorageMembers(rootEntry.dirID, rootStorage);
    
    return Success;
}

#pragma mark - Private

void CompDoc::getBytes(void *buffer, char *data, int pos, int length)
{
    Util::getBytes(buffer, data, pos, length, _byteOrder);
}

void CompDoc::getSectorBytes(char *buffer, SEC_ID secID)
{
    SEC_POS secPos = kCompDocHeaderSize + secID * _secSize;
    _fileStream.seekg(secPos);
    _fileStream.read(buffer, _secSize);
}

void CompDoc::getShortSectorBytes(char *buffer, SEC_ID secID)
{
    SEC_POS secPos = secID * _shortSecSize;
    memcpy(buffer, _sscStream + secPos, _shortSecSize);
}

/// 读取标准Stream的数据
/// @param data 数据
/// @param secID SecID
/// @param size 数据长度
void CompDoc::getStandardStreamData(char *data, SEC_ID secID, SEC_SIZE size)
{
    SEC_POS pos = 0;
    while (secID != kEndOfChainSecID) {
        char secData[_secSize];
        getSectorBytes(secData, secID);
        SEC_SIZE len = pos + _secSize > size ? size - pos : _secSize;
        memcpy(data + pos, secData, len);
        secID = _sat[secID];
    }
}

/// 读取Short-Stream的数据
/// @param data 数据
/// @param secID SecID
/// @param size 数据长度
void CompDoc::getShortStreamData(char *data, SEC_ID secID, SEC_SIZE size)
{
    SEC_POS pos = 0;
    while (secID != kEndOfChainSecID) {
        char secData[_shortSecSize];
        getShortSectorBytes(secData, secID);
        SEC_SIZE len = pos + _shortSecSize > size ? size - pos : _shortSecSize;
        memcpy(data + pos, secData, len);
        secID = _ssat[secID];
    }
}

void CompDoc::getStorageMembers(DIR_ID dirID, Storage *storage)
{
    
}
