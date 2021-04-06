// MIT License

// Copyright (c) 2020 slientjack

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <iostream>
#include <sstream>
#include <codecvt>
#include "sat.h"
#include "entry.h"

using namespace std;

namespace cfbf {

string ReadEntryName(Byte name[64], Int16 len) {
    char16_t sName[len / 2];
    memcpy(sName, name, len);
    return wstring_convert< codecvt_utf8_utf16<char16_t>, char16_t >().to_bytes(sName);
}

int CompareEntryName(string a, string b) {
    if (a.length() < b.length()) {
        return -1;
    } else if (a.length() > b.length()) {
        return 1;
    } else {
        transform(a.begin(), a.end(), a.begin(), ::toupper);
        transform(b.begin(), b.end(), b.begin(), ::toupper);
        return a.compare(b);
    }
}

CFBFDirectoryEntry* FindSiblingEntry(vector<CFBFDirectoryEntry*>& entries, CFBFDirectoryEntry* entry, string name) {
    if (name == "") {
        return entries[0];
    }

    string eName = ReadEntryName(entry->name, entry->name_length);
    int s = CompareEntryName(name, eName);
    if (s == 0) {
        return entry;
    } else if (s < 1) {
        return entry->left_sibling != -1 
            ? FindSiblingEntry(entries, entries[entry->left_sibling], name) : nullptr;
    } else {
        return entry->right_sibling != -1 
            ? FindSiblingEntry(entries, entries[entry->right_sibling], name) : nullptr;
    }
}

DirectoryEntry::DirectoryEntry(
    string path,
    CFBFHeader& header,
    vector<CFBFDirectoryEntry*>& entries, 
    SectorAllocationTable& sat, 
    ShortSectorAllocationTable& ssat
): path_(path), header_(header), entries_(entries), sat_(sat), ssat_(ssat) {

    if (path.find_first_of("/") == string::npos) {
        throw "path must be start with /";
    }
    
    vector<string> tokens;
    istringstream input(path);

    for (string name; getline(input, name, '/');) {
        tokens.push_back(name);
    }
    
    for (int i = 0; i < tokens.size(); i++) {
        node_ = FindSiblingEntry(entries_, node_, tokens[i]);
        if (node_ == nullptr || (node_->child == -1 && i < tokens.size() - 1)) {
            return;
        }

        if (i < tokens.size() - 1) {
            node_ = entries_[node_->child];
        }
    }
}

CFBFDirectoryEntry* DirectoryEntry::node() {
    return node_;
}

string DirectoryEntry::name() {
    return ReadEntryName(node_->name, node_->name_length);
}

string DirectoryEntry::path() {
    return path_;
}

Boolean DirectoryEntry::valid() {
    return node_ != nullptr;
}

Boolean DirectoryEntry::root() {
    return node_->type == kRoot;
}

Boolean DirectoryEntry::storage() {
    return node_->type == kStorage;
}

Boolean DirectoryEntry::stream() {
    return node_->type == kStream;
}

Stream DirectoryEntry::OpenStream() {
    return  node_->size < header_.mini_sector_cutoff
        ? Stream(node_->sector_start, node_->size, ssat_)
        : Stream(node_->sector_start, node_->size, sat_);
}

Stream::Stream(Int32 sector_start, Int32 length, AbstractSectorAllocationTable& sat)
    : sector_start_(sector_start), length_(length), sat_(sat)  {

    Int32 pos = 0;
    data_ = new Byte[length_]; 
    for (Sector sector = sat_[sector_start]; sector.index() >= 0 && pos < length_; sector++) {
        Int32 size = pos + sat_.sector_size() <= length ? sat_.sector_size() : length - pos;
        memcpy(data_ + pos, sector.data(), size);
        pos += sat_.sector_size();
    }
}

Stream::~Stream() {
    Close();
}

Int32 Stream::sector_start() {
    return sector_start_;
}

Int16 Stream::sector_size() {
    return sat_.sector_size();
}

Byte* Stream::data() {
    return data_;
}

Int32 Stream::length() {
    return length_;
}

void  Stream::Close() {
    if (data_ != nullptr) {
        delete[] data_;
        data_ = nullptr;
    }
}

}
