////////////////////////////////////////////////////////////////////////////////
/// @brief Ahuacatl, index access
///
/// @file
///
/// DISCLAIMER
///
/// Copyright 2010-2012 triagens GmbH, Cologne, Germany
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
/// @author Jan Steemann
/// @author Copyright 2012, triagens GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#include "Ahuacatl/ahuacatl-index.h"
#include "Ahuacatl/ahuacatl-access-optimiser.h"
#include "Ahuacatl/ahuacatl-context.h" 
#include "Ahuacatl/ahuacatl-log.h"

// -----------------------------------------------------------------------------
// --SECTION--                                                 private functions
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Ahuacatl
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief log information about the used index
////////////////////////////////////////////////////////////////////////////////

#ifdef TRI_DEBUG_AQL    
static void LogIndexString (const char* const what,
                            TRI_index_t const* idx,
                            char const* collectionName) {
  TRI_string_buffer_t* buffer = TRI_CreateStringBuffer(TRI_UNKNOWN_MEM_ZONE);
  size_t i;

  if (buffer == NULL) {
    return;
  }
  
  for (i = 0; i < idx->_fields._length; i++) {
    if (i > 0) {
      TRI_AppendStringStringBuffer(buffer, ", "); 
    }

    TRI_AppendStringStringBuffer(buffer, idx->_fields._buffer[i]);
  }

  TRI_AQL_LOG("%s %s index (%s) for '%s'", 
              what,
              TRI_TypeNameIndex(idx), 
              buffer->_buffer, 
              collectionName);

  TRI_FreeStringBuffer(TRI_UNKNOWN_MEM_ZONE, buffer);
}
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief check whether a field access candidate is an exact access
////////////////////////////////////////////////////////////////////////////////

