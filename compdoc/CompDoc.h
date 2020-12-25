//
//  CompDoc.h
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

#ifndef CompDoc_h
#define CompDoc_h

#include <iostream>
#include <fstream>
#include <vector>
#include "Typedef.h"
#include "Header.h"
#include "DirectoryEntry.h"

namespace slient {
namespace compdoc {

class CompDoc
{
public:
    /// 构造函数
    /// @param filePath 文件路径
    /// @param create 文件不存在时是否创建，默认false
    CompDoc(const std::string& filePath, bool create = false);
    
    /// 析构函数
    ~CompDoc();
    
    /// 文档是否已打开
    bool isOpen();
    
    /// 获取文件头
    /// @param header 文件头信息 [output]
    Result getHeader(Header* header);
    
    /// 获取Directory
    /// @param directory Directory [output]
    Result getDirectory(std::vector<DirectoryEntry>* directory);

private:
    /// 状态
    Result _status;
    /// 文件流
    std::fstream _fileStream;
    /// 文档唯一标识
    std::string _uid;
    /// 修订版本号
    short _revision;
    /// 版本号
    short _version;
    /// 字节序标记，0xFEFF：小端，0xFFFE：大端
    short _byteOrder;
    /// sector的大小（字节）
    SEC_SIZE _secSize;
    /// short-sector的大小（字节）
    SEC_SIZE _shortSecSize;
    /// SAT使用的sector数量
    SEC_SIZE _satSecNum;
    /// Directory Stream的第一个SecID
    SEC_ID _dirSecID;
    /// 标准Stream的最小大小（字节）
    SEC_SIZE _minStreamSize;
    /// SSAT的第一个SecID（-2表示不存在SSAT）
    SEC_ID _ssatSecID;
    /// SSAT使用的sector数量
    SEC_SIZE _ssatSecNum;
    /// MSAT的第一个SecID（-2表示不需要sector存放）
    SEC_ID _msatSecID;
    /// MSAT使用的sector数量
    SEC_SIZE _msatSecNum;
    /// MSAT
    std::vector<SEC_ID> _msat;
    /// SAT
    std::vector<SEC_ID> _sat;
    /// SSAT
    std::vector<SEC_ID> _ssat;
    /// Directory
    std::vector<DirectoryEntry> _directory;
    /// Short-Stream Container Stream
    char *_sscStream;
    /// Short-Stream Container Stream的长度
    SEC_SIZE _sscStreamSize;
    
    /// 读取字节
    /// @param buffer 缓存区
    /// @param data 数据
    /// @param pos 起始字节位置
    /// @param length 字节长度
    inline void getBytes(void *buffer, char *data, int pos, int length);
    
    /// 读取sector的字节
    /// @param buffer 缓存区
    /// @param secID SecID
    void getSectorBytes(char *buffer, SEC_ID secID);
    
    /// 读取short-sector的字节
    /// @param buffer 缓存区
    /// @param secID SecID
    void getShortSectorBytes(char *buffer, SEC_ID secID);
    
    /// 读取标准Stream的数据
    /// @param data 数据
    /// @param secID SecID
    /// @param size 数据长度
    void getStandardStreamData(char *data, SEC_ID secID, SEC_SIZE size);
    
    /// 读取Short-Stream的数据
    /// @param data 数据
    /// @param secID SecID
    /// @param size 数据长度
    void getShortStreamData(char *data, SEC_ID secID, SEC_SIZE size);
};

} // namespace compdoc
} // namespace slient

#endif /* CompDoc_h */
