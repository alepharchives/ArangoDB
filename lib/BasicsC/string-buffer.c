////////////////////////////////////////////////////////////////////////////////
/// @brief a string buffer for sequential string concatenation 
///
/// @file
///
/// DISCLAIMER
///
/// Copyright 2004-2012 triagens GmbH, Cologne, Germany
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// Copyright holder is triAGENS GmbH, Cologne, Germany
///
/// @author Dr. Frank Celler
/// @author Copyright 2011-2012, triAGENS GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#include "string-buffer.h"

// -----------------------------------------------------------------------------
// --SECTION--                                                 private functions
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Strings
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief append a character without check
////////////////////////////////////////////////////////////////////////////////

static void AppendChar (TRI_string_buffer_t * self, char chr) {
  *self->_current++ = chr;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief how much space is presently left in buffer?
////////////////////////////////////////////////////////////////////////////////

static size_t Remaining (TRI_string_buffer_t * self) {
  return self->_len - (self->_current - self->_buffer);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief reserve space
////////////////////////////////////////////////////////////////////////////////

static int Reserve (TRI_string_buffer_t * self, size_t size) {
  size_t len;
  char* ptr;

  if (size < 1) {
    return TRI_ERROR_NO_ERROR;
  }

  if (size > Remaining(self)) {
    ptrdiff_t off;

    off = self->_current - self->_buffer;
    len = (size_t)(1.2 * (self->_len + size));
    ptr = TRI_Reallocate(self->_memoryZone, self->_buffer, len + 1);

    if (ptr == NULL) {
      return TRI_ERROR_OUT_OF_MEMORY;
    }

    self->_buffer = ptr;
    self->_len = len;
    self->_current = self->_buffer + off;

    memset(self->_current, 0, Remaining(self) + 1);
  }

  return TRI_ERROR_NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                      constructors and destructors
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Strings
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief create a new string buffer and initialise it
////////////////////////////////////////////////////////////////////////////////

TRI_string_buffer_t* TRI_CreateStringBuffer (TRI_memory_zone_t* zone) {
  TRI_string_buffer_t* self = (TRI_string_buffer_t*) TRI_Allocate(zone, sizeof(TRI_string_buffer_t), false);

  if (self == NULL) {
    return NULL;
  }

  TRI_InitStringBuffer(self, zone);

  return self;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief initialises the string buffer
///
/// @warning You must call initialise before using the string buffer.
////////////////////////////////////////////////////////////////////////////////

void TRI_InitStringBuffer (TRI_string_buffer_t * self, TRI_memory_zone_t* zone) {
  memset(self, 0, sizeof(TRI_string_buffer_t));
  self->_memoryZone = zone;

  Reserve(self, 100);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief frees the string buffer
///
/// @warning You must call free or destroy after using the string buffer.
////////////////////////////////////////////////////////////////////////////////

void  TRI_DestroyStringBuffer (TRI_string_buffer_t * self) {
  if (self->_buffer != NULL) {
    TRI_Free(self->_memoryZone, self->_buffer);
  }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief frees the string buffer and cleans the buffer
///
/// @warning You must call free after or destroy using the string buffer.
////////////////////////////////////////////////////////////////////////////////

void TRI_AnnihilateStringBuffer (TRI_string_buffer_t * self) {
  if (self->_buffer != NULL) {

    // somewhat paranoid? don't ask me
    memset(self->_buffer, 0, self->_len);
    TRI_Free(self->_memoryZone, self->_buffer);
  }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief frees the string buffer and the pointer
////////////////////////////////////////////////////////////////////////////////

void  TRI_FreeStringBuffer (TRI_memory_zone_t* zone, TRI_string_buffer_t * self) {
  TRI_DestroyStringBuffer(self);
  TRI_Free(zone, self);
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                    public methods
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Strings
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief swaps content with another string buffer
////////////////////////////////////////////////////////////////////////////////

void TRI_SwapStringBuffer (TRI_string_buffer_t * self, TRI_string_buffer_t * other) {
  char * otherBuffer           = other->_buffer;
  char * otherCurrent          = other->_current;
  size_t otherLen              = other->_len;
  TRI_memory_zone_t* otherZone = other->_memoryZone;

  other->_buffer      = self->_buffer;
  other->_current     = self->_current;
  other->_len         = self->_len;
  other->_memoryZone  = self->_memoryZone;

  self->_buffer       = otherBuffer;
  self->_current      = otherCurrent;
  self->_len          = otherLen;
  self->_memoryZone   = otherZone;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief returns pointer to the beginning of the character buffer
////////////////////////////////////////////////////////////////////////////////

char const * TRI_BeginStringBuffer (TRI_string_buffer_t const * self) {
  return self->_buffer;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief returns pointer to the end of the character buffer
////////////////////////////////////////////////////////////////////////////////

char const * TRI_EndStringBuffer (TRI_string_buffer_t const * self) {
  return self->_current;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief returns length of the character buffer
////////////////////////////////////////////////////////////////////////////////

size_t TRI_LengthStringBuffer (TRI_string_buffer_t const * self) {
  return self->_current - self->_buffer;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief returns true if buffer is empty
////////////////////////////////////////////////////////////////////////////////

bool TRI_EmptyStringBuffer (TRI_string_buffer_t const * self) {
  return self->_buffer == self->_current;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief clears the buffer
////////////////////////////////////////////////////////////////////////////////

void TRI_ClearStringBuffer (TRI_string_buffer_t * self) {
  self->_current = self->_buffer; 
  memset(self->_buffer, 0, self->_len + 1);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief copies the string buffer
////////////////////////////////////////////////////////////////////////////////

int TRI_CopyStringBuffer (TRI_string_buffer_t * self, TRI_string_buffer_t const * source) {
  return TRI_ReplaceStringStringBuffer(self, source->_buffer, source->_current - source->_buffer);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief removes the first characters
////////////////////////////////////////////////////////////////////////////////

void TRI_EraseFrontStringBuffer (TRI_string_buffer_t * self, size_t len) {
  size_t off = self->_current - self->_buffer;

  if (off <= len) {
    TRI_ClearStringBuffer(self); 
  }
  else if (0 < len) {
    memmove(self->_buffer, self->_buffer + len, off - len);
    self->_current -= len;
    memset(self->_current, 0, self->_len - (self->_current - self->_buffer));
  }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief replaces characters
////////////////////////////////////////////////////////////////////////////////

int TRI_ReplaceStringStringBuffer (TRI_string_buffer_t * self, char const * str, size_t len) {
  self->_current = self->_buffer;

  return TRI_AppendString2StringBuffer(self, str, len);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief replaces characters
////////////////////////////////////////////////////////////////////////////////

int TRI_ReplaceStringBufferStringBuffer (TRI_string_buffer_t * self, TRI_string_buffer_t const * text) {
  self->_current = self->_buffer;

  return TRI_AppendString2StringBuffer(self, text->_buffer, text->_current - text->_buffer);
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                  STRING APPENDERS
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// --SECTION--                                                  public functions
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Strings
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief appends character
////////////////////////////////////////////////////////////////////////////////

int TRI_AppendCharStringBuffer (TRI_string_buffer_t * self, char chr) {
  int res;

  res = Reserve(self, 1);

  if (res != TRI_ERROR_NO_ERROR) {
    return res;
  }

  AppendChar(self, chr);
  return TRI_ERROR_NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief appends characters
////////////////////////////////////////////////////////////////////////////////

int TRI_AppendStringStringBuffer (TRI_string_buffer_t * self, char const * str) {
  size_t len = strlen(str);
  int res;

  res = Reserve(self, len);

  if (res != TRI_ERROR_NO_ERROR) {
    return res;
  }

  memcpy(self->_current, str, len);
  self->_current += len;
  return TRI_ERROR_NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief appends characters
////////////////////////////////////////////////////////////////////////////////

int TRI_AppendString2StringBuffer (TRI_string_buffer_t * self, char const * str, size_t len) {
  int res;

  if (0 < len) {
    res = Reserve(self, len);

    if (res != TRI_ERROR_NO_ERROR) {
      return res;
    }

    memcpy(self->_current, str, len);
    self->_current += len;
  }

  return TRI_ERROR_NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief appends a string buffer
////////////////////////////////////////////////////////////////////////////////

int TRI_AppendStringBufferStringBuffer (TRI_string_buffer_t * self, TRI_string_buffer_t const * text) {
  return TRI_AppendString2StringBuffer(self, text->_buffer, text->_current - text->_buffer);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief appends a blob
////////////////////////////////////////////////////////////////////////////////

int TRI_AppendBlobStringBuffer (TRI_string_buffer_t * self, TRI_blob_t const * text) {
  return TRI_AppendString2StringBuffer(self, text->data, text->length);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief appends eol character
////////////////////////////////////////////////////////////////////////////////

int TRI_AppendEolStringBuffer (TRI_string_buffer_t * self) {
  return TRI_AppendCharStringBuffer(self, '\n');
}

////////////////////////////////////////////////////////////////////////////////
/// @brief appends characters but url-encode the string
////////////////////////////////////////////////////////////////////////////////

int TRI_AppendUrlEncodedStringStringBuffer (TRI_string_buffer_t * self,
                                            char const * src) {
  static char hexChars[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

  size_t len = strlen(src);
  int res;

  char const* end;

  res = Reserve(self, len * 3);

  if (res != TRI_ERROR_NO_ERROR) {
    return res;
  }

  end = src + len;

  for (; src < end;  ++src) {
    if ('0' <= *src && *src <= '9') {
      AppendChar(self, *src);
    }

    else if ('a' <= *src && *src <= 'z') {
      AppendChar(self, *src);
    }

    else if ('A' <= *src && *src <= 'Z') {
      AppendChar(self, *src);
    }

    else if (*src == '-' || *src == '_' || *src == '.' || *src == '~') {
      AppendChar(self, *src);
    }

    else {
      uint8_t n = (uint8_t)(*src);
      uint8_t n1 = n >> 4;
      uint8_t n2 = n & 0x0F;

      AppendChar(self, '%');
      AppendChar(self, hexChars[n1]);
      AppendChar(self, hexChars[n2]);
    }
  }

  return TRI_ERROR_NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                 INTEGER APPENDERS
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// --SECTION--                                                  public functions
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Strings
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief appends integer with two digits
////////////////////////////////////////////////////////////////////////////////

int TRI_AppendInteger2StringBuffer (TRI_string_buffer_t * self, uint32_t attr) {
  int res;

  res = Reserve(self, 2);

  if (res != TRI_ERROR_NO_ERROR) {
    return res;
  }

  AppendChar(self, (attr / 10U) % 10 + '0');
  AppendChar(self,  attr % 10        + '0');

  return TRI_ERROR_NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief appends integer with three digits
////////////////////////////////////////////////////////////////////////////////

int TRI_AppendInteger3StringBuffer (TRI_string_buffer_t * self, uint32_t attr) {
  int res;

  res = Reserve(self, 3);

  if (res != TRI_ERROR_NO_ERROR) {
    return res;
  }

  AppendChar(self, (attr / 100U) % 10 + '0');
  AppendChar(self, (attr /  10U) % 10 + '0');
  AppendChar(self,  attr         % 10 + '0');

  return TRI_ERROR_NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief appends integer with four digits
////////////////////////////////////////////////////////////////////////////////

int TRI_AppendInteger4StringBuffer (TRI_string_buffer_t * self, uint32_t attr) {
  int res;

  res = Reserve(self, 4);

  if (res != TRI_ERROR_NO_ERROR) {
    return res;
  }

  AppendChar(self, (attr / 1000U) % 10 + '0');
  AppendChar(self, (attr /  100U) % 10 + '0');
  AppendChar(self, (attr /   10U) % 10 + '0');
  AppendChar(self,  attr          % 10 + '0');

  return TRI_ERROR_NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief appends integer with 8 bits
////////////////////////////////////////////////////////////////////////////////

int TRI_AppendInt8StringBuffer (TRI_string_buffer_t * self, int8_t attr) {
  int res;

  if (attr == INT8_MIN) {
    return TRI_AppendString2StringBuffer(self, "-128", 4);
  }

  res = Reserve(self, 4);

  if (res != TRI_ERROR_NO_ERROR) {
    return res;
  }

  if (attr < 0) {
    AppendChar(self, '-');
    attr = -attr;
  }

  if (100 <= attr) { AppendChar(self, (attr / 100) % 10 + '0'); }
  if ( 10 <= attr) { AppendChar(self, (attr /  10) % 10 + '0'); }

  AppendChar(self, attr % 10 + '0');

  return TRI_ERROR_NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief appends unsigned integer with 8 bits
////////////////////////////////////////////////////////////////////////////////

int TRI_AppendUInt8StringBuffer (TRI_string_buffer_t * self, uint8_t attr) {
  int res;

  res = Reserve(self, 3);

  if (res != TRI_ERROR_NO_ERROR) {
    return res;
  }

  if (100U <= attr) { AppendChar(self, (attr / 100U) % 10 + '0'); }
  if ( 10U <= attr) { AppendChar(self, (attr /  10U) % 10 + '0'); }

  AppendChar(self, attr % 10 + '0');

  return TRI_ERROR_NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief appends integer with 16 bits
////////////////////////////////////////////////////////////////////////////////

int TRI_AppendInt16StringBuffer (TRI_string_buffer_t * self, int16_t attr) {
  int res;

  if (attr == INT16_MIN) {
    return TRI_AppendString2StringBuffer(self, "-32768", 6);
  }

  res = Reserve(self, 6);

  if (res != TRI_ERROR_NO_ERROR) {
    return res;
  }

  if (attr < 0) {
    AppendChar(self, '-');
    attr = -attr;
  }

  if (10000 <= attr) { AppendChar(self, (attr / 10000) % 10 + '0'); }
  if ( 1000 <= attr) { AppendChar(self, (attr /  1000) % 10 + '0'); }
  if (  100 <= attr) { AppendChar(self, (attr /   100) % 10 + '0'); }
  if (   10 <= attr) { AppendChar(self, (attr /    10) % 10 + '0'); }

  AppendChar(self, attr % 10 + '0');

  return TRI_ERROR_NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief appends unsigned integer with 32 bits
////////////////////////////////////////////////////////////////////////////////

int TRI_AppendUInt16StringBuffer (TRI_string_buffer_t * self, uint16_t attr) {
  int res;

  res = Reserve(self, 5);

  if (res != TRI_ERROR_NO_ERROR) {
    return res;
  }

  if (10000U <= attr) { AppendChar(self, (attr / 10000U) % 10 + '0'); }
  if ( 1000U <= attr) { AppendChar(self, (attr /  1000U) % 10 + '0'); }
  if (  100U <= attr) { AppendChar(self, (attr /   100U) % 10 + '0'); }
  if (   10U <= attr) { AppendChar(self, (attr /    10U) % 10 + '0'); }

  AppendChar(self, attr % 10 + '0');

  return TRI_ERROR_NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief appends integer with 32 bits
////////////////////////////////////////////////////////////////////////////////

int TRI_AppendInt32StringBuffer (TRI_string_buffer_t * self, int32_t attr) {
  int res;

  if (attr == INT32_MIN) {
    return TRI_AppendString2StringBuffer(self, "-2147483648", 11);
  }

  res = Reserve(self, 11);

  if (res != TRI_ERROR_NO_ERROR) {
    return res;
  }

  if (attr < 0) {
    AppendChar(self, '-');
    attr = -attr;
  }

  if (1000000000 <= attr) { AppendChar(self, (attr / 1000000000) % 10 + '0'); }
  if ( 100000000 <= attr) { AppendChar(self, (attr /  100000000) % 10 + '0'); }
  if (  10000000 <= attr) { AppendChar(self, (attr /   10000000) % 10 + '0'); }
  if (   1000000 <= attr) { AppendChar(self, (attr /    1000000) % 10 + '0'); }
  if (    100000 <= attr) { AppendChar(self, (attr /     100000) % 10 + '0'); }
  if (     10000 <= attr) { AppendChar(self, (attr /      10000) % 10 + '0'); }
  if (      1000 <= attr) { AppendChar(self, (attr /       1000) % 10 + '0'); }
  if (       100 <= attr) { AppendChar(self, (attr /        100) % 10 + '0'); }
  if (        10 <= attr) { AppendChar(self, (attr /         10) % 10 + '0'); }

  AppendChar(self, attr % 10 + '0');

  return TRI_ERROR_NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief appends unsigned integer with 32 bits
////////////////////////////////////////////////////////////////////////////////

int TRI_AppendUInt32StringBuffer (TRI_string_buffer_t * self, uint32_t attr) {
  int res;

  res = Reserve(self, 10);

  if (res != TRI_ERROR_NO_ERROR) {
    return res;
  }

  if (1000000000U <= attr) { AppendChar(self, (attr / 1000000000U) % 10 + '0'); }
  if ( 100000000U <= attr) { AppendChar(self, (attr /  100000000U) % 10 + '0'); }
  if (  10000000U <= attr) { AppendChar(self, (attr /   10000000U) % 10 + '0'); }
  if (   1000000U <= attr) { AppendChar(self, (attr /    1000000U) % 10 + '0'); }
  if (    100000U <= attr) { AppendChar(self, (attr /     100000U) % 10 + '0'); }
  if (     10000U <= attr) { AppendChar(self, (attr /      10000U) % 10 + '0'); }
  if (      1000U <= attr) { AppendChar(self, (attr /       1000U) % 10 + '0'); }
  if (       100U <= attr) { AppendChar(self, (attr /        100U) % 10 + '0'); }
  if (        10U <= attr) { AppendChar(self, (attr /         10U) % 10 + '0'); }

  AppendChar(self, attr % 10 + '0');

  return TRI_ERROR_NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief appends integer with 64 bits
////////////////////////////////////////////////////////////////////////////////

int TRI_AppendInt64StringBuffer (TRI_string_buffer_t * self, int64_t attr) {
  int res;

  if (attr == INT64_MIN) {
    return TRI_AppendString2StringBuffer(self, "-9223372036854775808", 20);
  }
  else if (attr < 0) {
    res = Reserve(self, 1);

    if (res != TRI_ERROR_NO_ERROR) {
      return res;
    }

    AppendChar(self, '-');
    attr = -attr;
  }

  if ((attr >> 32) == 0) {
    return TRI_AppendUInt32StringBuffer(self, (uint32_t) attr);
  }

  res = Reserve(self, 20);

  if (res != TRI_ERROR_NO_ERROR) {
    return res;
  }

  // uint64_t has one more decimal than int64_t
  if (1000000000000000000LL <= attr) { AppendChar(self, (attr / 1000000000000000000LL) % 10 + '0'); }
  if ( 100000000000000000LL <= attr) { AppendChar(self, (attr /  100000000000000000LL) % 10 + '0'); }
  if (  10000000000000000LL <= attr) { AppendChar(self, (attr /   10000000000000000LL) % 10 + '0'); }
  if (   1000000000000000LL <= attr) { AppendChar(self, (attr /    1000000000000000LL) % 10 + '0'); }
  if (    100000000000000LL <= attr) { AppendChar(self, (attr /     100000000000000LL) % 10 + '0'); }
  if (     10000000000000LL <= attr) { AppendChar(self, (attr /      10000000000000LL) % 10 + '0'); }
  if (      1000000000000LL <= attr) { AppendChar(self, (attr /       1000000000000LL) % 10 + '0'); }
  if (       100000000000LL <= attr) { AppendChar(self, (attr /        100000000000LL) % 10 + '0'); }
  if (        10000000000LL <= attr) { AppendChar(self, (attr /         10000000000LL) % 10 + '0'); }
  if (         1000000000LL <= attr) { AppendChar(self, (attr /          1000000000LL) % 10 + '0'); }
  if (          100000000LL <= attr) { AppendChar(self, (attr /           100000000LL) % 10 + '0'); }
  if (           10000000LL <= attr) { AppendChar(self, (attr /            10000000LL) % 10 + '0'); }
  if (            1000000LL <= attr) { AppendChar(self, (attr /             1000000LL) % 10 + '0'); }
  if (             100000LL <= attr) { AppendChar(self, (attr /              100000LL) % 10 + '0'); }
  if (              10000LL <= attr) { AppendChar(self, (attr /               10000LL) % 10 + '0'); }
  if (               1000LL <= attr) { AppendChar(self, (attr /                1000LL) % 10 + '0'); }
  if (                100LL <= attr) { AppendChar(self, (attr /                 100LL) % 10 + '0'); }
  if (                 10LL <= attr) { AppendChar(self, (attr /                  10LL) % 10 + '0'); }

  AppendChar(self, attr % 10 + '0');

  return TRI_ERROR_NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief appends unsigned integer with 64 bits
////////////////////////////////////////////////////////////////////////////////

int TRI_AppendUInt64StringBuffer (TRI_string_buffer_t * self, uint64_t attr) {
  int res;

  if ((attr >> 32) == 0) {
    return TRI_AppendUInt32StringBuffer(self, (uint32_t) attr);
  }

  res = Reserve(self, 21);

  if (res != TRI_ERROR_NO_ERROR) {
    return res;
  }

  // uint64_t has one more decimal than int64_t
  if (10000000000000000000ULL <= attr) { AppendChar(self, (attr / 10000000000000000000ULL) % 10 + '0'); }
  if ( 1000000000000000000ULL <= attr) { AppendChar(self, (attr /  1000000000000000000ULL) % 10 + '0'); }
  if (  100000000000000000ULL <= attr) { AppendChar(self, (attr /   100000000000000000ULL) % 10 + '0'); }
  if (   10000000000000000ULL <= attr) { AppendChar(self, (attr /    10000000000000000ULL) % 10 + '0'); }
  if (    1000000000000000ULL <= attr) { AppendChar(self, (attr /     1000000000000000ULL) % 10 + '0'); }
  if (     100000000000000ULL <= attr) { AppendChar(self, (attr /      100000000000000ULL) % 10 + '0'); }
  if (      10000000000000ULL <= attr) { AppendChar(self, (attr /       10000000000000ULL) % 10 + '0'); }
  if (       1000000000000ULL <= attr) { AppendChar(self, (attr /        1000000000000ULL) % 10 + '0'); }
  if (        100000000000ULL <= attr) { AppendChar(self, (attr /         100000000000ULL) % 10 + '0'); }
  if (         10000000000ULL <= attr) { AppendChar(self, (attr /          10000000000ULL) % 10 + '0'); }
  if (          1000000000ULL <= attr) { AppendChar(self, (attr /           1000000000ULL) % 10 + '0'); }
  if (           100000000ULL <= attr) { AppendChar(self, (attr /            100000000ULL) % 10 + '0'); }
  if (            10000000ULL <= attr) { AppendChar(self, (attr /             10000000ULL) % 10 + '0'); }
  if (             1000000ULL <= attr) { AppendChar(self, (attr /              1000000ULL) % 10 + '0'); }
  if (              100000ULL <= attr) { AppendChar(self, (attr /               100000ULL) % 10 + '0'); }
  if (               10000ULL <= attr) { AppendChar(self, (attr /                10000ULL) % 10 + '0'); }
  if (                1000ULL <= attr) { AppendChar(self, (attr /                 1000ULL) % 10 + '0'); }
  if (                 100ULL <= attr) { AppendChar(self, (attr /                  100ULL) % 10 + '0'); }
  if (                  10ULL <= attr) { AppendChar(self, (attr /                   10ULL) % 10 + '0'); }

  AppendChar(self, attr % 10 + '0');

  return TRI_ERROR_NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief appends size_t
////////////////////////////////////////////////////////////////////////////////

int TRI_AppendSizeStringBuffer (TRI_string_buffer_t * self, size_t attr) {
#if TRI_SIZEOF_SIZE_T == 8
  return TRI_AppendUInt64StringBuffer(self, (uint64_t) attr);
#else
  return TRI_AppendUInt32StringBuffer(self, (uint32_t) attr);
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                           INTEGER OCTAL APPENDERS
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// --SECTION--                                                  public functions
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Strings
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief appends unsigned integer with 32 bits in octal
////////////////////////////////////////////////////////////////////////////////

int TRI_AppendUInt32OctalStringBuffer (TRI_string_buffer_t * self, uint32_t attr) {
  int res;

  res = Reserve(self, 8);

  if (res != TRI_ERROR_NO_ERROR) {
    return res;
  }

  if (010000000U <= attr) { AppendChar(self, (attr / 010000000U) % 010 + '0'); }
  if ( 01000000U <= attr) { AppendChar(self, (attr /  01000000U) % 010 + '0'); }
  if (  0100000U <= attr) { AppendChar(self, (attr /   0100000U) % 010 + '0'); }
  if (   010000U <= attr) { AppendChar(self, (attr /    010000U) % 010 + '0'); }
  if (    01000U <= attr) { AppendChar(self, (attr /     01000U) % 010 + '0'); }
  if (     0100U <= attr) { AppendChar(self, (attr /      0100U) % 010 + '0'); }
  if (      010U <= attr) { AppendChar(self, (attr /       010U) % 010 + '0'); }

  AppendChar(self, attr % 010 + '0');

  return TRI_ERROR_NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief appends unsigned integer with 64 bits in octal
////////////////////////////////////////////////////////////////////////////////

int TRI_AppendUInt64OctalStringBuffer (TRI_string_buffer_t * self, uint64_t attr) {
  int res;

  res = Reserve(self, 16);

  if (res != TRI_ERROR_NO_ERROR) {
    return res;
  }

  if (01000000000000000ULL <= attr) { AppendChar(self, (attr / 01000000000000000ULL) % 010 + '0'); }
  if ( 0100000000000000ULL <= attr) { AppendChar(self, (attr /  0100000000000000ULL) % 010 + '0'); }
  if (  010000000000000ULL <= attr) { AppendChar(self, (attr /   010000000000000ULL) % 010 + '0'); }
  if (   01000000000000ULL <= attr) { AppendChar(self, (attr /    01000000000000ULL) % 010 + '0'); }
  if (    0100000000000ULL <= attr) { AppendChar(self, (attr /     0100000000000ULL) % 010 + '0'); }
  if (     010000000000ULL <= attr) { AppendChar(self, (attr /      010000000000ULL) % 010 + '0'); }
  if (      01000000000ULL <= attr) { AppendChar(self, (attr /       01000000000ULL) % 010 + '0'); }
  if (       0100000000ULL <= attr) { AppendChar(self, (attr /        0100000000ULL) % 010 + '0'); }
  if (        010000000ULL <= attr) { AppendChar(self, (attr /         010000000ULL) % 010 + '0'); }
  if (         01000000ULL <= attr) { AppendChar(self, (attr /          01000000ULL) % 010 + '0'); }
  if (          0100000ULL <= attr) { AppendChar(self, (attr /           0100000ULL) % 010 + '0'); }
  if (           010000ULL <= attr) { AppendChar(self, (attr /            010000ULL) % 010 + '0'); }
  if (            01000ULL <= attr) { AppendChar(self, (attr /             01000ULL) % 010 + '0'); }
  if (             0100ULL <= attr) { AppendChar(self, (attr /              0100ULL) % 010 + '0'); }
  if (              010ULL <= attr) { AppendChar(self, (attr /               010ULL) % 010 + '0'); }

  AppendChar(self, attr % 010 + '0');

  return TRI_ERROR_NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief appends size_t in octal
////////////////////////////////////////////////////////////////////////////////

int TRI_AppendSizeOctalStringBuffer (TRI_string_buffer_t * self, size_t attr) {
#if TRI_SIZEOF_SIZE_T == 8
  return TRI_AppendUInt64OctalStringBuffer(self, (uint64_t) attr);
#else
  return TRI_AppendUInt32OctalStringBuffer(self, (uint32_t) attr);
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                             INTEGER HEX APPENDERS
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// --SECTION--                                                  public functions
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Strings
/// @{
////////////////////////////////////////////////////////////////////////////////

static char const * const HEX = "0123456789ABCDEF";

////////////////////////////////////////////////////////////////////////////////
/// @brief appends unsigned integer with 32 bits in hex
////////////////////////////////////////////////////////////////////////////////

int TRI_AppendUInt32HexStringBuffer (TRI_string_buffer_t * self, uint32_t attr) {
  int res;

  res = Reserve(self, 4);

  if (res != TRI_ERROR_NO_ERROR) {
    return res;
  }

  if (0x1000U <= attr) { AppendChar(self, HEX[(attr / 0x1000U) % 0x10]); }
  if ( 0x100U <= attr) { AppendChar(self, HEX[(attr /  0x100U) % 0x10]); }
  if (  0x10U <= attr) { AppendChar(self, HEX[(attr /   0x10U) % 0x10]); }

  AppendChar(self, HEX[attr % 0x10]);

  return TRI_ERROR_NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief appends unsigned integer with 64 bits in hex
////////////////////////////////////////////////////////////////////////////////

int TRI_AppendUInt64HexStringBuffer (TRI_string_buffer_t * self, uint64_t attr) {
  int res;

  res = Reserve(self, 8);

  if (res != TRI_ERROR_NO_ERROR) {
    return res;
  }

  if (0x10000000U <= attr) { AppendChar(self, HEX[(attr / 0x10000000U) % 0x10]); }
  if ( 0x1000000U <= attr) { AppendChar(self, HEX[(attr /  0x1000000U) % 0x10]); }
  if (  0x100000U <= attr) { AppendChar(self, HEX[(attr /   0x100000U) % 0x10]); }
  if (   0x10000U <= attr) { AppendChar(self, HEX[(attr /    0x10000U) % 0x10]); }
  if (    0x1000U <= attr) { AppendChar(self, HEX[(attr /     0x1000U) % 0x10]); }
  if (     0x100U <= attr) { AppendChar(self, HEX[(attr /      0x100U) % 0x10]); }
  if (      0x10U <= attr) { AppendChar(self, HEX[(attr /       0x10U) % 0x10]); }

  AppendChar(self, HEX[attr % 0x10]);

  return TRI_ERROR_NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief appends size_t in hex
////////////////////////////////////////////////////////////////////////////////

int TRI_AppendSizeHexStringBuffer (TRI_string_buffer_t * self, size_t attr) {
#if TRI_SIZEOF_SIZE_T == 8
  return TRI_AppendUInt64HexStringBuffer(self, (uint64_t) attr);
#else
  return TRI_AppendUInt32HexStringBuffer(self, (uint32_t) attr);
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                   FLOAT APPENDERS
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// --SECTION--                                                  public functions
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Strings
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief appends floating point number with 8 bits
////////////////////////////////////////////////////////////////////////////////

int TRI_AppendDoubleStringBuffer (TRI_string_buffer_t * self, double attr) {
  int res;

  res = Reserve(self, 1);

  if (res != TRI_ERROR_NO_ERROR) {
    return res;
  }

  if (attr < 0.0) {
    AppendChar(self, '-');
    attr = -attr;
  }
  else if (attr == 0.0) {
    AppendChar(self, '0');
    return TRI_ERROR_NO_ERROR;
  }

  if (((double)((uint32_t) attr)) == attr) {
    return TRI_AppendUInt32StringBuffer(self, (uint32_t) attr);
  }
  else if (attr < (double) 429496U) {
    uint32_t smll;

    smll = (uint32_t)(attr * 10000.0);

    if (((double) smll) == attr * 10000.0) {
      uint32_t ep;

      TRI_AppendUInt32StringBuffer(self, smll / 10000);

      ep = smll % 10000;

      if (ep != 0) {
        size_t pos;
        char a1;
        char a2;
        char a3;
        char a4;

        pos = 0;

        res = Reserve(self, 6);

        if (res != TRI_ERROR_NO_ERROR) {
          return res;
        }

        AppendChar(self, '.');

        if ((ep / 1000L) % 10 != 0)  pos = 1;
        a1 = (char) ((ep / 1000L) % 10 + '0');

        if ((ep / 100L) % 10 != 0)  pos = 2;
        a2 = (char) ((ep / 100L) % 10 + '0');

        if ((ep / 10L) % 10 != 0)  pos = 3;
        a3 = (char) ((ep / 10L) % 10 + '0');

        if (ep % 10 != 0)  pos = 4;
        a4 = (char) (ep % 10 + '0');

        AppendChar(self, a1);
        if (pos > 1) { AppendChar(self, a2); }
        if (pos > 2) { AppendChar(self, a3); }
        if (pos > 3) { AppendChar(self, a4); }

      }

      return TRI_ERROR_NO_ERROR;
    }
  }

  // we do not habe a small integral number nor small decimal number with only a few decimal digits

  // there at most 16 significant digits, first find out if we have an integer value
  if (10000000000000000.0 < attr) {
    size_t n;

    n = 0;

    while (10000000000000000.0 < attr) {
      attr /= 10.0;
      ++n;
    }

    res = TRI_AppendUInt64StringBuffer(self, (uint64_t) attr);

    if (res != TRI_ERROR_NO_ERROR) {
      return res;
    }

    res = Reserve(self, n);

    if (res != TRI_ERROR_NO_ERROR) {
      return res;
    }

    for (;  0 < n;  --n) {
      AppendChar(self, '0');
    }

    return TRI_ERROR_NO_ERROR;
  }


  // very small, i. e. less than 1
  else if (attr < 1.0) {
    size_t n;

    n = 0;

    while (attr < 1.0) {
      attr *= 10.0;
      ++n;

      // should not happen, so it must be almost 0
      if (n > 400) {
        return TRI_AppendUInt32StringBuffer(self, 0);
      }
    }

    res = Reserve(self, n + 2);

    if (res != TRI_ERROR_NO_ERROR) {
      return res;
    }

    AppendChar(self, '0');
    AppendChar(self, '.');

    for (--n;  0 < n;  --n) {
      AppendChar(self, '0');
    }

    attr = 10000000000000000.0 * attr;

    return TRI_AppendUInt64StringBuffer(self, (uint64_t) attr);
  }


  // somewhere in between
  else {
    uint64_t m;
    double d;
    size_t n;

    m = (uint64_t) attr;
    d = attr - m;
    n = 0;

    TRI_AppendUInt64StringBuffer(self, m);

    while (d < 1.0) {
      d *= 10.0;
      ++n;

      // should not happen, so it must be almost 0
      if (n > 400) {
        return TRI_ERROR_NO_ERROR;
      }
    }

    res = Reserve(self, n + 1);

    if (res != TRI_ERROR_NO_ERROR) {
      return res;
    }

    AppendChar(self, '.');

    for (--n;  0 < n;  --n) {
      AppendChar(self, '0');
    }

    d = 10000000000000000.0 * d;

    return TRI_AppendUInt64StringBuffer(self, (uint64_t) d);
  }
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                           DATE AND TIME APPENDERS
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// --SECTION--                                                  public functions
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Strings
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief appends time in standard format
////////////////////////////////////////////////////////////////////////////////

int TRI_AppendTimeStringBuffer (TRI_string_buffer_t * self, int32_t attr) {
  int hour;
  int minute;
  int second;
  int res;

  hour = attr / 3600;
  minute = (attr / 60) % 60;
  second = attr % 60;

  res = Reserve(self, 9);

  if (res != TRI_ERROR_NO_ERROR) {
    return res;
  }

  TRI_AppendInteger2StringBuffer(self, hour);
  AppendChar(self, ':');
  TRI_AppendInteger2StringBuffer(self, minute);
  AppendChar(self, ':');
  TRI_AppendInteger2StringBuffer(self, second);

  return TRI_ERROR_NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                     CSV APPENDERS
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// --SECTION--                                                  public functions
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Strings
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief appends csv 32-bit integer
////////////////////////////////////////////////////////////////////////////////

int TRI_AppendCsvInt32StringBuffer (TRI_string_buffer_t * self, int32_t i) {
  int res;

  res = TRI_AppendInt32StringBuffer(self, i);

  if (res != TRI_ERROR_NO_ERROR) {
    return res;
  }

  res = Reserve(self, 1);

  if (res != TRI_ERROR_NO_ERROR) {
    return res;
  }

  AppendChar(self, ';');
  return TRI_ERROR_NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief appends csv unisgned 32-bit integer
////////////////////////////////////////////////////////////////////////////////

int TRI_AppendCsvUInt32StringBuffer (TRI_string_buffer_t * self, uint32_t i) {
  int res;

  res = TRI_AppendUInt32StringBuffer(self, i);

  if (res != TRI_ERROR_NO_ERROR) {
    return res;
  }

  res = Reserve(self, 1);

  if (res != TRI_ERROR_NO_ERROR) {
    return res;
  }

  AppendChar(self, ';');
  return TRI_ERROR_NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief appends csv 64-bit integer
////////////////////////////////////////////////////////////////////////////////

int TRI_AppendCsvInt64StringBuffer (TRI_string_buffer_t * self, int64_t i) {
  int res;

  res = TRI_AppendInt64StringBuffer(self, i);

  if (res != TRI_ERROR_NO_ERROR) {
    return res;
  }

  res = Reserve(self, 1);

  if (res != TRI_ERROR_NO_ERROR) {
    return res;
  }

  AppendChar(self, ';');
  return TRI_ERROR_NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief appends csv unsigned 64-bit integer
////////////////////////////////////////////////////////////////////////////////

int TRI_AppendCsvUInt64StringBuffer (TRI_string_buffer_t * self, uint64_t i) {
  int res;

  res = TRI_AppendUInt64StringBuffer(self, i);

  if (res != TRI_ERROR_NO_ERROR) {
    return res;
  }

  res = Reserve(self, 1);

  if (res != TRI_ERROR_NO_ERROR) {
    return res;
  }

  AppendChar(self, ';');
  return TRI_ERROR_NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief appends csv double
////////////////////////////////////////////////////////////////////////////////

int TRI_AppendCsvDoubleStringBuffer (TRI_string_buffer_t * self, double d) {
  int res;

  res = TRI_AppendDoubleStringBuffer(self, d);

  if (res != TRI_ERROR_NO_ERROR) {
    return res;
  }

  res = Reserve(self, 1);

  if (res != TRI_ERROR_NO_ERROR) {
    return res;
  }

  AppendChar(self, ';');
  return TRI_ERROR_NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// Local Variables:
// mode: outline-minor
// outline-regexp: "^\\(/// @brief\\|/// {@inheritDoc}\\|/// @addtogroup\\|// --SECTION--\\|/// @\\}\\)"
// End:
