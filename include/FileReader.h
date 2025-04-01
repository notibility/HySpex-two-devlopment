#ifndef HYSPEX_FILEREADER_H
#define HYSPEX_FILEREADER_H
#pragma once

#include <string>
#include "datatypes.h"
#include "HySpexLibraryExport.h"

namespace hyspex
{
    class FileReaderImpl;
    /*!
    * @brief Class for reading .hyspex files.
    *
    * This class uses the pImpl idiom to reduce external dependencies.
    * We also want to avoid relying on STL for maximum compatibility.
    * This class is not thread-safe.
    *
    * TODO: include helper functions to get values, matrixes etc from file without returning virtual camera.
    *
    * EXAMPLE:
    * @code
    * hyspex::FileReader reader;
    * reader.open( "test.hyspex" );
    *
    * for( size_t i = 0; i < reader.getImageCount(); i++ )
    * {
    *    ConstBuffer< unsigned int > image = reader.getImage( i );
    *    // do processing on image here.
    * }
    * @endcode
    */
    class FileReader
    {
    public:
        HYSPEXLIBRARY_API FileReader(); //!< Constructor.
        HYSPEXLIBRARY_API ~FileReader(); //!< Destructor.
        HYSPEXLIBRARY_API bool open( const char* a_fileName ); //!< open file from path.
        HYSPEXLIBRARY_API bool close(); //!< Close file.

        HYSPEXLIBRARY_API uint64_t getPropertyKeyCount() const; //!< Count of properties

        /*!
         * This will return the key name of the property at a_index.
         */
        inline std::string getPropertyKeyNameByIndex( uint64_t a_index )
        {
            char destination[ 256 ];
            getPropertyKeyNameByIndex( destination, 256, a_index );
            return std::string( destination );
        }

        inline std::string getPropertyValue( const char* a_key, bool* a_ok = NULL ) const //!< This will return a string value from setting
        {
            char destination[ 256 ];
            getPropertyValue( destination, 256, a_key, a_ok );
            return std::string( destination );
        }

        HYSPEXLIBRARY_API uint64_t getDoubleMatrixCount() const; //!< Count of matrixes of doubles, like RE,QE etc.

        inline std::string getDoubleMatrixKeyNameByIndex( uint64_t a_index ) const //!< This will return the name of the double matrix at a_index.
        {
            char destination[ 256 ];
            getDoubleMatrixKeyNameByIndex( destination, 256, a_index );
            return std::string( destination );
        }

        HYSPEXLIBRARY_API uint64_t getIntMatrixCount() const; //!< Count of matrixes of ints, like bad_pixels.

        inline std::string getIntMatrixKeyNameByIndex( uint64_t a_index ) const //!< This will return the name of the int matrix at a_index.
        {
            char destination[ 256 ];
            getIntMatrixKeyNameByIndex( destination, 256, a_index );
            return std::string( destination );
        }

        HYSPEXLIBRARY_API ConstBuffer< int > getIntMatrix( const char* a_key, bool* a_ok = NULL ) const; //!< get int matrix based on key name. 
        HYSPEXLIBRARY_API ConstBuffer< double > getDoubleMatrix( const char* a_key, bool* a_ok = NULL ) const; //!< get double matrix based on key name.

        HYSPEXLIBRARY_API size_t getImageCount(); //!< count of images.
        HYSPEXLIBRARY_API ConstBuffer< unsigned short > getImage( size_t a_index ); //!< image at a_index.
        HYSPEXLIBRARY_API ConstBuffer< float > getFloatImage( size_t a_index ); //!< image at a_index (float32 (RAD))
        FileReaderImpl* getInternal(); //!< pointer to internal implementation (not exposed externally)
        HYSPEXLIBRARY_API size_t getImageOffsetInBytes() const; //!< image offset
        HYSPEXLIBRARY_API const char* getRawBinaryHeader() const; //!< raw binary header

    protected:
        HYSPEXLIBRARY_API void getPropertyKeyNameByIndex( char* a_destination, size_t a_maxDestinationSize, uint64_t a_index ) const; //!< See public getPropertyKeyNameByIndex
        HYSPEXLIBRARY_API void getPropertyValue( char* a_destination, size_t a_maxDestinationSize, const char* a_key, bool* a_ok = NULL ) const; //!< See public getPropertyValue
        HYSPEXLIBRARY_API void getDoubleMatrixKeyNameByIndex( char* a_destination, size_t a_maxDestinationSize, uint64_t a_index ) const; //!< See public getDoubleMatrixKeyNameByIndex
        HYSPEXLIBRARY_API void getIntMatrixKeyNameByIndex( char* a_destination, size_t a_maxDestinationSize, uint64_t a_index ) const; //!< See public getIntMatrixKeyNameByIndex

    private:
        // disallow copy-constructors
        FileReader( const FileReader& that );
        FileReader& operator=( const FileReader& that );
        FileReaderImpl* m_pImpl;
    };
}

#endif // HYSPEX_FILEREADER_H
