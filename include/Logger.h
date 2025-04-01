#ifndef HYSPEX_LOGGER_H
#define HYSPEX_LOGGER_H
#pragma once
#include <sstream>
#include "HySpexLibraryExport.h"

#ifdef _WIN32
#define HYSPEX_CB_CALLING_CONVENTION __stdcall
#else
#define HYSPEX_CB_CALLING_CONVENTION
#endif

/*!
 * Initialize logger, will display a console window for now. More facilities will be provided in the future. NOT thread-safe, suggest calling before utilizing rest of library.
 */
#define HYSPEX_LOG_INIT() hyspex::Logger::createInstance( true )

/*!
 * Initialize logger, but only send logs to callback functions (if any)
 */
#define HYSPEX_LOG_INIT_WITHOUT_CONSOLE() hyspex::Logger::createInstance( false )

// NOLINTBEGIN
/*!
 * Log information
 */
#define HYSPEX_LOG_INFO( X )  do { if( hyspex::Logger::isCreated() ) { std::stringstream hyspex_logger_ss; hyspex_logger_ss << "INFO: "  << X; hyspex::Logger::getInstance()->addTimestampedMessage( hyspex::Logger::HYSPEX_LOGTYPE_INFO, hyspex_logger_ss.str().c_str() ); } } while( 0 )
/*!
 * Log warnings
 */
#define HYSPEX_LOG_WARN( X )  do { if( hyspex::Logger::isCreated() ) { std::stringstream hyspex_logger_ss; hyspex_logger_ss << "WARN: "  << X; hyspex::Logger::getInstance()->addTimestampedMessage( hyspex::Logger::HYSPEX_LOGTYPE_WARN, hyspex_logger_ss.str().c_str() ); } } while( 0 )
/*!
 * Log errors
 */
#define HYSPEX_LOG_ERROR( X ) do { if( hyspex::Logger::isCreated() ) { std::stringstream hyspex_logger_ss; hyspex_logger_ss << "ERROR: " << X; hyspex::Logger::getInstance()->addTimestampedMessage( hyspex::Logger::HYSPEX_LOGTYPE_ERROR, hyspex_logger_ss.str().c_str() ); } } while( 0 )
/*!
 * Log debug
 */
#define HYSPEX_LOG_DEBUG( X ) do { if( hyspex::Logger::isCreated() ) { std::stringstream hyspex_logger_ss; hyspex_logger_ss << "DEBUG: " << X; hyspex::Logger::getInstance()->addTimestampedMessage( hyspex::Logger::HYSPEX_LOGTYPE_DEBUG, hyspex_logger_ss.str().c_str() ); } } while( 0 )
/*!
 * Log performance
 */
#define HYSPEX_LOG_PERF( X ) do { if( hyspex::Logger::isCreated() ) { std::stringstream hyspex_logger_ss; hyspex_logger_ss << "PERF: " << X; hyspex::Logger::getInstance()->addTimestampedMessage( hyspex::Logger::HYSPEX_LOGTYPE_PERF, hyspex_logger_ss.str().c_str() ); } } while( 0 )

// NOLINTEND
namespace hyspex
{
    class LoggerImpl;

    /*!
    * @brief Singleton class to deal with logging of errors, information etc.
    *
    * This class uses the pImpl idiom to reduce external dependencies.
    * We also want to avoid relying on STL for maximum compatibility.
    *
    * EXAMPLE with console output:
    * @code
    * // initialize logging structure with console output
    * HYSPEX_LOG_INIT();
    * // log with timestamp:
    * int number = 5;
    * HYSPEX_LOG_INFO( "Logging test number" << number );
    * HYSPEX_LOG_WARN( "Something went wrong." );
    * HYSPEX_LOG_ERROR( "Fatal error" );
    * @endcode
    *
    * EXAMPLE with callback:
    * @code
    * // initialize logging structure without console output
    *
    * void test_callback( void* a_userData, int64_t a_secondsSinceEpoch, uint64_t a_milliseconds, hyspex::Logger::hyspex_logtype_t a_type, const char* a_message, size_t a_length )
    * {
    *    struct tm ltime;
    * #ifdef _WIN32
    *    localtime_s( &ltime, &a_secondsSinceEpoch );
    * #else
    *    localtime_r( &a_secondsSinceEpoch, &ltime );
    * #endif
    *    std::cout << std::put_time( &ltime, "%F %T" ) << "." << std::setfill( '0' ) << std::setw( 3 ) << a_milliseconds << ": " << a_message << std::endl;
    * }
    *
    * HYSPEX_LOG_INIT_WITHOUT_CONSOLE();
    * hyspex::Logger::getInstance()->registerCallback( test_callback, nullptr );
    * @endcode
    *
    * NOTE: if HYSPEX_LOG_INIT / HYSPEX_LOG_INIT_WITHOUT_CONSOLE is not called nothing will be printed.
    */
    class Logger
    {
    public:
        /*!
        * Enum for log type.
        */
        typedef enum
        {
            HYSPEX_LOGTYPE_INFO = 0x1,
            HYSPEX_LOGTYPE_DEBUG = 0x2,
            HYSPEX_LOGTYPE_ERROR = 0x4,
            HYSPEX_LOGTYPE_WARN = 0x8,
            HYSPEX_LOGTYPE_PERF = 0x10
        } hyspex_logtype_t;

        /*!
        * Callback for log messages
        */
        typedef void ( HYSPEX_CB_CALLING_CONVENTION *loggerCallBackFnType )( void* a_userData,                //!< User data (for instance a class instance)
                                                                             int64_t a_secondsSinceEpoch,     //!< Seconds since epoch, see example for conversion to local time.
                                                                             uint64_t a_milliseconds,         //!< Milliseconds since epoch, see example
                                                                             Logger::hyspex_logtype_t a_type, //!< Log type
                                                                             const char* a_message,           //!< Log message (0-terminated)
                                                                             size_t a_length );               //!< Length of log message.

        HYSPEXLIBRARY_API static void createInstance(); //!< ( NB: not recommended to use, use macro)
        HYSPEXLIBRARY_API static void createInstance( bool a_withConsole ); //!< ( NB: not recommended to use, use macro)
        HYSPEXLIBRARY_API static bool isCreated(); //!< Used to determine if we want to do anything with the log or not. ( NB: not recommended to use, use macro)
        HYSPEXLIBRARY_API static Logger* getInstance(); //!< Get instance. ( NB: not recommended to use, use macro)
        HYSPEXLIBRARY_API void addTimestampedMessage( hyspex_logtype_t a_logType, const char* a_message ); //!< Add message with current timestamp. ( NB: not recommended to use, use macro )
        HYSPEXLIBRARY_API void registerCallback( loggerCallBackFnType a_func , void* a_userData ); //!< Callback to call when logs are received.
        HYSPEXLIBRARY_API void unregisterCallback( loggerCallBackFnType a_func ); //!< Remove callback.
        HYSPEXLIBRARY_API bool openLogFile( const char* a_path = "", const char* a_fileName = "" ); //!< Open log file. If both arguments are empty, a file with HySpexLogger_<timestamp>.log will be created.
        HYSPEXLIBRARY_API void closeLogFile(); //!< Close log file (will be done automatically on destruction)

    private:
        Logger( bool a_withConsole );
        ~Logger();
        LoggerImpl* m_pImpl;
        static Logger* sm_instance;
    };


}

#endif // HYSPEX_LOGGER_H
