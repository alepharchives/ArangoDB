////////////////////////////////////////////////////////////////////////////////
/// @brief V8 client connection
///
/// @file
///
/// DISCLAIMER
///
/// Copyright by triAGENS GmbH - All rights reserved.
///
/// The Programs (which include both the software and documentation)
/// contain proprietary information of triAGENS GmbH; they are
/// provided under a license agreement containing restrictions on use and
/// disclosure and are also protected by copyright, patent and other
/// intellectual and industrial property laws. Reverse engineering,
/// disassembly or decompilation of the Programs, except to the extent
/// required to obtain interoperability with other independently created
/// software or as specified by law, is prohibited.
///
/// The Programs are not intended for use in any nuclear, aviation, mass
/// transit, medical, or other inherently dangerous applications. It shall
/// be the licensee's responsibility to take all appropriate fail-safe,
/// backup, redundancy, and other measures to ensure the safe use of such
/// applications if the Programs are used for such purposes, and triAGENS
/// GmbH disclaims liability for any damages caused by such use of
/// the Programs.
///
/// This software is the confidential and proprietary information of
/// triAGENS GmbH. You shall not disclose such confidential and
/// proprietary information and shall use it only in accordance with the
/// terms of the license agreement you entered into with triAGENS GmbH.
///
/// Copyright holder is triAGENS GmbH, Cologne, Germany
///
/// @author Dr. Frank Celler
/// @author Achim Brandt
/// @author Copyright 2008-2011, triagens GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#ifndef TRIAGENS_V8_CLIENT_CONNECTION_H
#define TRIAGENS_V8_CLIENT_CONNECTION_H 1

#include <Basics/Common.h>

#include <v8.h>

// -----------------------------------------------------------------------------
// --SECTION--                                              forward declarations
// -----------------------------------------------------------------------------

namespace triagens {
  namespace httpclient {
    class SimpleHttpClient;
    class SimpleHttpResult;
  }
}

// -----------------------------------------------------------------------------
// --SECTION--                                          class V8ClientConnection
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ArangoDB
/// @{
////////////////////////////////////////////////////////////////////////////////

namespace triagens {
  namespace v8client {

////////////////////////////////////////////////////////////////////////////////
/// @brief class for http requests
////////////////////////////////////////////////////////////////////////////////

    class V8ClientConnection {
      private:
        V8ClientConnection (V8ClientConnection const&);
        V8ClientConnection& operator= (V8ClientConnection const&);

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                      constructors and destructors
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ArangoDB
/// @{
////////////////////////////////////////////////////////////////////////////////

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief constructor
////////////////////////////////////////////////////////////////////////////////

        V8ClientConnection (const string& hostname,
                            const string& username, 
                            const string& password, 
                            int port, 
                            double requestTimeout,
                            size_t retries,
                            double connectionTimeout,
                            bool warn);

////////////////////////////////////////////////////////////////////////////////
/// @brief destructor
////////////////////////////////////////////////////////////////////////////////

        ~V8ClientConnection ();

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                    public methods
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ArangoDB
/// @{
////////////////////////////////////////////////////////////////////////////////

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief returns true if it is connected
////////////////////////////////////////////////////////////////////////////////

        bool isConnected ();

////////////////////////////////////////////////////////////////////////////////
/// @brief returns the version and build number of the arango server
////////////////////////////////////////////////////////////////////////////////

        const string& getVersion ();

////////////////////////////////////////////////////////////////////////////////
/// @brief get the last http return code
///
/// @return int          the code
////////////////////////////////////////////////////////////////////////////////

        int getLastHttpReturnCode ();

////////////////////////////////////////////////////////////////////////////////
/// @brief get the last error message
///
/// @return string          the error message
////////////////////////////////////////////////////////////////////////////////

        const std::string& getErrorMessage ();

////////////////////////////////////////////////////////////////////////////////
/// @brief get the hostname 
///
/// @return string          the server name
////////////////////////////////////////////////////////////////////////////////

        const std::string& getHostname ();
      
////////////////////////////////////////////////////////////////////////////////
/// @brief get the port
///
/// @return string          the server port
////////////////////////////////////////////////////////////////////////////////

        int getPort ();
      
////////////////////////////////////////////////////////////////////////////////
/// @brief get the simple http client
///
/// @return triagens::httpclient::SimpleHttpClient*    then client connection
////////////////////////////////////////////////////////////////////////////////
            
        triagens::httpclient::SimpleHttpClient* getHttpClient();

////////////////////////////////////////////////////////////////////////////////
/// @brief do a "GET" request
///
/// @param string location                     the request location
/// @param map<string, string> headerFields    additional header fields
///
/// @return v8::Value                          a V8 JavaScript object
////////////////////////////////////////////////////////////////////////////////

        v8::Handle<v8::Value> getData (std::string const& location,
                                       map<string, string> const& headerFields);

////////////////////////////////////////////////////////////////////////////////
/// @brief do a "DELETE" request
///
/// @param string location                     the request location
/// @param map<string, string> headerFields    additional header fields
///
/// @return v8::Value                          a V8 JavaScript object
////////////////////////////////////////////////////////////////////////////////

        v8::Handle<v8::Value> deleteData (std::string const& location,
                                          map<string, string> const& headerFields);

////////////////////////////////////////////////////////////////////////////////
/// @brief do a "HEAD" request
///
/// @param string location                     the request location
/// @param map<string, string> headerFields    additional header fields
///
/// @return v8::Value                          a V8 JavaScript object
////////////////////////////////////////////////////////////////////////////////

        v8::Handle<v8::Value> headData (std::string const& location,
                                        map<string, string> const& headerFields);
      
////////////////////////////////////////////////////////////////////////////////
/// @brief do a "POST" request
///
/// @param string location                     the request location
/// @param string body                         the request body
/// @param map<string, string> headerFields    additional header fields
///
/// @return v8::Value                          a V8 JavaScript object
////////////////////////////////////////////////////////////////////////////////

        v8::Handle<v8::Value> postData (std::string const& location,
                                        std::string const& body,
                                        map<string, string> const& headerFields);

////////////////////////////////////////////////////////////////////////////////
/// @brief do a "PUT" request
///
/// @param string location                     the request location
/// @param string body                         the request body
/// @param map<string, string> headerFields    additional header fields
///
/// @return v8::Value                          a V8 JavaScript object
////////////////////////////////////////////////////////////////////////////////

        v8::Handle<v8::Value> putData (std::string const& location,
                                       std::string const& body,
                                       map<string, string> const& headerFields);

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                 private functions
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup V8ClientConnection
/// @{
////////////////////////////////////////////////////////////////////////////////

    private:
      
////////////////////////////////////////////////////////////////////////////////
/// @brief executs a request
////////////////////////////////////////////////////////////////////////////////

      v8::Handle<v8::Value> requestData (int method, 
                                         std::string const& location,
                                         std::string const& body,
                                         map<string, string> const& headerFields);
      
////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                 private variables
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup V8ClientConnection
/// @{
////////////////////////////////////////////////////////////////////////////////

    private:

////////////////////////////////////////////////////////////////////////////////
/// @brief server version
////////////////////////////////////////////////////////////////////////////////

      std::string _version;

////////////////////////////////////////////////////////////////////////////////
/// @brief connection status
////////////////////////////////////////////////////////////////////////////////

      bool _connected;

////////////////////////////////////////////////////////////////////////////////
/// @brief last http return code
////////////////////////////////////////////////////////////////////////////////

      int _lastHttpReturnCode;

////////////////////////////////////////////////////////////////////////////////
/// @brief last error message
////////////////////////////////////////////////////////////////////////////////

      std::string _lastErrorMessage;
      
////////////////////////////////////////////////////////////////////////////////
/// @brief underlying client
////////////////////////////////////////////////////////////////////////////////

      triagens::httpclient::SimpleHttpClient* _client;
      
////////////////////////////////////////////////////////////////////////////////
/// @brief last result
////////////////////////////////////////////////////////////////////////////////

      triagens::httpclient::SimpleHttpResult* _httpResult;
    };
  }
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

#endif

// Local Variables:
// mode: outline-minor
// outline-regexp: "^\\(/// @brief\\|/// {@inheritDoc}\\|/// @addtogroup\\|// --SECTION--\\|/// @\\}\\)"
// End:
