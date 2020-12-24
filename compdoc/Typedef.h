//
//  Typedef.h
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

#ifndef Typedef_h
#define Typedef_h

#define kCompDocHeaderSize 512      // Header大小，512字节
#define kCompDocDirEntrySize 128    // Directory Entry大小，128字节

#define kFreeSecID -1               // 空闲的SecID
#define kEndOfChainSecID -2         // SecID链的结束ID
#define kSATSecID -3                // SAT自身使用的SecID
#define kMSATSecID -4               // MSAT自身使用的SecID

namespace slient {
namespace compdoc {

typedef int             SEC_ID;
typedef unsigned int    SEC_SIZE;
typedef unsigned int    SEC_POS;
typedef int             DIR_ID;

} // namespace compdoc
} // namespace slient

#endif /* Typedef_h */
