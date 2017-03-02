#pragma once

#include "Platform.h"
#include <sys/stat.h>


class filebuffer
{
    vector<uint8_t> _data;
    size_t          _pos;

public:

    filebuffer(const char* filename)
    {
        ifstream stream(filename, ios::binary);
        stream.exceptions(ios::failbit | ios::badbit);

        struct stat results;
        stat(filename, &results);

/*
        stream.seekg(ios::end);
        streampos streamPos = stream.tellg();
        int fileSize = (int)streamPos.seekpos();
        stream.seekg(ios::beg);
*/
size_t fileSize = results.st_size;

        _data.resize(fileSize);
        stream.read((char*)&_data[0], fileSize);
        _pos = 0;
    }

    void
    read(char *buf, size_t count)
    {
        if (count > _data.size() - _pos)
        {
            printf("count(%ld) pos(%ld) size(%ld)\n", count, _pos, _data.size());
            throw std::invalid_argument("filebuffer::read()");
        }
        memcpy(buf, &_data[_pos], count);
        _pos += count;
    }

};