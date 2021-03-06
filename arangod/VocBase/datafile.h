////////////////////////////////////////////////////////////////////////////////
/// @brief datafiles
///
/// @file
///
/// DISCLAIMER
///
/// Copyright 2010-2011 triagens GmbH, Cologne, Germany
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
/// @author Copyright 2011, triagens GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#ifndef TRIAGENS_DURHAM_VOC_BASE_DATAFILE_H
#define TRIAGENS_DURHAM_VOC_BASE_DATAFILE_H 1

#include <BasicsC/common.h>

#include <BasicsC/locks.h>

#include <VocBase/vocbase.h>

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
/// @page DurhamDatafiles Datafiles
///
/// All data is stored in datafiles. A set of datafiles forms a collection.
/// In the following sections the internal structure of a datafile is
/// described.
///
/// A datafile itself is a collection of blobs. These blobs can be shaped
/// JSON documents or any other information. All blobs have a header field,
/// call marker followed by the data of the blob itself.
///
/// @section DatafileMarker Datafile Marker
///
/// @copydetails TRI_df_marker_t
///
/// @copydetails TRI_df_header_marker_t
///
/// @copydetails TRI_df_footer_marker_t
///
/// A datafile is therefore structured as follows:
///
/// <table border>
///   <tr>
///     <td>TRI_df_header_marker_t</td>
///     <td>header entry</td>
///   </tr>
///   <tr>
///     <td>...</td>
///     <td>data entry</td>
///   </tr>
///   <tr>
///     <td>...</td>
///     <td>data entry</td>
///   </tr>
///   <tr>
///     <td>...</td>
///     <td>data entry</td>
///   </tr>
///   <tr>
///     <td>...</td>
///     <td>data entry</td>
///   </tr>
///   <tr>
///     <td>TRI_df_footer_marker_t</td>
///     <td>footer entry</td>
///   </tr>
/// </table>
///
/// @section WorkingWithDatafile Working With Datafiles
///
/// A datafile is created using the function @ref TRI_CreateDatafile.
///
/// @copydetails TRI_CreateDatafile
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                  public constants
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup VocBase
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief datafile version
////////////////////////////////////////////////////////////////////////////////

#define TRI_DF_VERSION          (1)

////////////////////////////////////////////////////////////////////////////////
/// @brief alignment in datafile blocks
////////////////////////////////////////////////////////////////////////////////

#define TRI_DF_BLOCK_ALIGN      (8)

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                      public types
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup VocBase
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief state of the datafile
////////////////////////////////////////////////////////////////////////////////

typedef enum {
  TRI_DF_STATE_CLOSED = 1,        // datafile is closed
  TRI_DF_STATE_READ = 2,          // datafile is opened read only
  TRI_DF_STATE_WRITE = 3,         // datafile is opened read/append
  TRI_DF_STATE_OPEN_ERROR = 4,    // an error has occurred while opening
  TRI_DF_STATE_WRITE_ERROR = 5,   // an error has occurred while writing
  TRI_DF_STATE_RENAME_ERROR = 6   // an error has occurred while renaming
}
TRI_df_state_e;

////////////////////////////////////////////////////////////////////////////////
/// @brief type of the marker
////////////////////////////////////////////////////////////////////////////////

typedef enum {
  TRI_DF_MARKER_HEADER    = 1000,
  TRI_DF_MARKER_FOOTER    = 1001,
  TRI_DF_MARKER_SKIP      = 1002,
  TRI_DF_MARKER_ATTRIBUTE = 1003,
  TRI_DF_MARKER_SHAPE     = 1004,

  TRI_COL_MARKER_HEADER = 2000,

  TRI_DOC_MARKER_HEADER             = 3000,
  TRI_DOC_MARKER_DOCUMENT           = 3001,
  TRI_DOC_MARKER_DELETION           = 3002,
  TRI_DOC_MARKER_BEGIN_TRANSACTION  = 3003,
  TRI_DOC_MARKER_COMMIT_TRANSACTION = 3004,
  TRI_DOC_MARKER_ABORT_TRANSACTION  = 3005,
  TRI_DOC_MARKER_EDGE               = 3006
}
TRI_df_marker_type_e;

////////////////////////////////////////////////////////////////////////////////
/// @brief storage type of the marker
////////////////////////////////////////////////////////////////////////////////

typedef uint32_t TRI_df_marker_type_t;

////////////////////////////////////////////////////////////////////////////////
/// @brief datafile version
////////////////////////////////////////////////////////////////////////////////

typedef uint32_t TRI_df_version_t;

////////////////////////////////////////////////////////////////////////////////
/// @brief boolean flag
////////////////////////////////////////////////////////////////////////////////

typedef uint32_t TRI_df_flag_t;

////////////////////////////////////////////////////////////////////////////////
/// @brief scan result
////////////////////////////////////////////////////////////////////////////////

typedef struct TRI_df_scan_s {
  TRI_voc_size_t _currentSize;
  TRI_voc_size_t _maximalSize;
  TRI_voc_size_t _endPosition;
  TRI_voc_size_t _numberMarkers;

  TRI_vector_t _entries;

  uint32_t _status;
}
TRI_df_scan_t;

////////////////////////////////////////////////////////////////////////////////
/// @brief scan result entry
////////////////////////////////////////////////////////////////////////////////

typedef struct TRI_df_scan_entry_s {
  TRI_voc_size_t _position;
  TRI_voc_size_t _size;    
  TRI_voc_tick_t _tick;

  TRI_df_marker_type_t _type;

  uint32_t _status;
}
TRI_df_scan_entry_t;

////////////////////////////////////////////////////////////////////////////////
/// @brief datafile
////////////////////////////////////////////////////////////////////////////////

typedef struct TRI_datafile_s {
  TRI_voc_fid_t _fid;            // datafile identifier

  TRI_df_state_e _state;         // state of the datafile (READ or WRITE)

  char* _filename;               // underlying filename
  int _fd;                       // underlying file descriptor

  TRI_voc_size_t _maximalSize;   // maximale size of the datafile
  TRI_voc_size_t _currentSize;   // current size of the datafile
  TRI_voc_size_t _footerSize;    // size of the final footer

  bool _isSealed;                // true, if footer has been written
  int _lastError;                // last (cirtical) error

  bool _full;                    // at least one request was rejected because there is not enough room

  char* _data;                   // start of the data array
  char* _next;                   // end of the current data

  // .............................................................................
  // access to the following attributes must be protected by a _lock
  // .............................................................................

  char* _synced;                 // currently synced upto, not including
  TRI_voc_size_t _nSynced;       // number of synced markers
  double _lastSynced;            // timestamp of the last sync

  char* _written;                // currently written upto, not including
  TRI_voc_size_t _nWritten;      // number of markers in file
}
TRI_datafile_t;

////////////////////////////////////////////////////////////////////////////////
/// @brief datafile marker
///
/// All blobs of a datafile start with a header. The base structure for all
/// such headers is as follows:
///
/// <table border>
///   <tr>
///     <td>TRI_voc_size_t</td>
///     <td>_size</td>
///     <td>The total size of the blob. This includes the size of the the
///         marker and the data. In order to iterate through the datafile
///         you can read the TRI_voc_size_t entry _size and skip the next
///         _size - sizeof(TRI_voc_size_t) bytes.</td>
///   </tr>
///   <tr>
///     <td>TRI_voc_crc_t</td>
///     <td>_crc</td>
///     <td>A crc of the marker and the data. The zero is computed as if
///         the field _crc is equal to 0.</td>
///   </tr>
///   <tr>
///     <td>TRI_df_marker_type_t</td>
///     <td>_type</td>
///     <td>see @ref TRI_df_marker_type_t</td>
///   </tr>
///   <tr>
///     <td>TRI_voc_tick_t</td>
///     <td>_tick</td>
///     <td>A unique identifier of the current blob. The identifier is
///         unique within all datafiles of all collections. See
///         @ref TRI_voc_tick_t for details.</td>
///   </tr>
/// </table>
///
/// Note that the order is important: _size must be the first entry
/// and _crc the second.
////////////////////////////////////////////////////////////////////////////////

typedef struct TRI_df_marker_s {
  TRI_voc_size_t _size;                 // 4 bytes, must be supplied
  TRI_voc_crc_t _crc;                   // 4 bytes, will be generated

  TRI_df_marker_type_t _type;           // 4 bytes, must be supplied

  TRI_voc_tick_t _tick;                 // 8 bytes, will be generated

}
TRI_df_marker_t;

////////////////////////////////////////////////////////////////////////////////
/// @brief datafile header marker
///
/// The first blob entry in a datafile is always a TRI_df_header_marker_t.
/// The header marker contains the version number of the datafile, its
/// maximal size and the creation time. There is no data payload.
///
/// <table border>
///   <tr>
///     <td>TRI_df_version_t</td>
///     <td>_version</td>
///     <td>The version of a datafile, see @ref TRI_df_version_t.</td>
///   </tr>
///   <tr>
///     <td>TRI_voc_size_t</td>
///     <td>_maximalSize</td>
///     <td>The maximal size to which a datafile can grow. If you
///         attempt to add more datafile to a datafile, then an
///         error TRI_ERROR_ARANGO_DATAFILE_FULL is returned.</td>
///   </tr>
///   <tr>
///     <td>TRI_voc_tick_t</td>
///     <td>_fid</td>
///     <td>The creation time of the datafile. This time is different
///         from the creation time of the blob entry stored in
///         base._tick.</td>
///   </tr>
/// </table>
////////////////////////////////////////////////////////////////////////////////

typedef struct TRI_df_header_marker_s {
  TRI_df_marker_t base;

  TRI_df_version_t _version;
  TRI_voc_size_t _maximalSize;
  TRI_voc_tick_t _fid;
}
TRI_df_header_marker_t;

////////////////////////////////////////////////////////////////////////////////
/// @brief datafile footer marker
///
/// The last entry in a full datafile is always a TRI_df_footer_marker_t.
/// The footer contains the maximal size of the datafile and it total
/// size.
///
/// <table border>
///   <tr>
///     <td>TRI_voc_size_t</td>
///     <td>_maximalSize</td>
///     <td>The maximal size to which a datafile can grow. This should match
///         the maximal stored in the @ref TRI_df_header_marker_t.</td>
///   </tr>
///   <tr>
///     <td>TRI_voc_size_t</td>
///     <td>_totalSize</td>
///     <td>The real size of the datafile. Should always be less than or equal
///         to the _maximalSize.</td>
///   </tr>
/// </table>
///
/// It is not possible to append entries after a footer. A datafile which
/// contains a footer is sealed and read-only.
////////////////////////////////////////////////////////////////////////////////

typedef struct TRI_df_footer_marker_s {
  TRI_df_marker_t base;

  TRI_voc_size_t _maximalSize;
  TRI_voc_size_t _totalSize;
}
TRI_df_footer_marker_t;

////////////////////////////////////////////////////////////////////////////////
/// @brief datafile document marker
////////////////////////////////////////////////////////////////////////////////

typedef struct TRI_df_document_marker_s {
  TRI_df_marker_t base;
}
TRI_df_document_marker_t;

////////////////////////////////////////////////////////////////////////////////
/// @brief datafile skip marker
////////////////////////////////////////////////////////////////////////////////

typedef struct TRI_df_skip_marker_s {
  TRI_df_marker_t base;
}
TRI_df_skip_marker_t;

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                      constructors and destructors
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup VocBase
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief creates a new datafile
///
/// You must specify a directory. This directory must exist and must be
/// writable. You must also specify a maximal size for the datafile. The maximal
/// size must be divisible by the page size. If it is not, then the size is
/// rounded down.  The datafile is created as sparse file. So there is a chance
/// that writing to the datafile will fill up your filesystem. This file is then
/// mapped into the address of the process using mmap. The create functions
/// automatically adds a @ref TRI_df_footer_marker_t to the file.
////////////////////////////////////////////////////////////////////////////////

TRI_datafile_t* TRI_CreateDatafile (char const* directory, TRI_voc_size_t maximalSize);

////////////////////////////////////////////////////////////////////////////////
/// @brief frees the memory allocated, but does not free the pointer
////////////////////////////////////////////////////////////////////////////////

void TRI_DestroyDatafile (TRI_datafile_t* datafile);

////////////////////////////////////////////////////////////////////////////////
/// @brief frees the memory allocated and but frees the pointer
////////////////////////////////////////////////////////////////////////////////

void TRI_FreeDatafile (TRI_datafile_t* datafile);

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                  public functions
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup VocBase
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief checks a CRC of a marker
////////////////////////////////////////////////////////////////////////////////

bool TRI_CheckCrcMarkerDatafile (TRI_df_marker_t const* marker);

////////////////////////////////////////////////////////////////////////////////
/// @brief creates a CRC and writes that into the header
////////////////////////////////////////////////////////////////////////////////

void TRI_FillCrcMarkerDatafile (TRI_df_marker_t* marker,
                                TRI_voc_size_t markerSize,
                                void const* body,
                                TRI_voc_size_t bodySize);

////////////////////////////////////////////////////////////////////////////////
/// @brief reserves room for an element, advances the pointer
////////////////////////////////////////////////////////////////////////////////

int TRI_ReserveElementDatafile (TRI_datafile_t* datafile,
                                TRI_voc_size_t size,
                                TRI_df_marker_t** position);

////////////////////////////////////////////////////////////////////////////////
/// @brief writes a marker and body to the datafile
////////////////////////////////////////////////////////////////////////////////

int TRI_WriteElementDatafile (TRI_datafile_t*,
                              void* position,
                              TRI_df_marker_t const* marker,
                              TRI_voc_size_t markerSize,
                              void const* body,
                              TRI_voc_size_t bodySize,
                              bool sync);

////////////////////////////////////////////////////////////////////////////////
/// @brief iterates over a datafile
////////////////////////////////////////////////////////////////////////////////

bool TRI_IterateDatafile (TRI_datafile_t*,
                          bool (*iterator)(TRI_df_marker_t const*, void*, TRI_datafile_t*, bool),
                          void* data,
                          bool journal);

////////////////////////////////////////////////////////////////////////////////
/// @brief opens an existing datafile read-only
////////////////////////////////////////////////////////////////////////////////

TRI_datafile_t* TRI_OpenDatafile (char const* filename);

////////////////////////////////////////////////////////////////////////////////
/// @brief opens an existing, possible corrupt datafile read-write
////////////////////////////////////////////////////////////////////////////////

TRI_datafile_t* TRI_ForcedOpenDatafile (char const* filename);

////////////////////////////////////////////////////////////////////////////////
/// @brief closes a datafile and all memory regions
////////////////////////////////////////////////////////////////////////////////

bool TRI_CloseDatafile (TRI_datafile_t* datafile);

////////////////////////////////////////////////////////////////////////////////
/// @brief seals a database, writes a footer, sets it to read-only
////////////////////////////////////////////////////////////////////////////////

int TRI_SealDatafile (TRI_datafile_t* datafile);

////////////////////////////////////////////////////////////////////////////////
/// @brief renames a datafile
////////////////////////////////////////////////////////////////////////////////

bool TRI_RenameDatafile (TRI_datafile_t* datafile, char const* filename);

////////////////////////////////////////////////////////////////////////////////
/// @brief truncates a datafile and seals it
////////////////////////////////////////////////////////////////////////////////

int TRI_TruncateDatafile (char const* path, TRI_voc_size_t position);

////////////////////////////////////////////////////////////////////////////////
/// @brief returns information about the datafile
////////////////////////////////////////////////////////////////////////////////

TRI_df_scan_t TRI_ScanDatafile (char const* path);

////////////////////////////////////////////////////////////////////////////////
/// @brief destroys information about the datafile
////////////////////////////////////////////////////////////////////////////////

void TRI_DestroyDatafileScan (TRI_df_scan_t* scan);

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif

// Local Variables:
// mode: outline-minor
// outline-regexp: "^\\(/// @brief\\|/// {@inheritDoc}\\|/// @addtogroup\\|// --SECTION--\\|/// @\\}\\)"
// End:
