
#ifndef TRIAGENS_DURHAM_VOC_BASE_ERRORS_H
#define TRIAGENS_DURHAM_VOC_BASE_ERRORS_H 1

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
/// @page ArangoErrors Error codes and meanings
///
/// The following errors might be raised when running ArangoDB:
///
/// - 0: @CODE{no error}
///   No error has occurred.
/// - 1: @CODE{failed}
///   Will be raised when a general error occurred.
/// - 2: @CODE{system error}
///   Will be raised when operating system error occurred.
/// - 3: @CODE{out of memory}
///   Will be raised when there is a memory shortage.
/// - 4: @CODE{internal error}
///   Will be raised when an internal error occurred.
/// - 5: @CODE{illegal number}
///   Will be raised when an illegal representation of a number was given.
/// - 6: @CODE{numeric overflow}
///   Will be raised when a numeric overflow occurred.
/// - 7: @CODE{illegal option}
///   Will be raised when an unknown option was supplied by the user.
/// - 8: @CODE{dead process identifier}
///   Will be raised when a PID without a living process was found.
/// - 9: @CODE{not implemented}
///   Will be raised when hitting an unimplemented feature.
/// - 10: @CODE{bad parameter}
///   Will be raised when the parameter does not fulfill the requirements.
/// - 11: @CODE{forbidden}
///   Will be raised when you are missing permission for the operation.
/// - 12: @CODE{out of memory in mmap}
///   Will be raised when there is a memory shortage.
/// - 13: @CODE{csv is corrupt}
///   Will be raised when encountering a corrupt csv line.
/// - 400: @CODE{bad parameter}
///   Will be raised when the HTTP request does not fulfill the requirements.
/// - 403: @CODE{forbidden}
///   Will be raised when the operation is forbidden.
/// - 404: @CODE{not found}
///   Will be raised when an URI is unknown.
/// - 405: @CODE{method not supported}
///   Will be raised when an unsupported HTTP method is used for an operation.
/// - 500: @CODE{internal server error}
///   Will be raised when an internal server is encountered.
/// - 600: @CODE{invalid JSON object}
///   Will be raised when a string representation an JSON object is corrupt.
/// - 601: @CODE{superfluous URL suffices}
///   Will be raised when the URL contains superfluous suffices.
/// - 1000: @CODE{illegal state}
///   Internal error that will be raised when the datafile is not in the
///   required state.
/// - 1001: @CODE{illegal shaper}
///   Internal error that will be raised when the shaper encountered a porblem.
/// - 1002: @CODE{datafile sealed}
///   Internal error that will be raised when trying to write to a datafile.
/// - 1003: @CODE{unknown type}
///   Internal error that will be raised when an unknown collection type is
///   encountered.
/// - 1004: @CODE{ready only}
///   Internal error that will be raised when trying to write to a read-only
///   datafile or collection.
/// - 1005: @CODE{duplicate identifier}
///   Internal error that will be raised when a identifier duplicate is
///   detected.
/// - 1006: @CODE{datafile unreadable}
///   Internal error that will be raised when the datafile is unreadable.
/// - 1100: @CODE{corrupted datafile}
///   Will be raised when a corruption is detected in a datafile.
/// - 1101: @CODE{illegal parameter file}
///   Will be raised if a parameter file is corrupted.
/// - 1102: @CODE{corrupted collection}
///   Will be raised when a collection contains one or more corrupted datafiles.
/// - 1103: @CODE{mmap failed}
///   Will be raised when the system call mmap failed.
/// - 1104: @CODE{filesystem full}
///   Will be raised when the filesystem is full.
/// - 1105: @CODE{no journal}
///   Will be raised when a journal cannot be created.
/// - 1106: @CODE{cannot create/rename datafile because it already exists}
///   Will be raised when the datafile cannot be created or renamed because a
///   file of the same name already exists.
/// - 1107: @CODE{database is locked}
///   Will be raised when the database is locked by a different process.
/// - 1108: @CODE{cannot create/rename collection because directory already exists}
///   Will be raised when the collection cannot be created because a directory
///   of the same name already exists.
/// - 1200: @CODE{conflict}
///   Will be raised when updating or deleting a document and a conflict has
///   been detected.
/// - 1201: @CODE{wrong path for database}
///   Will be raised when a non-existing directory was specified as path for
///   the database.
/// - 1202: @CODE{document not found}
///   Will be raised when a document with a given identifier or handle is
///   unknown.
/// - 1203: @CODE{collection not found}
///   Will be raised when a collection with a given identifier or name is
///   unknown.
/// - 1204: @CODE{parameter 'collection' not found}
///   Will be raised when the collection parameter is missing.
/// - 1205: @CODE{illegal document handle}
///   Will be raised when a document handle is corrupt.
/// - 1206: @CODE{maixaml size of journal too small}
///   Will be raised when the maximal size of the journal is too small.
/// - 1207: @CODE{duplicate name}
///   Will be raised when a name duplicate is detected.
/// - 1208: @CODE{illegal name}
///   Will be raised when an illegal name is detected.
/// - 1209: @CODE{no suitable index known}
///   Will be raised when no suitable index for the query is known.
/// - 1210: @CODE{unique constraint violated}
///   Will be raised when there is a unique constraint violation.
/// - 1211: @CODE{geo index violated}
///   Will be raised when a illegale coordinate is used.
/// - 1212: @CODE{index not found}
///   Will be raised when an index with a given identifier is unknown.
/// - 1213: @CODE{cross collection request not allowed}
///   Will be raised when a cross-collection is requested.
/// - 1214: @CODE{illegal index handle}
///   Will be raised when a index handle is corrupt.
/// - 1215: @CODE{cap constraint already defined}
///   Will be raised when a cap constraint was already defined.
/// - 1216: @CODE{document too large}
///   Will be raised when the document cannot fit into any datafile because of
///   it is too large.
/// - 1217: @CODE{collection must be unloaded}
///   Will be raised when a collection should be unloaded, but has a different
///   status.
/// - 1300: @CODE{datafile full}
///   Will be raised when the datafile reaches its limit.
/// - 1500: @CODE{query killed}
///   Will be raised when a running query is killed by an explicit admin
///   command.
/// - 1501: @CODE{\%s}
///   Will be raised when query is parsed and is found to be syntactially
///   invalid.
/// - 1502: @CODE{query is empty}
///   Will be raised when an empty query is specified.
/// - 1503: @CODE{runtime error '\%s'}
///   Will be raised when a runtime error is caused by the query.
/// - 1504: @CODE{number out of range}
///   Will be raised when a number is outside the expected range.
/// - 1510: @CODE{variable name '\%s' has an invalid format}
///   Will be raised when an invalid variable name is used.
/// - 1511: @CODE{variable '\%s' is assigned multiple times}
///   Will be raised when a variable gets re-assigned in a query.
/// - 1520: @CODE{unable to open collection '\%s'}
///   Will be raised when one of the collections referenced in the query was
///   not found.
/// - 1521: @CODE{unable to read-lock collection \%s}
///   Will be raised when a read lock on the collection cannot be acquired.
/// - 1522: @CODE{too many collections}
///   Will be raised when the number of collections in a query is beyond the
///   allowed value.
/// - 1530: @CODE{document attribute '\%s' is assigned multiple times}
///   Will be raised when a document attribute is re-assigned.
/// - 1540: @CODE{usage of unknown function '\%s'}
///   Will be raised when an undefined function is called.
/// - 1541: @CODE{invalid number of arguments for function '\%s'}
///   Will be raised when the number of arguments used in a function call does
///   not match the expected number of arguments for the function.
/// - 1542: @CODE{invalid argument type used in call to function '\%s()'}
///   Will be raised when the type of an argument used in a function call does
///   not match the expected argument type.
/// - 1550: @CODE{invalid structure of bind parameters}
///   Will be raised when the structure of bind parameters passed has an
///   unexpected format.
/// - 1551: @CODE{no value specified for declared bind parameter '\%s'}
///   Will be raised when a bind parameter was declared in the query but the
///   query is being executed with no value for that parameter.
/// - 1552: @CODE{bind parameter '\%s' was not declared in the query}
///   Will be raised when a value gets specified for an undeclared bind
///   parameter.
/// - 1553: @CODE{bind parameter '\%s' has an invalid value or type}
///   Will be raised when a bind parameter has an invalid value or type.
/// - 1560: @CODE{invalid logical value}
///   Will be raised when a non-boolean value is used in a logical operation.
/// - 1561: @CODE{invalid arithmetic value}
///   Will be raised when a non-numeric value is used in an arithmetic
///   operation.
/// - 1562: @CODE{division by zero}
///   Will be raised when there is an attempt to divide by zero.
/// - 1563: @CODE{list expected}
///   Will be raised when a non-list operand is used for an operation that
///   expects a list argument operand.
/// - 1569: @CODE{FAIL(\%s) called}
///   Will be raised when the function FAIL() is called from inside a query.
/// - 1570: @CODE{no suitable geo index found for geo restriction on '\%s'}
///   Will be raised when a geo restriction was specified but no suitable geo
///   index is found to resolve it.
/// - 1600: @CODE{cursor not found}
///   Will be raised when a cursor is requested via its id but a cursor with
///   that id cannot be found.
/// - 1700: @CODE{expecting \<prefix\>/user/\<username\>}
///   TODO
/// - 1701: @CODE{cannot create user}
///   TODO
/// - 1702: @CODE{role not found}
///   TODO
/// - 1703: @CODE{no permission to create user with that role}
///   TODO
/// - 1704: @CODE{user not found}
///   TODO
/// - 1705: @CODE{cannot manage password for user}
///   TODO
/// - 1706: @CODE{expecting POST \<prefix\>/session}
///   TODO
/// - 1707: @CODE{expecting GET \<prefix\>/session/\<sid\>}
///   TODO
/// - 1708: @CODE{expecting PUT \<prefix\>/session/\<sid\>/\<method\>}
///   TODO
/// - 1709: @CODE{expecting DELETE \<prefix\>/session/\<sid\>}
///   TODO
/// - 1710: @CODE{unknown session}
///   TODO
/// - 1711: @CODE{session has not bound to user}
///   TODO
/// - 1712: @CODE{cannot login with session}
///   TODO
/// - 1713: @CODE{expecting GET \<prefix\>/users}
///   TODO
/// - 1714: @CODE{expecting /directory/sessionvoc/\<token\>}
///   TODO
/// - 1715: @CODE{directory server is not configured}
///   TODO
/// - 1800: @CODE{invalid key declaration}
///   Will be raised when an invalid key specification is passed to the server
/// - 1801: @CODE{key already exists}
///   Will be raised when a key is to be created that already exists
/// - 1802: @CODE{key not found}
///   Will be raised when the specified key is not found
/// - 1803: @CODE{key is not unique}
///   Will be raised when the specified key is not unique
/// - 1804: @CODE{key value not changed}
///   Will be raised when updating the value for a key does not work
/// - 1805: @CODE{key value not removed}
///   Will be raised when deleting a key/value pair does not work
/// - 1806: @CODE{missing value}
///   Will be raised when the value is missing
/// - 1901: @CODE{invalid graph}
///   Will be raised when an invalid name is passed to the server
/// - 1902: @CODE{could not create graph}
///   Will be raised when an invalid name, vertices or edges is passed to the
///   server
/// - 1903: @CODE{invalid vertex}
///   Will be raised when an invalid vertex id is passed to the server
/// - 1904: @CODE{could not create vertex}
///   Will be raised when the vertex could not be created
/// - 1905: @CODE{could not change vertex}
///   Will be raised when the vertex could not be changed
/// - 1906: @CODE{invalid edge}
///   Will be raised when an invalid edge id is passed to the server
/// - 1907: @CODE{could not create edge}
///   Will be raised when the edge could not be created
/// - 1908: @CODE{could not change edge}
///   Will be raised when the edge could not be changed
/// - 2000: @CODE{unknown client error}
///   This error should not happen.
/// - 2001: @CODE{could not connect to server}
///   Will be raised when the client could not connect to the server.
/// - 2002: @CODE{could not write to server}
///   Will be raised when the client could not write data.
/// - 2003: @CODE{could not read from server}
///   Will be raised when the client could not read data.
/// - 3100: @CODE{priority queue insert failure}
///   Will be raised when an attempt to insert a document into a priority queue
///   index fails for some reason.
/// - 3110: @CODE{priority queue remove failure}
///   Will be raised when an attempt to remove a document from a priority queue
///   index fails for some reason.
/// - 3111: @CODE{priority queue remove failure - item missing in index}
///   Will be raised when an attempt to remove a document from a priority queue
///   index fails when document can not be located within the index.
/// - 3312: @CODE{(non-unique) hash index insert failure - document duplicated in index}
///   Will be raised when an attempt to insert a document into a non-unique
///   hash index fails due to the fact that document is duplicated within that
///   index.
/// - 3313: @CODE{(non-unique) skiplist index insert failure - document duplicated in index}
///   Will be raised when an attempt to insert a document into a non-unique
///   skiplist index fails due to the fact that document is duplicated within
///   that index.
/// - 3200: @CODE{hash index insertion warning - attribute missing in document}
///   Will be raised when an attempt to insert a document into a hash index is
///   caused by the document not having one or more attributes which are
///   required by the hash index.
/// - 3202: @CODE{hash index update warning - attribute missing in revised document}
///   Will be raised when an attempt to update a document results in the
///   revised document not having one or more attributes which are required by
///   the hash index.
/// - 3211: @CODE{hash index remove failure - item missing in index}
///   Will be raised when an attempt to remove a document from a hash index
///   fails when document can not be located within that index.
/// - 3300: @CODE{skiplist index insertion warning - attribute missing in document}
///   Will be raised when an attempt to insert a document into a skiplist index
///   is caused by in the document not having one or more attributes which are
///   required by the skiplist index.
/// - 3302: @CODE{skiplist index update warning - attribute missing in revised document}
///   Will be raised when an attempt to update a document results in the
///   revised document not having one or more attributes which are required by
///   the skiplist index.
/// - 3311: @CODE{skiplist index remove failure - item missing in index}
///   Will be raised when an attempt to remove a document from a skiplist index
///   fails when document can not be located within that index.
/// - 3400: @CODE{bitarray index insertion warning - attribute missing in document}
///   Will be raised when an attempt to insert a document into a bitarray index
///   is caused by in the document not having one or more attributes which are
///   required by the bitarray index.
/// - 3402: @CODE{bitarray index update warning - attribute missing in revised document}
///   Will be raised when an attempt to update a document results in the
///   revised document not having one or more attributes which are required by
///   the bitarray index.
/// - 3411: @CODE{bitarray index remove failure - item missing in index}
///   Will be raised when an attempt to remove a document from a bitarray index
///   fails when document can not be located within that index.
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup VocError
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief helper macro to define an error string
////////////////////////////////////////////////////////////////////////////////

#define REG_ERROR(id, label) TRI_set_errno_string(TRI_ ## id, label);

////////////////////////////////////////////////////////////////////////////////
/// @brief register all errors for ArangoDB
////////////////////////////////////////////////////////////////////////////////

void TRI_InitialiseErrorMessages (void);

////////////////////////////////////////////////////////////////////////////////
/// @brief 0: ERROR_NO_ERROR
///
/// no error
///
/// No error has occurred.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_NO_ERROR                                                (0)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1: ERROR_FAILED
///
/// failed
///
/// Will be raised when a general error occurred.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_FAILED                                                  (1)

////////////////////////////////////////////////////////////////////////////////
/// @brief 2: ERROR_SYS_ERROR
///
/// system error
///
/// Will be raised when operating system error occurred.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_SYS_ERROR                                               (2)

////////////////////////////////////////////////////////////////////////////////
/// @brief 3: ERROR_OUT_OF_MEMORY
///
/// out of memory
///
/// Will be raised when there is a memory shortage.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_OUT_OF_MEMORY                                           (3)

////////////////////////////////////////////////////////////////////////////////
/// @brief 4: ERROR_INTERNAL
///
/// internal error
///
/// Will be raised when an internal error occurred.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_INTERNAL                                                (4)

////////////////////////////////////////////////////////////////////////////////
/// @brief 5: ERROR_ILLEGAL_NUMBER
///
/// illegal number
///
/// Will be raised when an illegal representation of a number was given.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ILLEGAL_NUMBER                                          (5)

////////////////////////////////////////////////////////////////////////////////
/// @brief 6: ERROR_NUMERIC_OVERFLOW
///
/// numeric overflow
///
/// Will be raised when a numeric overflow occurred.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_NUMERIC_OVERFLOW                                        (6)

////////////////////////////////////////////////////////////////////////////////
/// @brief 7: ERROR_ILLEGAL_OPTION
///
/// illegal option
///
/// Will be raised when an unknown option was supplied by the user.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ILLEGAL_OPTION                                          (7)

////////////////////////////////////////////////////////////////////////////////
/// @brief 8: ERROR_DEAD_PID
///
/// dead process identifier
///
/// Will be raised when a PID without a living process was found.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_DEAD_PID                                                (8)

////////////////////////////////////////////////////////////////////////////////
/// @brief 9: ERROR_NOT_IMPLEMENTED
///
/// not implemented
///
/// Will be raised when hitting an unimplemented feature.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_NOT_IMPLEMENTED                                         (9)

////////////////////////////////////////////////////////////////////////////////
/// @brief 10: ERROR_BAD_PARAMETER
///
/// bad parameter
///
/// Will be raised when the parameter does not fulfill the requirements.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_BAD_PARAMETER                                           (10)

////////////////////////////////////////////////////////////////////////////////
/// @brief 11: ERROR_FORBIDDEN
///
/// forbidden
///
/// Will be raised when you are missing permission for the operation.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_FORBIDDEN                                               (11)

////////////////////////////////////////////////////////////////////////////////
/// @brief 12: ERROR_OUT_OF_MEMORY_MMAP
///
/// out of memory in mmap
///
/// Will be raised when there is a memory shortage.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_OUT_OF_MEMORY_MMAP                                      (12)

////////////////////////////////////////////////////////////////////////////////
/// @brief 13: ERROR_CORRUPTED_CSV
///
/// csv is corrupt
///
/// Will be raised when encountering a corrupt csv line.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_CORRUPTED_CSV                                           (13)

////////////////////////////////////////////////////////////////////////////////
/// @brief 400: ERROR_HTTP_BAD_PARAMETER
///
/// bad parameter
///
/// Will be raised when the HTTP request does not fulfill the requirements.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_HTTP_BAD_PARAMETER                                      (400)

////////////////////////////////////////////////////////////////////////////////
/// @brief 403: ERROR_HTTP_FORBIDDEN
///
/// forbidden
///
/// Will be raised when the operation is forbidden.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_HTTP_FORBIDDEN                                          (403)

////////////////////////////////////////////////////////////////////////////////
/// @brief 404: ERROR_HTTP_NOT_FOUND
///
/// not found
///
/// Will be raised when an URI is unknown.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_HTTP_NOT_FOUND                                          (404)

////////////////////////////////////////////////////////////////////////////////
/// @brief 405: ERROR_HTTP_METHOD_NOT_ALLOWED
///
/// method not supported
///
/// Will be raised when an unsupported HTTP method is used for an operation.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_HTTP_METHOD_NOT_ALLOWED                                 (405)

////////////////////////////////////////////////////////////////////////////////
/// @brief 500: ERROR_HTTP_SERVER_ERROR
///
/// internal server error
///
/// Will be raised when an internal server is encountered.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_HTTP_SERVER_ERROR                                       (500)

////////////////////////////////////////////////////////////////////////////////
/// @brief 600: ERROR_HTTP_CORRUPTED_JSON
///
/// invalid JSON object
///
/// Will be raised when a string representation an JSON object is corrupt.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_HTTP_CORRUPTED_JSON                                     (600)

////////////////////////////////////////////////////////////////////////////////
/// @brief 601: ERROR_HTTP_SUPERFLUOUS_SUFFICES
///
/// superfluous URL suffices
///
/// Will be raised when the URL contains superfluous suffices.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_HTTP_SUPERFLUOUS_SUFFICES                               (601)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1000: ERROR_ARANGO_ILLEGAL_STATE
///
/// illegal state
///
/// Internal error that will be raised when the datafile is not in the required
/// state.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_ILLEGAL_STATE                                    (1000)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1001: ERROR_ARANGO_SHAPER_FAILED
///
/// illegal shaper
///
/// Internal error that will be raised when the shaper encountered a porblem.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_SHAPER_FAILED                                    (1001)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1002: ERROR_ARANGO_DATAFILE_SEALED
///
/// datafile sealed
///
/// Internal error that will be raised when trying to write to a datafile.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_DATAFILE_SEALED                                  (1002)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1003: ERROR_ARANGO_UNKNOWN_COLLECTION_TYPE
///
/// unknown type
///
/// Internal error that will be raised when an unknown collection type is
/// encountered.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_UNKNOWN_COLLECTION_TYPE                          (1003)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1004: ERROR_ARANGO_READ_ONLY
///
/// ready only
///
/// Internal error that will be raised when trying to write to a read-only
/// datafile or collection.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_READ_ONLY                                        (1004)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1005: ERROR_ARANGO_DUPLICATE_IDENTIFIER
///
/// duplicate identifier
///
/// Internal error that will be raised when a identifier duplicate is detected.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_DUPLICATE_IDENTIFIER                             (1005)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1006: ERROR_ARANGO_DATAFILE_UNREADABLE
///
/// datafile unreadable
///
/// Internal error that will be raised when the datafile is unreadable.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_DATAFILE_UNREADABLE                              (1006)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1100: ERROR_ARANGO_CORRUPTED_DATAFILE
///
/// corrupted datafile
///
/// Will be raised when a corruption is detected in a datafile.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_CORRUPTED_DATAFILE                               (1100)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1101: ERROR_ARANGO_ILLEGAL_PARAMETER_FILE
///
/// illegal parameter file
///
/// Will be raised if a parameter file is corrupted.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_ILLEGAL_PARAMETER_FILE                           (1101)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1102: ERROR_ARANGO_CORRUPTED_COLLECTION
///
/// corrupted collection
///
/// Will be raised when a collection contains one or more corrupted datafiles.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_CORRUPTED_COLLECTION                             (1102)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1103: ERROR_ARANGO_MMAP_FAILED
///
/// mmap failed
///
/// Will be raised when the system call mmap failed.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_MMAP_FAILED                                      (1103)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1104: ERROR_ARANGO_FILESYSTEM_FULL
///
/// filesystem full
///
/// Will be raised when the filesystem is full.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_FILESYSTEM_FULL                                  (1104)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1105: ERROR_ARANGO_NO_JOURNAL
///
/// no journal
///
/// Will be raised when a journal cannot be created.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_NO_JOURNAL                                       (1105)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1106: ERROR_ARANGO_DATAFILE_ALREADY_EXISTS
///
/// cannot create/rename datafile because it already exists
///
/// Will be raised when the datafile cannot be created or renamed because a
/// file of the same name already exists.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_DATAFILE_ALREADY_EXISTS                          (1106)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1107: ERROR_ARANGO_DATABASE_LOCKED
///
/// database is locked
///
/// Will be raised when the database is locked by a different process.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_DATABASE_LOCKED                                  (1107)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1108: ERROR_ARANGO_COLLECTION_DIRECTORY_ALREADY_EXISTS
///
/// cannot create/rename collection because directory already exists
///
/// Will be raised when the collection cannot be created because a directory of
/// the same name already exists.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_COLLECTION_DIRECTORY_ALREADY_EXISTS              (1108)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1200: ERROR_ARANGO_CONFLICT
///
/// conflict
///
/// Will be raised when updating or deleting a document and a conflict has been
/// detected.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_CONFLICT                                         (1200)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1201: ERROR_ARANGO_WRONG_VOCBASE_PATH
///
/// wrong path for database
///
/// Will be raised when a non-existing directory was specified as path for the
/// database.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_WRONG_VOCBASE_PATH                               (1201)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1202: ERROR_ARANGO_DOCUMENT_NOT_FOUND
///
/// document not found
///
/// Will be raised when a document with a given identifier or handle is unknown.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_DOCUMENT_NOT_FOUND                               (1202)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1203: ERROR_ARANGO_COLLECTION_NOT_FOUND
///
/// collection not found
///
/// Will be raised when a collection with a given identifier or name is unknown.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_COLLECTION_NOT_FOUND                             (1203)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1204: ERROR_ARANGO_COLLECTION_PARAMETER_MISSING
///
/// parameter 'collection' not found
///
/// Will be raised when the collection parameter is missing.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_COLLECTION_PARAMETER_MISSING                     (1204)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1205: ERROR_ARANGO_DOCUMENT_HANDLE_BAD
///
/// illegal document handle
///
/// Will be raised when a document handle is corrupt.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_DOCUMENT_HANDLE_BAD                              (1205)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1206: ERROR_ARANGO_MAXIMAL_SIZE_TOO_SMALL
///
/// maixaml size of journal too small
///
/// Will be raised when the maximal size of the journal is too small.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_MAXIMAL_SIZE_TOO_SMALL                           (1206)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1207: ERROR_ARANGO_DUPLICATE_NAME
///
/// duplicate name
///
/// Will be raised when a name duplicate is detected.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_DUPLICATE_NAME                                   (1207)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1208: ERROR_ARANGO_ILLEGAL_NAME
///
/// illegal name
///
/// Will be raised when an illegal name is detected.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_ILLEGAL_NAME                                     (1208)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1209: ERROR_ARANGO_NO_INDEX
///
/// no suitable index known
///
/// Will be raised when no suitable index for the query is known.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_NO_INDEX                                         (1209)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1210: ERROR_ARANGO_UNIQUE_CONSTRAINT_VIOLATED
///
/// unique constraint violated
///
/// Will be raised when there is a unique constraint violation.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_UNIQUE_CONSTRAINT_VIOLATED                       (1210)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1211: ERROR_ARANGO_GEO_INDEX_VIOLATED
///
/// geo index violated
///
/// Will be raised when a illegale coordinate is used.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_GEO_INDEX_VIOLATED                               (1211)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1212: ERROR_ARANGO_INDEX_NOT_FOUND
///
/// index not found
///
/// Will be raised when an index with a given identifier is unknown.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_INDEX_NOT_FOUND                                  (1212)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1213: ERROR_ARANGO_CROSS_COLLECTION_REQUEST
///
/// cross collection request not allowed
///
/// Will be raised when a cross-collection is requested.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_CROSS_COLLECTION_REQUEST                         (1213)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1214: ERROR_ARANGO_INDEX_HANDLE_BAD
///
/// illegal index handle
///
/// Will be raised when a index handle is corrupt.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_INDEX_HANDLE_BAD                                 (1214)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1215: ERROR_ARANGO_CAP_CONSTRAINT_ALREADY_DEFINED
///
/// cap constraint already defined
///
/// Will be raised when a cap constraint was already defined.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_CAP_CONSTRAINT_ALREADY_DEFINED                   (1215)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1216: ERROR_ARANGO_DOCUMENT_TOO_LARGE
///
/// document too large
///
/// Will be raised when the document cannot fit into any datafile because of it
/// is too large.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_DOCUMENT_TOO_LARGE                               (1216)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1217: ERROR_ARANGO_COLLECTION_NOT_UNLOADED
///
/// collection must be unloaded
///
/// Will be raised when a collection should be unloaded, but has a different
/// status.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_COLLECTION_NOT_UNLOADED                          (1217)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1300: ERROR_ARANGO_DATAFILE_FULL
///
/// datafile full
///
/// Will be raised when the datafile reaches its limit.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_DATAFILE_FULL                                    (1300)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1500: ERROR_QUERY_KILLED
///
/// query killed
///
/// Will be raised when a running query is killed by an explicit admin command.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_QUERY_KILLED                                            (1500)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1501: ERROR_QUERY_PARSE
///
/// %s
///
/// Will be raised when query is parsed and is found to be syntactially invalid.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_QUERY_PARSE                                             (1501)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1502: ERROR_QUERY_EMPTY
///
/// query is empty
///
/// Will be raised when an empty query is specified.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_QUERY_EMPTY                                             (1502)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1503: ERROR_QUERY_SCRIPT
///
/// runtime error '%s'
///
/// Will be raised when a runtime error is caused by the query.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_QUERY_SCRIPT                                            (1503)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1504: ERROR_QUERY_NUMBER_OUT_OF_RANGE
///
/// number out of range
///
/// Will be raised when a number is outside the expected range.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_QUERY_NUMBER_OUT_OF_RANGE                               (1504)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1510: ERROR_QUERY_VARIABLE_NAME_INVALID
///
/// variable name '%s' has an invalid format
///
/// Will be raised when an invalid variable name is used.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_QUERY_VARIABLE_NAME_INVALID                             (1510)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1511: ERROR_QUERY_VARIABLE_REDECLARED
///
/// variable '%s' is assigned multiple times
///
/// Will be raised when a variable gets re-assigned in a query.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_QUERY_VARIABLE_REDECLARED                               (1511)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1520: ERROR_QUERY_COLLECTION_NOT_FOUND
///
/// unable to open collection '%s'
///
/// Will be raised when one of the collections referenced in the query was not
/// found.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_QUERY_COLLECTION_NOT_FOUND                              (1520)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1521: ERROR_QUERY_COLLECTION_LOCK_FAILED
///
/// unable to read-lock collection %s
///
/// Will be raised when a read lock on the collection cannot be acquired.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_QUERY_COLLECTION_LOCK_FAILED                            (1521)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1522: ERROR_QUERY_TOO_MANY_COLLECTIONS
///
/// too many collections
///
/// Will be raised when the number of collections in a query is beyond the
/// allowed value.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_QUERY_TOO_MANY_COLLECTIONS                              (1522)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1530: ERROR_QUERY_DOCUMENT_ATTRIBUTE_REDECLARED
///
/// document attribute '%s' is assigned multiple times
///
/// Will be raised when a document attribute is re-assigned.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_QUERY_DOCUMENT_ATTRIBUTE_REDECLARED                     (1530)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1540: ERROR_QUERY_FUNCTION_NAME_UNKNOWN
///
/// usage of unknown function '%s'
///
/// Will be raised when an undefined function is called.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_QUERY_FUNCTION_NAME_UNKNOWN                             (1540)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1541: ERROR_QUERY_FUNCTION_ARGUMENT_NUMBER_MISMATCH
///
/// invalid number of arguments for function '%s'
///
/// Will be raised when the number of arguments used in a function call does
/// not match the expected number of arguments for the function.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_QUERY_FUNCTION_ARGUMENT_NUMBER_MISMATCH                 (1541)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1542: ERROR_QUERY_FUNCTION_ARGUMENT_TYPE_MISMATCH
///
/// invalid argument type used in call to function '%s()'
///
/// Will be raised when the type of an argument used in a function call does
/// not match the expected argument type.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_QUERY_FUNCTION_ARGUMENT_TYPE_MISMATCH                   (1542)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1550: ERROR_QUERY_BIND_PARAMETERS_INVALID
///
/// invalid structure of bind parameters
///
/// Will be raised when the structure of bind parameters passed has an
/// unexpected format.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_QUERY_BIND_PARAMETERS_INVALID                           (1550)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1551: ERROR_QUERY_BIND_PARAMETER_MISSING
///
/// no value specified for declared bind parameter '%s'
///
/// Will be raised when a bind parameter was declared in the query but the
/// query is being executed with no value for that parameter.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_QUERY_BIND_PARAMETER_MISSING                            (1551)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1552: ERROR_QUERY_BIND_PARAMETER_UNDECLARED
///
/// bind parameter '%s' was not declared in the query
///
/// Will be raised when a value gets specified for an undeclared bind parameter.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_QUERY_BIND_PARAMETER_UNDECLARED                         (1552)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1553: ERROR_QUERY_BIND_PARAMETER_TYPE
///
/// bind parameter '%s' has an invalid value or type
///
/// Will be raised when a bind parameter has an invalid value or type.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_QUERY_BIND_PARAMETER_TYPE                               (1553)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1560: ERROR_QUERY_INVALID_LOGICAL_VALUE
///
/// invalid logical value
///
/// Will be raised when a non-boolean value is used in a logical operation.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_QUERY_INVALID_LOGICAL_VALUE                             (1560)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1561: ERROR_QUERY_INVALID_ARITHMETIC_VALUE
///
/// invalid arithmetic value
///
/// Will be raised when a non-numeric value is used in an arithmetic operation.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_QUERY_INVALID_ARITHMETIC_VALUE                          (1561)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1562: ERROR_QUERY_DIVISION_BY_ZERO
///
/// division by zero
///
/// Will be raised when there is an attempt to divide by zero.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_QUERY_DIVISION_BY_ZERO                                  (1562)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1563: ERROR_QUERY_LIST_EXPECTED
///
/// list expected
///
/// Will be raised when a non-list operand is used for an operation that
/// expects a list argument operand.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_QUERY_LIST_EXPECTED                                     (1563)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1569: ERROR_QUERY_FAIL_CALLED
///
/// FAIL(%s) called
///
/// Will be raised when the function FAIL() is called from inside a query.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_QUERY_FAIL_CALLED                                       (1569)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1570: ERROR_QUERY_GEO_INDEX_MISSING
///
/// no suitable geo index found for geo restriction on '%s'
///
/// Will be raised when a geo restriction was specified but no suitable geo
/// index is found to resolve it.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_QUERY_GEO_INDEX_MISSING                                 (1570)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1600: ERROR_CURSOR_NOT_FOUND
///
/// cursor not found
///
/// Will be raised when a cursor is requested via its id but a cursor with that
/// id cannot be found.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_CURSOR_NOT_FOUND                                        (1600)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1700: ERROR_SESSION_USERHANDLER_URL_INVALID
///
/// expecting \<prefix\>/user/\<username\>
///
/// TODO
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_SESSION_USERHANDLER_URL_INVALID                         (1700)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1701: ERROR_SESSION_USERHANDLER_CANNOT_CREATE_USER
///
/// cannot create user
///
/// TODO
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_SESSION_USERHANDLER_CANNOT_CREATE_USER                  (1701)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1702: ERROR_SESSION_USERHANDLER_ROLE_NOT_FOUND
///
/// role not found
///
/// TODO
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_SESSION_USERHANDLER_ROLE_NOT_FOUND                      (1702)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1703: ERROR_SESSION_USERHANDLER_NO_CREATE_PERMISSION
///
/// no permission to create user with that role
///
/// TODO
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_SESSION_USERHANDLER_NO_CREATE_PERMISSION                (1703)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1704: ERROR_SESSION_USERHANDLER_USER_NOT_FOUND
///
/// user not found
///
/// TODO
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_SESSION_USERHANDLER_USER_NOT_FOUND                      (1704)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1705: ERROR_SESSION_USERHANDLER_CANNOT_CHANGE_PW
///
/// cannot manage password for user
///
/// TODO
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_SESSION_USERHANDLER_CANNOT_CHANGE_PW                    (1705)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1706: ERROR_SESSION_SESSIONHANDLER_URL_INVALID1
///
/// expecting POST \<prefix\>/session
///
/// TODO
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_SESSION_SESSIONHANDLER_URL_INVALID1                     (1706)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1707: ERROR_SESSION_SESSIONHANDLER_URL_INVALID2
///
/// expecting GET \<prefix\>/session/\<sid\>
///
/// TODO
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_SESSION_SESSIONHANDLER_URL_INVALID2                     (1707)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1708: ERROR_SESSION_SESSIONHANDLER_URL_INVALID3
///
/// expecting PUT \<prefix\>/session/\<sid\>/\<method\>
///
/// TODO
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_SESSION_SESSIONHANDLER_URL_INVALID3                     (1708)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1709: ERROR_SESSION_SESSIONHANDLER_URL_INVALID4
///
/// expecting DELETE \<prefix\>/session/\<sid\>
///
/// TODO
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_SESSION_SESSIONHANDLER_URL_INVALID4                     (1709)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1710: ERROR_SESSION_SESSIONHANDLER_SESSION_UNKNOWN
///
/// unknown session
///
/// TODO
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_SESSION_SESSIONHANDLER_SESSION_UNKNOWN                  (1710)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1711: ERROR_SESSION_SESSIONHANDLER_SESSION_NOT_BOUND
///
/// session has not bound to user
///
/// TODO
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_SESSION_SESSIONHANDLER_SESSION_NOT_BOUND                (1711)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1712: ERROR_SESSION_SESSIONHANDLER_CANNOT_LOGIN
///
/// cannot login with session
///
/// TODO
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_SESSION_SESSIONHANDLER_CANNOT_LOGIN                     (1712)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1713: ERROR_SESSION_USERSHANDLER_INVALID_URL
///
/// expecting GET \<prefix\>/users
///
/// TODO
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_SESSION_USERSHANDLER_INVALID_URL                        (1713)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1714: ERROR_SESSION_DIRECTORYSERVER_INVALID_URL
///
/// expecting /directory/sessionvoc/\<token\>
///
/// TODO
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_SESSION_DIRECTORYSERVER_INVALID_URL                     (1714)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1715: ERROR_SESSION_DIRECTORYSERVER_NOT_CONFIGURED
///
/// directory server is not configured
///
/// TODO
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_SESSION_DIRECTORYSERVER_NOT_CONFIGURED                  (1715)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1800: ERROR_KEYVALUE_INVALID_KEY
///
/// invalid key declaration
///
/// Will be raised when an invalid key specification is passed to the server
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_KEYVALUE_INVALID_KEY                                    (1800)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1801: ERROR_KEYVALUE_KEY_EXISTS
///
/// key already exists
///
/// Will be raised when a key is to be created that already exists
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_KEYVALUE_KEY_EXISTS                                     (1801)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1802: ERROR_KEYVALUE_KEY_NOT_FOUND
///
/// key not found
///
/// Will be raised when the specified key is not found
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_KEYVALUE_KEY_NOT_FOUND                                  (1802)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1803: ERROR_KEYVALUE_KEY_NOT_UNIQUE
///
/// key is not unique
///
/// Will be raised when the specified key is not unique
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_KEYVALUE_KEY_NOT_UNIQUE                                 (1803)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1804: ERROR_KEYVALUE_KEY_NOT_CHANGED
///
/// key value not changed
///
/// Will be raised when updating the value for a key does not work
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_KEYVALUE_KEY_NOT_CHANGED                                (1804)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1805: ERROR_KEYVALUE_KEY_NOT_REMOVED
///
/// key value not removed
///
/// Will be raised when deleting a key/value pair does not work
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_KEYVALUE_KEY_NOT_REMOVED                                (1805)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1806: ERROR_KEYVALUE_NO_VALUE
///
/// missing value
///
/// Will be raised when the value is missing
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_KEYVALUE_NO_VALUE                                       (1806)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1901: ERROR_GRAPH_INVALID_GRAPH
///
/// invalid graph
///
/// Will be raised when an invalid name is passed to the server
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_GRAPH_INVALID_GRAPH                                     (1901)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1902: ERROR_GRAPH_COULD_NOT_CREATE_GRAPH
///
/// could not create graph
///
/// Will be raised when an invalid name, vertices or edges is passed to the
/// server
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_GRAPH_COULD_NOT_CREATE_GRAPH                            (1902)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1903: ERROR_GRAPH_INVALID_VERTEX
///
/// invalid vertex
///
/// Will be raised when an invalid vertex id is passed to the server
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_GRAPH_INVALID_VERTEX                                    (1903)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1904: ERROR_GRAPH_COULD_NOT_CREATE_VERTEX
///
/// could not create vertex
///
/// Will be raised when the vertex could not be created
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_GRAPH_COULD_NOT_CREATE_VERTEX                           (1904)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1905: ERROR_GRAPH_COULD_NOT_CHANGE_VERTEX
///
/// could not change vertex
///
/// Will be raised when the vertex could not be changed
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_GRAPH_COULD_NOT_CHANGE_VERTEX                           (1905)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1906: ERROR_GRAPH_INVALID_EDGE
///
/// invalid edge
///
/// Will be raised when an invalid edge id is passed to the server
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_GRAPH_INVALID_EDGE                                      (1906)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1907: ERROR_GRAPH_COULD_NOT_CREATE_EDGE
///
/// could not create edge
///
/// Will be raised when the edge could not be created
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_GRAPH_COULD_NOT_CREATE_EDGE                             (1907)

////////////////////////////////////////////////////////////////////////////////
/// @brief 1908: ERROR_GRAPH_COULD_NOT_CHANGE_EDGE
///
/// could not change edge
///
/// Will be raised when the edge could not be changed
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_GRAPH_COULD_NOT_CHANGE_EDGE                             (1908)

////////////////////////////////////////////////////////////////////////////////
/// @brief 2000: SIMPLE_CLIENT_UNKNOWN_ERROR
///
/// unknown client error
///
/// This error should not happen.
////////////////////////////////////////////////////////////////////////////////

#define TRI_SIMPLE_CLIENT_UNKNOWN_ERROR                                   (2000)

////////////////////////////////////////////////////////////////////////////////
/// @brief 2001: SIMPLE_CLIENT_COULD_NOT_CONNECT
///
/// could not connect to server
///
/// Will be raised when the client could not connect to the server.
////////////////////////////////////////////////////////////////////////////////

#define TRI_SIMPLE_CLIENT_COULD_NOT_CONNECT                               (2001)

////////////////////////////////////////////////////////////////////////////////
/// @brief 2002: SIMPLE_CLIENT_COULD_NOT_WRITE
///
/// could not write to server
///
/// Will be raised when the client could not write data.
////////////////////////////////////////////////////////////////////////////////

#define TRI_SIMPLE_CLIENT_COULD_NOT_WRITE                                 (2002)

////////////////////////////////////////////////////////////////////////////////
/// @brief 2003: SIMPLE_CLIENT_COULD_NOT_READ
///
/// could not read from server
///
/// Will be raised when the client could not read data.
////////////////////////////////////////////////////////////////////////////////

#define TRI_SIMPLE_CLIENT_COULD_NOT_READ                                  (2003)

////////////////////////////////////////////////////////////////////////////////
/// @brief 3100: ERROR_ARANGO_INDEX_PQ_INSERT_FAILED
///
/// priority queue insert failure
///
/// Will be raised when an attempt to insert a document into a priority queue
/// index fails for some reason.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_INDEX_PQ_INSERT_FAILED                           (3100)

////////////////////////////////////////////////////////////////////////////////
/// @brief 3110: ERROR_ARANGO_INDEX_PQ_REMOVE_FAILED
///
/// priority queue remove failure
///
/// Will be raised when an attempt to remove a document from a priority queue
/// index fails for some reason.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_INDEX_PQ_REMOVE_FAILED                           (3110)

////////////////////////////////////////////////////////////////////////////////
/// @brief 3111: ERROR_ARANGO_INDEX_PQ_REMOVE_ITEM_MISSING
///
/// priority queue remove failure - item missing in index
///
/// Will be raised when an attempt to remove a document from a priority queue
/// index fails when document can not be located within the index.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_INDEX_PQ_REMOVE_ITEM_MISSING                     (3111)

////////////////////////////////////////////////////////////////////////////////
/// @brief 3312: ERROR_ARANGO_INDEX_HASH_INSERT_ITEM_DUPLICATED
///
/// (non-unique) hash index insert failure - document duplicated in index
///
/// Will be raised when an attempt to insert a document into a non-unique hash
/// index fails due to the fact that document is duplicated within that index.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_INDEX_HASH_INSERT_ITEM_DUPLICATED                (3312)

////////////////////////////////////////////////////////////////////////////////
/// @brief 3313: ERROR_ARANGO_INDEX_SKIPLIST_INSERT_ITEM_DUPLICATED
///
/// (non-unique) skiplist index insert failure - document duplicated in index
///
/// Will be raised when an attempt to insert a document into a non-unique
/// skiplist index fails due to the fact that document is duplicated within
/// that index.
////////////////////////////////////////////////////////////////////////////////

#define TRI_ERROR_ARANGO_INDEX_SKIPLIST_INSERT_ITEM_DUPLICATED            (3313)

////////////////////////////////////////////////////////////////////////////////
/// @brief 3200: WARNING_ARANGO_INDEX_HASH_DOCUMENT_ATTRIBUTE_MISSING
///
/// hash index insertion warning - attribute missing in document
///
/// Will be raised when an attempt to insert a document into a hash index is
/// caused by the document not having one or more attributes which are required
/// by the hash index.
////////////////////////////////////////////////////////////////////////////////

#define TRI_WARNING_ARANGO_INDEX_HASH_DOCUMENT_ATTRIBUTE_MISSING          (3200)

////////////////////////////////////////////////////////////////////////////////
/// @brief 3202: WARNING_ARANGO_INDEX_HASH_UPDATE_ATTRIBUTE_MISSING
///
/// hash index update warning - attribute missing in revised document
///
/// Will be raised when an attempt to update a document results in the revised
/// document not having one or more attributes which are required by the hash
/// index.
////////////////////////////////////////////////////////////////////////////////

#define TRI_WARNING_ARANGO_INDEX_HASH_UPDATE_ATTRIBUTE_MISSING            (3202)

////////////////////////////////////////////////////////////////////////////////
/// @brief 3211: WARNING_ARANGO_INDEX_HASH_REMOVE_ITEM_MISSING
///
/// hash index remove failure - item missing in index
///
/// Will be raised when an attempt to remove a document from a hash index fails
/// when document can not be located within that index.
////////////////////////////////////////////////////////////////////////////////

#define TRI_WARNING_ARANGO_INDEX_HASH_REMOVE_ITEM_MISSING                 (3211)

////////////////////////////////////////////////////////////////////////////////
/// @brief 3300: WARNING_ARANGO_INDEX_SKIPLIST_DOCUMENT_ATTRIBUTE_MISSING
///
/// skiplist index insertion warning - attribute missing in document
///
/// Will be raised when an attempt to insert a document into a skiplist index
/// is caused by in the document not having one or more attributes which are
/// required by the skiplist index.
////////////////////////////////////////////////////////////////////////////////

#define TRI_WARNING_ARANGO_INDEX_SKIPLIST_DOCUMENT_ATTRIBUTE_MISSING      (3300)

////////////////////////////////////////////////////////////////////////////////
/// @brief 3302: WARNING_ARANGO_INDEX_SKIPLIST_UPDATE_ATTRIBUTE_MISSING
///
/// skiplist index update warning - attribute missing in revised document
///
/// Will be raised when an attempt to update a document results in the revised
/// document not having one or more attributes which are required by the
/// skiplist index.
////////////////////////////////////////////////////////////////////////////////

#define TRI_WARNING_ARANGO_INDEX_SKIPLIST_UPDATE_ATTRIBUTE_MISSING        (3302)

////////////////////////////////////////////////////////////////////////////////
/// @brief 3311: WARNING_ARANGO_INDEX_SKIPLIST_REMOVE_ITEM_MISSING
///
/// skiplist index remove failure - item missing in index
///
/// Will be raised when an attempt to remove a document from a skiplist index
/// fails when document can not be located within that index.
////////////////////////////////////////////////////////////////////////////////

#define TRI_WARNING_ARANGO_INDEX_SKIPLIST_REMOVE_ITEM_MISSING             (3311)

////////////////////////////////////////////////////////////////////////////////
/// @brief 3400: WARNING_ARANGO_INDEX_BITARRAY_DOCUMENT_ATTRIBUTE_MISSING
///
/// bitarray index insertion warning - attribute missing in document
///
/// Will be raised when an attempt to insert a document into a bitarray index
/// is caused by in the document not having one or more attributes which are
/// required by the bitarray index.
////////////////////////////////////////////////////////////////////////////////

#define TRI_WARNING_ARANGO_INDEX_BITARRAY_DOCUMENT_ATTRIBUTE_MISSING      (3400)

////////////////////////////////////////////////////////////////////////////////
/// @brief 3402: WARNING_ARANGO_INDEX_BITARRAY_UPDATE_ATTRIBUTE_MISSING
///
/// bitarray index update warning - attribute missing in revised document
///
/// Will be raised when an attempt to update a document results in the revised
/// document not having one or more attributes which are required by the
/// bitarray index.
////////////////////////////////////////////////////////////////////////////////

#define TRI_WARNING_ARANGO_INDEX_BITARRAY_UPDATE_ATTRIBUTE_MISSING        (3402)

////////////////////////////////////////////////////////////////////////////////
/// @brief 3411: WARNING_ARANGO_INDEX_BITARRAY_REMOVE_ITEM_MISSING
///
/// bitarray index remove failure - item missing in index
///
/// Will be raised when an attempt to remove a document from a bitarray index
/// fails when document can not be located within that index.
////////////////////////////////////////////////////////////////////////////////

#define TRI_WARNING_ARANGO_INDEX_BITARRAY_REMOVE_ITEM_MISSING             (3411)


////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif

