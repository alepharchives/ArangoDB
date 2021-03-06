////////////////////////////////////////////////////////////////////////////////
/// @brief JSON data container
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
/// @author Jan Steemann
/// @author Copyright 2012, triAGENS GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#ifndef TRIAGENS_FYN_REST_JSON_CONTAINER_H
#define TRIAGENS_FYN_REST_JSON_CONTAINER_H 1

#include "BasicsC/json.h"

// -----------------------------------------------------------------------------
// --SECTION--                                               class JsonContainer
// -----------------------------------------------------------------------------

namespace triagens {
  namespace rest {

////////////////////////////////////////////////////////////////////////////////
/// @brief JSON container
////////////////////////////////////////////////////////////////////////////////

    class JsonContainer {

// -----------------------------------------------------------------------------
// --SECTION--                                      constructors and destructors
// -----------------------------------------------------------------------------

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief constructs a JSON data container
////////////////////////////////////////////////////////////////////////////////

        JsonContainer (TRI_memory_zone_t*, TRI_json_t*);

////////////////////////////////////////////////////////////////////////////////
/// @brief destroys a JSON container
////////////////////////////////////////////////////////////////////////////////

        ~JsonContainer ();

// -----------------------------------------------------------------------------
// --SECTION--                                                    public methods
// -----------------------------------------------------------------------------

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief returns a pointer to the data
////////////////////////////////////////////////////////////////////////////////

        TRI_json_t* ptr () {
          return _data;
        }

// -----------------------------------------------------------------------------
// --SECTION--                                                   private methods
// -----------------------------------------------------------------------------

      private:

////////////////////////////////////////////////////////////////////////////////
/// @brief memory zone
////////////////////////////////////////////////////////////////////////////////

        TRI_memory_zone_t* _memoryZone;

////////////////////////////////////////////////////////////////////////////////
/// @brief the json data
////////////////////////////////////////////////////////////////////////////////

        TRI_json_t* _data;
    };
  }
}

#endif

// Local Variables:
// mode: outline-minor
// outline-regexp: "^\\(/// @brief\\|/// {@inheritDoc}\\|/// @addtogroup\\|// --SECTION--\\|/// @\\}\\)"
// End:
