#ifndef HYSPEX_FILEWRITER_H
#define HYSPEX_FILEWRITER_H
#pragma once
#include <cstddef>

#include "datatypes.h"
#include "HySpexLibraryExport.h"

namespace hyspex
{
    class FileWriterImpl;
    class Camera;
    class FileReader;

    /*!
    * @brief Class for writing .hyspex files.
    *
    * This class uses the pImpl idiom to reduce external dependencies.
    * We also want to avoid relying on STL for maximum compatibility.
    * This class is not thread-safe.
    *
    * This class is intended to be used if you need more flexibility than Recorder provides.
    * It is also used internally by Recorder.
    *
    */
    class FileWriter
    {
    public:

        HYSPEXLIBRARY_API FileWriter( bool ); //!< Deprecated: Argument has no effect.
        HYSPEXLIBRARY_API FileWriter(); //!< Constructor
        HYSPEXLIBRARY_API ~FileWriter();
        HYSPEXLIBRARY_API bool open( const char* a_fileName, bool a_includeSaturation = false ); //!< Creates and opens file for writing.
        HYSPEXLIBRARY_API bool writeHeader( hyspex::Camera* a_camera, const char* a_comment, bool a_reCorrected = false ); //!< Writes header containing all settings, and also reserves place for background matrixes etc.
        HYSPEXLIBRARY_API bool writeHeader( hyspex::FileReader* a_reader, const char* a_comment, bool a_reCorrected = false ); //!< Writes header containing all settings, and also reserves place for background matrixes etc.
        HYSPEXLIBRARY_API bool writeImage( const unsigned short* a_buffer, const size_t& a_size ); //!< Write image buffer to file.
        HYSPEXLIBRARY_API bool writeSaturated( const unsigned char* a_buffer, const size_t& a_size ); //!< Write saturated buffer to file.
        HYSPEXLIBRARY_API bool replaceMatrix( const char* a_name, const double* a_array, const size_t& a_size ); //!< Replaces matrixes written with writeArray and a_storelookupOffset == true.
        HYSPEXLIBRARY_API bool writeStartTemperature( double a_temperatureCelcius ); //!< Temperature at start of recording in celcius.
        HYSPEXLIBRARY_API bool writeEndTemperature( double a_temperatureCelcius ); //!< Temperature at end of recording in celcius.
        HYSPEXLIBRARY_API bool setAcquisitionDate( unsigned short a_year, unsigned short a_month, unsigned short a_day ); //!< Set acquisition date to write to file and ENVI header.
        HYSPEXLIBRARY_API bool setAcquisitionStartTime( unsigned short a_hours, unsigned short a_minutes, unsigned short a_seconds ); //!< Acquisition time at start of recording.
        HYSPEXLIBRARY_API bool setAcquisitionStopTime(  unsigned short a_hours, unsigned short a_minutes, unsigned short a_seconds );  //!< Acquisition time at stop of recording.
        HYSPEXLIBRARY_API bool close(); //!< write ENVI header and close file, updating all necessary data.

    private:
        // disallow copy-constructors
        HYSPEXLIBRARY_API FileWriter( const FileWriter& that );
        HYSPEXLIBRARY_API FileWriter& operator=( const FileWriter& that );
        FileWriterImpl* m_pImpl;
    };
}

#endif // HYSPEX_FILEREADER_H
