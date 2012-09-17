////////////////////////////////////////////////////////////////////////////////
/// @brief Utf8/Utf16 Helper class
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
/// @author Frank Celler
/// @author Achim Brandt
/// @author Copyright 2010-2012, triAGENS GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#include "Utf8Helper.h"

#ifdef TRI_HAVE_ICU
#include "BasicsC/utf8-helper.h"
#else
#include "string.h"
#endif

using namespace triagens::basics;
using namespace std;

// -----------------------------------------------------------------------------
// --SECTION--                                      constructors and destructors
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UTF8
/// @{
////////////////////////////////////////////////////////////////////////////////

Utf8Helper::Utf8Helper (const string& lang) : _coll(0) {
#ifdef TRI_HAVE_ICU  
  UErrorCode status = U_ZERO_ERROR;   
  
  if (lang == "") {
    _coll = Collator::createInstance(status); 
  }
  else {
    Locale locale(lang.c_str());
    _coll = Collator::createInstance(locale, status);     
  }
  
  if(U_FAILURE(status)) {
    cerr << "error in Collator::createInstance(): " << u_errorName(status) << endl;
    return;
  }
  
  // set the default attributes for sorting:
  _coll->setAttribute(UCOL_CASE_FIRST, UCOL_UPPER_FIRST, status);  // A < a
  _coll->setAttribute(UCOL_NORMALIZATION_MODE, UCOL_OFF, status);
  _coll->setAttribute(UCOL_STRENGTH, UCOL_IDENTICAL, status);      // UCOL_IDENTICAL, UCOL_PRIMARY, UCOL_SECONDARY, UCOL_TERTIARY

  if(U_FAILURE(status)) {
    cerr << "error in Collator::setAttribute(...): " << u_errorName(status) << endl;
  }
#endif
}

Utf8Helper::~Utf8Helper () {
  if (_coll) {
    delete _coll;
  }
}

int Utf8Helper::compareUtf8 (const char* left, size_t leftLength, const char* right, size_t rightLength) {
#ifdef TRI_HAVE_ICU  
  if (!_coll) {
    cerr << "no Collator!" << endl;
    return 0;
  }
  
  UErrorCode status = U_ZERO_ERROR;
  int result = _coll->compareUTF8(StringPiece(left, leftLength), StringPiece(right, rightLength), status);
  if(U_FAILURE(status)) {
    cerr << "error in Collator::compareUTF8(...): " << u_errorName(status) << endl;
    return 0;
  }
  
  switch (result) {
    case (UCOL_GREATER):
      return 1;
      
    case (UCOL_LESS):
      return -1;
      
    default:
      return 0;
  }
#else
  int result = 0;
  
  if (leftLength == rightLength) {
    return memcmp((const void*)left, (const void*)right, leftLength);
  }
  
  result = memcmp((const void*)left, (const void*)right, leftLength < rightLength ? leftLength : rightLength);
  
  if (result == 0) {
    if (leftLength < rightLength) {
      return -1;
    }
    return 1;
  }
  
  return result;  
#endif
}

int Utf8Helper::compareUtf16 (const uint16_t* left, size_t leftLength, const uint16_t* right, size_t rightLength) {  
#ifdef TRI_HAVE_ICU  
  if (!_coll) {
    cerr << "no Collator!" << endl;
    return 0;
  }
  
  int result = _coll->compare((const UChar *)left, leftLength, (const UChar *)right, rightLength);
  
  switch (result) {
    case (UCOL_GREATER):
      return 1;
      
    case (UCOL_LESS):
      return -1;
      
    default:
      return 0;
  }
#else 
  int result = 0;
  
  if (leftLength == rightLength) {
    return memcmp((const void*)left, (const void*)right, leftLength);
  }
  
  result = memcmp((const void*)left, (const void*)right, leftLength < rightLength ? leftLength : rightLength);
  
  if (result == 0) {
    if (leftLength < rightLength) {
      return -1;
    }
    return 1;
  }
  
  return result;  
#endif  
}

#ifdef TRI_HAVE_ICU  

char * normalizeUtf8 (TRI_memory_zone_t* zone, const char* utf8, size_t inLength, size_t* outLength) {
  return TR_normalize_utf8_to_NFC(zone, utf8, inLength, outLength);
}

char * normalizeUtf16 (TRI_memory_zone_t* zone, const uint16_t* utf16, size_t inLength, size_t* outLength) {
  return TR_normalize_utf16_to_NFC(zone, utf16, inLength, outLength);  
}

#endif

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// Local Variables:
// mode: outline-minor
// outline-regexp: "^\\(/// @brief\\|/// {@inheritDoc}\\|/// @addtogroup\\|// --SECTION--\\|/// @\\}\\)"
// End:
