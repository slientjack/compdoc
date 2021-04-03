/*
 * @Author: yejianfei
 * @Date: 2021-02-14 14:20:44
 * @LastEditors: yejianfei
 * @LastEditTime: 2021-02-21 22:22:02
 * @Description: 
 * @Developer: 
 */
#include <iostream>
#include <fstream>
#include "../src/document.h"

using namespace std;
using namespace cfbf;

struct B {
    Byte data[110];
};

int main(int argc, const char * argv[]) {
    //ifstream file("/Users/yejianfei/Downloads/兼职外包服务协议模板1.doc", ios_base::in | ios_base::binary);
    ifstream file("/Users/yejianfei/Downloads/[MS-CFB]-180912.doc", ios_base::in | ios_base::binary);
    file.seekg(0,ios::end);
    
    UInt32 len = file.tellg();
    Byte* buffer = new Byte[len];
    file.seekg(0,ios::beg);
    file.read(reinterpret_cast<char*>(buffer),len);
    

    Document document(buffer, len);
    //Sector b = document[14092];
    DirectoryEntry word = document["/WordDocument"];
    DirectoryEntry com = document[string("/\x01") +  string("CompObj")];
    // for (Sector b = document[15260]; b.index() >= 0; b++) {
    //     cout << b.index() << endl;
    // }

    if (com.valid()) {
        Stream stream = com.OpenStream();
        B* b2 = reinterpret_cast<B*>(stream.data());
        stream.Close();
    }

    if (word.valid()) {
        Stream stream = word.OpenStream();
        B* b3 = reinterpret_cast<B*>(stream.data());
        stream.Close();
    }
    
    cout << "ddd" << endl;

    delete[] buffer;
    file.close();
}