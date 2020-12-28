//
//  Util.h
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

#ifndef Util_h
#define Util_h

#include <iostream>
#include <vector>

namespace slient {
namespace compdoc {

class Util
{
public:
    /// 读取字节
    /// @param buffer 缓存区
    /// @param data 数据
    /// @param pos 起始字节位置
    /// @param length 字节长度
    static void getBytes(void *buffer, char *data, int pos, int length, short byteOrder);
    
    /// 分割字符串
    /// @param str 字符串
    /// @param pattern 分隔符
    static std::vector<std::string> split(std::string str, std::string pattern);
    
    /// 字符串比较大小
    /// @param str1 字符串1
    /// @param str2 字符串2
    static int compare(std::string str1, std::string str2);
};

} // namespace compdoc
} // namespace slient

#endif /* Util_h */