static bool IsExactCandidate (const TRI_aql_field_access_t* const candidate) {
  if (candidate->_type == TRI_AQL_ACCESS_EXACT) {
    // ==
    return true;
  }

  if (candidate->_type == TRI_AQL_ACCESS_LIST) {
    // in (...)
    return true;
  }

  if (candidate->_type == TRI_AQL_ACCESS_REFERENCE && 
      candidate->_value._reference._operator == TRI_AQL_NODE_OPERATOR_BINARY_EQ) {
    // == ref
    return true;
  }

  return false;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief pick or replace an index
////////////////////////////////////////////////////////////////////////////////
      
static TRI_aql_index_t* PickIndex (TRI_aql_context_t* const context,
                                   TRI_aql_index_t* pickedIndex, 
                                   const TRI_index_t* const idx,
                                   TRI_vector_pointer_t* fieldAccesses) {
  bool isBetter;

  assert(idx);
  assert(fieldAccesses);

  if (pickedIndex == NULL) {
    pickedIndex = TRI_Allocate(TRI_UNKNOWN_MEM_ZONE, sizeof(TRI_aql_index_t), false);
    pickedIndex->_idx = NULL;
    pickedIndex->_fieldAccesses = NULL;
  }

  if (pickedIndex == NULL) {
    // OOM
    TRI_SetErrorContextAql(context, TRI_ERROR_OUT_OF_MEMORY, NULL);
    return NULL;
  }

  if (pickedIndex->_idx == NULL) {
    // any index is better than none
    isBetter = true;
  }
  else {
    isBetter = idx->_type == TRI_IDX_TYPE_PRIMARY_INDEX || // primary index is better than any others
               (idx->_type == TRI_IDX_TYPE_HASH_INDEX && pickedIndex->_idx->_type == TRI_IDX_TYPE_SKIPLIST_INDEX) || // hash is better than skiplist
               (idx->_unique && !pickedIndex->_idx->_unique) || // unique indexes are better than non-unique ones 
               (fieldAccesses->_length < pickedIndex->_fieldAccesses->_length && idx->_unique) || // shorter indexes are better if unique
               (fieldAccesses->_length > pickedIndex->_fieldAccesses->_length && !idx->_unique); // longer indexes are better if non-unique

    // if we have already picked the primary index, we won't overwrite it with any other index
    isBetter &= (pickedIndex->_idx->_type != TRI_IDX_TYPE_PRIMARY_INDEX);
  }

  if (isBetter) { 
    if (pickedIndex->_fieldAccesses != NULL) {
      TRI_FreeVectorPointer(TRI_UNKNOWN_MEM_ZONE, pickedIndex->_fieldAccesses);
    }

    pickedIndex->_idx = (TRI_index_t*) idx;
    pickedIndex->_fieldAccesses = TRI_CopyVectorPointer(TRI_UNKNOWN_MEM_ZONE, fieldAccesses);
  }

  return pickedIndex;
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                  public functions
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Ahuacatl
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief free an index structure
////////////////////////////////////////////////////////////////////////////////

void TRI_FreeIndexAql (TRI_aql_index_t* const idx) {
  assert(idx);

  TRI_FreeVectorPointer(TRI_UNKNOWN_MEM_ZONE, idx->_fieldAccesses);
  TRI_Free(TRI_UNKNOWN_MEM_ZONE, idx);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief determine which index to use for a specific for loop
////////////////////////////////////////////////////////////////////////////////

TRI_aql_index_t* TRI_DetermineIndexAql (TRI_aql_context_t* const context,
                                        const TRI_vector_pointer_t* const availableIndexes,
                                        const char* const collectionName,
                                        const TRI_vector_pointer_t* const candidates) {
  TRI_aql_index_t* picked = NULL;
  TRI_vector_pointer_t matches;
  size_t i, j, k, n; 

  TRI_InitVectorPointer(&matches, TRI_UNKNOWN_MEM_ZONE);

  assert(context);
  assert(collectionName);
  assert(candidates);

  n = availableIndexes->_length;
  for (i = 0; i < n; ++i) {
    TRI_index_t* idx = (TRI_index_t*) availableIndexes->_buffer[i];
    size_t numIndexFields = idx->_fields._length;
    bool lastTypeWasExact;
    
    if (numIndexFields == 0) {
      // index should contain at least one field
      continue;
    }

    // we'll use a switch here so the compiler warns if new index types are added elsewhere but not here
    switch (idx->_type) {
      case TRI_IDX_TYPE_GEO1_INDEX:
      case TRI_IDX_TYPE_GEO2_INDEX:
      case TRI_IDX_TYPE_PRIORITY_QUEUE_INDEX:
      case TRI_IDX_TYPE_CAP_CONSTRAINT:
      case TRI_IDX_TYPE_BITARRAY_INDEX:
        // ignore all these index types for now
        continue;
      case TRI_IDX_TYPE_PRIMARY_INDEX:
      case TRI_IDX_TYPE_HASH_INDEX:
      case TRI_IDX_TYPE_SKIPLIST_INDEX:
        // these indexes are valid candidates
        break;
    }
    
#ifdef TRI_DEBUG_AQL    
    LogIndexString("checking", idx, collectionName);
#endif

    TRI_ClearVectorPointer(&matches);

    lastTypeWasExact = true;

    // now loop over all index fields, from left to right
    // index field order is important because skiplists can be used with leftmost prefixes as well,
    // but not with rightmost prefixes
    for (j = 0; j < numIndexFields; ++j) {
      char* indexedFieldName = idx->_fields._buffer[j];

      if (indexedFieldName == NULL) {
        continue;
      }

      // now loop over all candidates
      for (k = 0; k < candidates->_length; ++k) {
        TRI_aql_field_access_t* candidate = (TRI_aql_field_access_t*) TRI_AtVectorPointer(candidates, k);
      
        if (candidate->_type == TRI_AQL_ACCESS_IMPOSSIBLE || 
            candidate->_type == TRI_AQL_ACCESS_ALL) {
          // wrong index type, doesn't help us at all
          continue;
        }
        
        if (!TRI_EqualString(indexedFieldName, candidate->_fullName + candidate->_variableNameLength + 1)) {
          // different attribute, doesn't help
          continue;
        }

        // attribute is used in index

        if (idx->_type == TRI_IDX_TYPE_PRIMARY_INDEX) {
          if (!IsExactCandidate(candidate)) {
            // wrong access type for primary index
            continue;
          }
          
          TRI_PushBackVectorPointer(&matches, candidate);
        }
        else if (idx->_type == TRI_IDX_TYPE_HASH_INDEX) {
          if (!IsExactCandidate(candidate)) {
            // wrong access type for hash index
            continue;
          }
          
          if (candidate->_type == TRI_AQL_ACCESS_LIST && numIndexFields != 1) {
            // we found a list, but the index covers multiple attributes. that means we cannot use list access
            continue;
          }
          
          TRI_PushBackVectorPointer(&matches, candidate);
        }
        else if (idx->_type == TRI_IDX_TYPE_SKIPLIST_INDEX) {
          bool candidateIsExact;

          if (candidate->_type != TRI_AQL_ACCESS_EXACT &&
              candidate->_type != TRI_AQL_ACCESS_LIST && 
              candidate->_type != TRI_AQL_ACCESS_RANGE_SINGLE && 
              candidate->_type != TRI_AQL_ACCESS_RANGE_DOUBLE &&
              candidate->_type != TRI_AQL_ACCESS_REFERENCE) {
            // wrong access type for skiplists
            continue;
          }
          
          if (candidate->_type == TRI_AQL_ACCESS_LIST && numIndexFields != 1) {
            // we found a list, but the index covers multiple attributes. that means we cannot use list access
            continue;
          }

          candidateIsExact = IsExactCandidate(candidate);

          if ((candidateIsExact && !lastTypeWasExact) ||
              (!candidateIsExact && !lastTypeWasExact)) {
            // if we already had a range query, we cannot check for equality after that
            // if we already had a range query, we cannot check another range after that
            continue;
          }

          lastTypeWasExact = candidateIsExact;

          TRI_PushBackVectorPointer(&matches, candidate);
        }
      }

      // finished iterating over all candidates

      if (matches._length != j + 1) {
        // we already have picked less candidate fields than we should
        break;
      }
    }

    if (matches._length < 1) {
      // nothing found
      continue;
    }

    // we now do or don't have an index candidate in the matches vector
    if (matches._length < numIndexFields && TRI_NeedsFullCoverageIndex(idx)) { 
      // the matches vector does not fully cover the indexed fields, but the index requires it
      continue;
    }

    // if we can use the primary index, we'll use it
    picked = PickIndex(context, picked, idx, &matches);
  }

  TRI_DestroyVectorPointer(&matches);

#ifdef TRI_DEBUG_AQL    
  if (picked) {
    LogIndexString("using", picked->_idx, collectionName);
  }
#endif

  return picked;
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// Local Variables:
// mode: outline-minor
// outline-regexp: "^\\(/// @brief\\|/// {@inheritDoc}\\|/// @addtogroup\\|// --SECTION--\\|/// @\\}\\)"
// End:
