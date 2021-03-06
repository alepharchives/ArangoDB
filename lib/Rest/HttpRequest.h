////////////////////////////////////////////////////////////////////////////////
/// @brief http request
///
/// @file
///
/// DISCLAIMER
///
/// Copyright 2004-2012 triAGENS GmbH, Cologne, Germany
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
/// @author Achim Brandt
/// @author Copyright 2004-2012, triAGENS GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#ifndef TRIAGENS_REST_HTTP_REQUEST_H
#define TRIAGENS_REST_HTTP_REQUEST_H 1

#include "Basics/Common.h"

#include "BasicsC/string-buffer.h"
#include "Rest/ConnectionInfo.h"

// -----------------------------------------------------------------------------
// --SECTION--                                                 class HttpRequest
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Rest
/// @{
////////////////////////////////////////////////////////////////////////////////

namespace triagens {
  namespace rest {

////////////////////////////////////////////////////////////////////////////////
/// @brief http request
///
/// The http server reads the request string from the client and converts it
/// into an instance of this class. An http request object provides methods to
/// inspect the header and parameter fields.
////////////////////////////////////////////////////////////////////////////////

    class  HttpRequest {
      private:
        HttpRequest (HttpRequest const&);
        HttpRequest& operator= (HttpRequest const&);

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                      public types
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Rest
/// @{
////////////////////////////////////////////////////////////////////////////////

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief http request type
////////////////////////////////////////////////////////////////////////////////

        enum HttpRequestType {
          HTTP_REQUEST_DELETE,
          HTTP_REQUEST_GET,
          HTTP_REQUEST_HEAD,
          HTTP_REQUEST_POST,
          HTTP_REQUEST_PUT,
          HTTP_REQUEST_ILLEGAL
        };

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                      constructors and destructors
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Rest
/// @{
////////////////////////////////////////////////////////////////////////////////

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief http request constructor
///
/// Constructs a http request given nothing. You can add the values, the header
/// information, and path information afterwards.
////////////////////////////////////////////////////////////////////////////////

        HttpRequest ();

////////////////////////////////////////////////////////////////////////////////
/// @brief destructor
////////////////////////////////////////////////////////////////////////////////

        virtual ~HttpRequest ();

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                    public methods
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Rest
/// @{
////////////////////////////////////////////////////////////////////////////////

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief returns the server IP
////////////////////////////////////////////////////////////////////////////////

        ConnectionInfo const& connectionInfo () const;

////////////////////////////////////////////////////////////////////////////////
/// @brief sets the server IP
////////////////////////////////////////////////////////////////////////////////

        void setConnectionInfo (ConnectionInfo const& info);

////////////////////////////////////////////////////////////////////////////////
/// @brief returns the http request type
////////////////////////////////////////////////////////////////////////////////

        HttpRequestType requestType () const;

////////////////////////////////////////////////////////////////////////////////
/// @brief sets the http request type
////////////////////////////////////////////////////////////////////////////////

        void setRequestType (HttpRequestType newType);

////////////////////////////////////////////////////////////////////////////////
/// @brief returns the full request path
/// The request path consists of the URL without the host and without any
/// parameters.
////////////////////////////////////////////////////////////////////////////////

        virtual const char* requestPath () const = 0;

////////////////////////////////////////////////////////////////////////////////
/// @brief writes representation to string buffer
////////////////////////////////////////////////////////////////////////////////

        virtual void write (TRI_string_buffer_t*) const = 0;

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                      public prefix/suffix methods
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Rest
/// @{
////////////////////////////////////////////////////////////////////////////////

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief returns the prefix path of the request
///
/// The request path consists of the URL without the host and without any
/// parameters.  The request path is split into two parts: the prefix, namely
/// the part of the request path that was match by a handler and the suffix with
/// all the remaining arguments.
////////////////////////////////////////////////////////////////////////////////

        char const* prefix () const;

////////////////////////////////////////////////////////////////////////////////
/// @brief sets the prefix path of the request
////////////////////////////////////////////////////////////////////////////////

        void setPrefix (char const* path);

////////////////////////////////////////////////////////////////////////////////
/// @brief returns all suffix parts
////////////////////////////////////////////////////////////////////////////////

        vector<string> const& suffix () const;

////////////////////////////////////////////////////////////////////////////////
/// @brief adds a suffix part
////////////////////////////////////////////////////////////////////////////////

        void addSuffix (char const* part);

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                             public header methods
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Rest
/// @{
////////////////////////////////////////////////////////////////////////////////

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief returns the content length
////////////////////////////////////////////////////////////////////////////////

        virtual size_t contentLength () const = 0;

////////////////////////////////////////////////////////////////////////////////
/// @brief returns a header field
///
/// Returns the value of a header field with given name. If no header field
/// with the given name was specified by the client, the empty string is
/// returned.
///
/// @note The @FA{field} must be lowercase.
////////////////////////////////////////////////////////////////////////////////

        virtual char const* header (char const* field) const = 0;

////////////////////////////////////////////////////////////////////////////////
/// @brief returns a header field
///
/// Returns the value of a header field with given name. If no header field
/// with the given name was specified by the client, the empty string is
/// returned. found is try if the client specified the header field.
///
/// @note The @FA{field} must be lowercase.
////////////////////////////////////////////////////////////////////////////////

        virtual char const* header (char const* field, bool& found) const = 0;

////////////////////////////////////////////////////////////////////////////////
/// @brief returns all header fields
///
/// Returns a copy of all header fields.
////////////////////////////////////////////////////////////////////////////////

        virtual map<string, string> headers () const = 0;

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                              public value methods
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Rest
/// @{
////////////////////////////////////////////////////////////////////////////////

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief returns the value of a key
///
/// Returns the value of a key. The empty string is returned if key was not
/// specified by the client.
////////////////////////////////////////////////////////////////////////////////

        virtual char const* value (char const* key) const = 0;

////////////////////////////////////////////////////////////////////////////////
/// @brief returns the value of a key
///
/// Returns the value of a key. The empty string is returned if key was not
/// specified by the client. found is true if the client specified the key.
////////////////////////////////////////////////////////////////////////////////

        virtual char const* value (char const* key, bool& found) const = 0;

////////////////////////////////////////////////////////////////////////////////
/// @brief returns all values
///
/// Returns all key/value pairs of the request.
////////////////////////////////////////////////////////////////////////////////

        virtual map<string, string> values () const = 0;

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                               public body methods
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Rest
/// @{
////////////////////////////////////////////////////////////////////////////////

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief gets the body
////////////////////////////////////////////////////////////////////////////////

        virtual char const* body () const = 0;

////////////////////////////////////////////////////////////////////////////////
/// @brief gets the body size
////////////////////////////////////////////////////////////////////////////////

        virtual size_t bodySize () const = 0;

////////////////////////////////////////////////////////////////////////////////
/// @brief sets the body
////////////////////////////////////////////////////////////////////////////////

        virtual int setBody (char const* newBody, size_t length) = 0;

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                               protected variables
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Rest
/// @{
////////////////////////////////////////////////////////////////////////////////

      protected:

////////////////////////////////////////////////////////////////////////////////
/// @brief connection info for the server and the peer
////////////////////////////////////////////////////////////////////////////////

        ConnectionInfo _connectionInfo;

////////////////////////////////////////////////////////////////////////////////
/// @brief the request type
////////////////////////////////////////////////////////////////////////////////

        HttpRequestType _type;

////////////////////////////////////////////////////////////////////////////////
/// @brief the prefix of the request path
////////////////////////////////////////////////////////////////////////////////

        string _prefix;

////////////////////////////////////////////////////////////////////////////////
/// @brief the suffixes for the request path
////////////////////////////////////////////////////////////////////////////////

        vector<string> _suffix;
    };
  }
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

#endif

// -----------------------------------------------------------------------------
// --SECTION--                                                       END-OF-FILE
// -----------------------------------------------------------------------------

// Local Variables:
// mode: outline-minor
// outline-regexp: "^\\(/// @brief\\|/// {@inheritDoc}\\|/// @addtogroup\\|/// @page\\|// --SECTION--\\|/// @\\}\\)"
// End:
