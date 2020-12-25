//
//  DirectoryEntry.cpp
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

#include "DirectoryEntry.h"
#include "Util.h"

using namespace std;
using namespace slient::compdoc;

DirectoryEntry::DirectoryEntry(char *data, short byteOrder)
{
    // 名称
    unsigned short nameLen;
    Util::getBytes(&nameLen, data, 64, 2, byteOrder);
    if (nameLen > 0) {
        name.reserve(nameLen);
        for (int k = 0; k < nameLen; ++k) {
            if (data[k]) {
                name.append(1, data[k]);
            }
        }
    } else {
        name = "";
    }
    // 类型
    type = (EntryType)data[66];
    // 颜色
    isBlack = data[67] == 1;
    // 左子节点DirID
    Util::getBytes(&lChildDirID, data, 68, 4, byteOrder);
    // 右子节点DirID
    Util::getBytes(&rChildDirID, data, 72, 4, byteOrder);
    // 根节点DirID
    Util::getBytes(&rootDirID, data, 76, 4, byteOrder);
    // 创建时间
    unsigned long long cTime;
    Util::getBytes(&cTime, data, 100, 8, byteOrder);
    createTime = cTime / 10000000.0f + kUnixTimeSince1601;
    // 修改时间
    unsigned long long mTime;
    Util::getBytes(&mTime, data, 108, 8, byteOrder);
    modifyTime = mTime / 10000000.0f + kUnixTimeSince1601;
    // SecID
    Util::getBytes(&secID, data, 116, 4, byteOrder);
    // Size
    Util::getBytes(&size, data, 120, 4, byteOrder);
}
