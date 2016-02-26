/*
  PString.cpp - Lightweight printable string class
  Copyright (c) 2009-2012 Mikal Hart.  All right reserved.

  XPString.cpp (modifications)
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

#include "XPString.h"

bool XParts::has_next()
{
    return _pch != NULL;
}

char *XParts::get_next()
{
    char *tmp = _pch;
    _pch = strtok_r(NULL, _dels, &_save);
    return tmp;
}

XParts XPString::split(const char *dels)
{
    return XParts(_buf, dels);
}

void XPString::reset()
{
    _cur = _buf;
    if (_size > 0)
    {
        _buf[0] = '\0';
    }
}

size_t XPString::write(uint8_t b)
{
    if (_cur + 1 < _buf + _size)
    {
        *_cur++ = (char)b;
        *_cur = '\0';
		return 1;
	}

	return 0;
}

int XPString::format(char *str, ...)
{
    va_list argptr;
    va_start(argptr, str);
    int ret = vsnprintf(_cur, _size - (_cur - _buf), str, argptr);
    if (_size)
    {
        while (*_cur)
        {
            ++_cur;
        }
    }
    return ret;
}

bool XPString::starts_with(const char *str)
{
    size_t len = strnlen(str, _size - 1);
    if (len == 0)
    {
        return true;
    }
    else if (len > length())
    {
        return false;
    }

    int comp = strncmp(_buf, str, len);
    return comp == 0;
}

bool XPString::ends_with(const char *str)
{
    size_t other_len = strnlen(str, _size - 1);
    size_t self_len = length();
    if (other_len == 0)
    {
        return true;
    }
    else if (other_len > self_len)
    {
        return false;
    }

    size_t offset = self_len - other_len;
    int comp = strncmp(&_buf[offset], str, other_len);
    return comp == 0;
}

int XPString::index_of(const char *str)
{
    size_t len = strnlen(str, _size - 1);
    if (len > length())
    {
        return -1;
    }

    char *index = strstr(_buf, str);
    if (index == NULL) {
        return -1;
    }

    return (int)(index - _buf);
}

void XPString::reverse()
{
    strrev(_buf);
}
