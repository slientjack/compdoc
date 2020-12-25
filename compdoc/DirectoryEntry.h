//
//  DirectoryEntry.h
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

#ifndef DirectoryEntry_h
#define DirectoryEntry_h

#include <iostream>
#include "Typedef.h"

namespace slient {
namespace compdoc {

class DirectoryEntry
{
public:
    /// 名称
    std::string name;
    /// ID
    DIR_ID dirID;
    /// 类型
    EntryType type;
    /// 红/黑
    bool isBlack;
    /// 左子节点的DirID（-1表示没有）
    DIR_ID lChildDirID;
    /// 右子节点的DirID（-1表示没有）
    DIR_ID rChildDirID;
    /// 根节点的DirID（storage才有，否则-1）
    DIR_ID rootDirID;
    /// 节点创建时间
    double createTime;
    /// 节点修改时间
    double modifyTime;
    /// stream：第一个sector/short-sector的ID，root storage：short-stream container stream的第一个secID
    SEC_ID secID;
    /// stream: stream所占的总字节，root storage：short-stream container stream所占的总字节
    SEC_SIZE size;
    
    /// 默认构造函数
    DirectoryEntry() {};
    
    /// 构造函数
    /// @param data 数据
    /// @param byteOrder 字节序
    DirectoryEntry(char *data, short byteOrder);
};

} // namespace compdoc
} // namespace slient

#endif /* DirectoryEntry_h */
