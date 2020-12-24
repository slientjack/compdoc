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

using namespace std;
using namespace slient::compdoc;

CompDoc::CompDoc(const string& filePath, bool create)
{
    printf("CompDoc 构造函数，路径:%s\n", filePath.c_str());
    
    _fileStream.open(filePath, ios::in | ios::binary);
    if (!_fileStream.is_open()) {
        printf("Read Compound Document File Fail\n");
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
        return;
    }
    
    // 字节序(0xFEFF：小端，0xFFFE：大端)
    memcpy(&_byteOrderFlag, header + 28, 2);
    
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
    // 2.构建SAT
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
    
    printf("end\n");
}

CompDoc::~CompDoc()
{
    printf("CompDoc 析构函数\n");
    _fileStream.close();
}

void CompDoc::getBytes(void *buffer, char *data, int pos, int length)
{
    if (_byteOrderFlag == -2) {
        // 字节序一致
        memcpy(buffer, data + pos, length);
    } else {
        // 字节序不一致
        char *p = (char *)buffer;
        for (int i = pos + length - 1; i >= pos; --i) {
            *p = data[i];
            ++p;
        }
    }
}

void CompDoc::getSectorBytes(char *buffer, SEC_ID secID)
{
    SEC_POS secPos = kCompDocHeaderSize + secID * _secSize;
    _fileStream.seekg(secPos);
    _fileStream.read(buffer, _secSize);
}

void CompDoc::getShortSectorBytes(char *buffer, SEC_ID secID)
{
    // TODO:
}
