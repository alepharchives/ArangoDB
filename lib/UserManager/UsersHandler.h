////////////////////////////////////////////////////////////////////////////////
/// @brief users handler
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

#ifndef TRIAGENS_USER_MANAGER_USERS_HANDLER_H
#define TRIAGENS_USER_MANAGER_USERS_HANDLER_H 1

#include "Admin/RestAdminBaseHandler.h"

namespace triagens {
  namespace admin {
    class ApplicationAdminServer;
    class User;

////////////////////////////////////////////////////////////////////////////////
/// @brief user handler
////////////////////////////////////////////////////////////////////////////////

    class UsersHandler : public RestAdminBaseHandler {
      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief static constructor
////////////////////////////////////////////////////////////////////////////////

        static rest::HttpHandler* create (rest::HttpRequest* request, void* data) {
          return new UsersHandler(request, (ApplicationAdminServer*) data);
        }

      public:

////////////////////////////////////////////////////////////////////////////////
/// {@inheritDoc}
////////////////////////////////////////////////////////////////////////////////

        bool isDirect () {
          return true;
        }

////////////////////////////////////////////////////////////////////////////////
/// {@inheritDoc}
////////////////////////////////////////////////////////////////////////////////

        status_e execute ();

      protected:

////////////////////////////////////////////////////////////////////////////////
/// @brief constructor
////////////////////////////////////////////////////////////////////////////////

        UsersHandler (rest::HttpRequest* request, ApplicationAdminServer*);

      private:
        ApplicationAdminServer* _server;
    };
  }
}

#endif
