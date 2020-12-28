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
#define kUnixTimeSince1601 -11644502400 // 1601-01-01 00:00:00的Unix时间戳

#define kFreeSecID -1               // 空闲的SecID
#define kEndOfChainSecID -2         // SecID链的结束ID
#define kSATSecID -3                // SAT自身使用的SecID
#define kMSATSecID -4               // MSAT自身使用的SecID

#define kNoneDirID -1               // 不存在的DirID

namespace slient {
namespace compdoc {

typedef int             SEC_ID;
typedef unsigned int    SEC_SIZE;
typedef unsigned int    SEC_POS;
typedef int             DIR_ID;

/// 返回结果
typedef enum {
    Success = 0,        // 成功
    FileOpenFail,       // 文件打开失败
    FileInvalid,        // 文件无效
    ParametersInvalid,  // 参数无效
    EntryNotExists,     // Entry不存在
} Result;

/// Entry类型
typedef enum {
    EmptyEntry =    0x00,   // 空
    UserStorage =   0x01,   // 用户Storage
    UserStream =    0x02,   // 用户Stream
    LockBytes =     0x03,   // 未知
    Property =      0x04,   // 未知
    RootStorage =   0x05,   // 根Storage
} EntryType;

} // namespace compdoc
} // namespace slient

#endif /* Typedef_h */
