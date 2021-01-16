//
//  main.cpp
//  compdoc
//
//  Created by jack on 2020/12/24.
//

#include <iostream>
#include "../compdoc/CompDoc.h"
#include "../compdoc/Util.h"

using namespace std;
using namespace slient::compdoc;

void printData(char *data, int size) {
    for (int i = 0; i < size; ++i) {
        if (i % 32 == 0) {
            if (i > 0) {
                printf("\n");
            }
            printf("[%d] ", i);
        }
        printf("%02x ", (unsigned char)data[i]);
    }
    printf("\n-----------------------------\n");
}

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
    
    // 读取Storage
    Storage storage;
    doc.getEntry("/", &storage);
    
    // 读取Stream
    Stream stream;
    doc.getEntry("/WordDocument", &stream);
    //doc.getEntry("/0Table", &stream);
    
    // 读取Stream的字节流
    char data[stream.size];
    doc.read(&stream, data);
    printData(data, stream.size);
    
//    // fibRgFcLcb
//    char fibRgFcLcb[1312];
//    memcpy(fibRgFcLcb, data + 154, 1312);
//    printData(fibRgFcLcb, 1312);
//
//    unsigned int fcPlcfBtePapx;
//    memcpy(&fcPlcfBtePapx, fibRgFcLcb + 26 * 4, 4);
//    unsigned int lcbPlcfBtePapx;
//    memcpy(&lcbPlcfBtePapx, fibRgFcLcb + 27 * 4, 4);
    
//    // plcbtePapx
//    char plcbtePapx[20];
//    memcpy(plcbtePapx, data + 1318, 20);
//    printData(plcbtePapx, 20);
    
    // PapxFkp
    char papxFkp[512];
    memcpy(papxFkp, data+4096, 512);
    printData(papxFkp, 512);
    
    // 读取文字
    printf("%s", Util::getUnicode16String(data, 2048, 378 * 2).c_str());
    printf("\n");
    
    return 0;
}
