//
//  Header.h
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

#ifndef Header_h
#define Header_h

#include <iostream>
#include "Typedef.h"

namespace slient {
namespace compdoc {

class Header
{
public:
    /// 唯一标识
    std::string uid;
    /// 修订版本号
    short revision;
    /// 版本号
    short version;
    /// 字节序，0xFEFF：小端，0xFFFE：大端
    short byteOrder;
    /// sector的大小（字节）
    SEC_SIZE secSize;
    /// short-sector的大小（字节）
    SEC_SIZE shortSecSize;
    /// SAT使用的sector数量
    SEC_SIZE satSecNum;
    /// Directory Stream的第一个SecID
    SEC_ID dirSecID;
    /// 标准Stream的最小大小（字节）
    SEC_SIZE minStreamSize;
    /// SSAT的第一个SecID（-2表示不存在SSAT）
    SEC_ID ssatSecID;
    /// SSAT使用的sector数量
    SEC_SIZE ssatSecNum;
    /// MSAT的第一个SecID（-2表示不需要sector存放）
    SEC_ID msatSecID;
    /// MSAT使用的sector数量
    SEC_SIZE msatSecNum;
};

} // namespace compdoc
} // namespace slient

#endif /* Header_h */
