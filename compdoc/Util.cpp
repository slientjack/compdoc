//
//  Util.cpp
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

#include "Util.h"
#include <codecvt>

using namespace std;
using namespace slient::compdoc;

void Util::getBytes(void *buffer, char *data, int pos, int length, short byteOrder)
{
    if (byteOrder == -2) {
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

vector<string> Util::split(string str, string pattern)
{
    size_t pos;
    vector<string> result;
    str += pattern;
    size_t size = str.size();
    for (size_t i = 0; i < size; ++i) {
        pos = str.find(pattern, i);
        if (pos < size) {
            string s = str.substr(i, pos - i);
            if (s.length() > 0) {
                result.push_back(s);
            }
            i = pos + pattern.size() - 1;
        }
    }
    return result;
}

int Util::compare(string str1, string str2)
{
    if (str1.length() < str2.length()) {
        return -1;
    } else if (str1.length() > str2.length()) {
        return 1;
    } else {
        transform(str1.begin(), str1.end(), str1.begin(), ::toupper);
        transform(str2.begin(), str2.end(), str2.begin(), ::toupper);
        return str1.compare(str2);
    }
}

string Util::toUTF8(u16string str16)
{
    return wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t>{}.to_bytes(str16);
}

string Util::getUnicode16String(char *data, int pos, int length, short byteOrder)
{
    char16_t strData[length / 2];
    memcpy(strData, data + pos, length);
    if (byteOrder != -2) {
        // 字节序不一致，字节反转
        for (int i = 0; i < length / 2; ++i) {
            char16_t c = strData[i];
            strData[i] = (c >> 8) | (c << 8);
        }
    }
    return toUTF8(u16string(strData));
}
