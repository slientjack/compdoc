//
//  main.cpp
//  compdoc
//
//  Created by jack on 2020/12/24.
//

#include <iostream>
#include "../compdoc/CompDoc.h"

using namespace std;
using namespace slient::compdoc;

int main(int argc, const char * argv[]) {
    // 打开文件
    CompDoc doc("/Users/jack/code/cpp/demo.doc");
    if (!doc.isOpen()) {
        printf("打开文档失败\n");
    }
    
    // 读取文档头信息
    Header header;
    Result res = doc.getHeader(&header);
    if (res == Success) {
        printf("文档sector大小：%dbyte\n", header.secSize);
    }
    
    // 读取Directory
    vector<DirectoryEntry> directory;
    res = doc.getDirectory(&directory);
    if (res == Success) {
        printf("Directory大小：%lu\n", directory.size());
    }
    
    Storage storage;
    doc.getEntry("/", &storage);
    
    return 0;
}
