/*
  PString.h - Lightweight printable string class
  Copyright (c) 2009-2012 Mikal Hart.  All right reserved.

  XPString.h (modifications)
  Copyright (c) 2016 Federico Tedin

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef XPString_h
#define XPString_h

#include "Print.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

class XParts {
private:
    char *_pch;
    const char *_dels;
    char *_save;
public:
    XParts(char *buf, const char *dels) : _dels(dels), _save(NULL)
    {
        _pch = strtok_r(buf, _dels, &_save);
    }
    bool has_next();
    char *get_next();
};

class XPString : public Print
{
private:
    char *_buf, *_cur;
    size_t _size;

public:
    virtual size_t write(uint8_t);

    // Basic constructor requires a preallocated buffer
    XPString(char *buf, size_t size) : _buf(buf), _size(size)
    {
        reset();
    }

    // templated constructors allow inline renderings of this type: XPString(buf, size, myfloat[, modifier]);
    template<class T> XPString(char *buf, size_t size, T arg) : _buf(buf), _size(size)
    {
        reset();
        print(arg);
    }

    template<class T> XPString(char *buf, size_t size, T arg, int modifier) : _buf(buf), _size(size)
    {
        reset();
        print(arg, modifier);
    }

    // returns the length of the current string, not counting the 0 terminator
    inline const size_t length()
    {
        return _cur - _buf;
    }

    // returns the capacity of the string
    inline const size_t capacity()
    {
        return _size;
    }

    // gives access to the internal string
    inline operator const char *()
    {
        return c_str();
    }

    inline const char *c_str()
    {
        return _buf;
    }

    // compare to another string
    bool operator==(const char *str)
    {
        return _size > 0 && !strcmp(_buf, str);
    }

    // call this to re-use an existing string
    void reset();

    // This function allows assignment to an arbitrary scalar value like str = myfloat;
    template<class T> inline XPString &operator =(T arg)
    {
        reset();
        print(arg);
        return *this;
    }

    // Concatenation str += myfloat;
    template<class T> inline XPString &operator +=(T arg)
    {
        print(arg);
        return *this;
    }

    // Safe access to sprintf-like formatting, e.g. str.format("Hi, my name is %s and I'm %d years old", name, age);
    int format(char *str, ...);

    void append(XPString other);

    void remove(unsigned index);
    void remove(unsigned index, unsigned count);

    bool starts_with(const char *str);
    bool ends_with(const char *str);
    int index_of(const char *str);
    void reverse();
    XParts split(const char *dels);
};

#endif /* XPString_h */
