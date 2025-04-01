#ifndef HYSPEX_STAGE_H
#define HYSPEX_STAGE_H
#pragma once
#include <string>
#include "HySpexLibraryExport.h"
#include "datatypes.h"

namespace hyspex
{
    class StageImpl;

    /*!
    * @brief Common class for all stages.
    *
    * This class uses the pImpl idiom to reduce external dependencies.
    * We also want to avoid relying on STL for maximum compatibility.
    * This class is Thread Safe.
    *
    * This class is returned by StageManager's getStageByIndex( <index> ).
    *
    * EXAMPLE:
    * @code
    * hyspex::Stage* stage = manager->getStageByIndex( 0 );
    *
    * if( stage )
    * {
    *     stage->setSpeed( 5.0 );
    *     stage->moveRelative( 90.0 ); // 90 degrees.
    *     bool is_moving = true;
    *     while( is_moving )
    *     {
    *          stage->isMoving( &is_moving );
    *          using namespace std::chrono_literals;
    *          std::this_thread::sleep_for( 10ms ); // replace with Sleep( 10 ) or usleep( 10000 ) or other suitable equivalent if not using C++11.
    *     }
    * }
    * @endcode
    * This resource is owned by hyspex::StageManager and should not be deleted by the user.
    */

    class Stage
    {
    public:
        Stage( StageImpl* a_pImpl ); //!< internal constructor not exposed externally.
        HYSPEXLIBRARY_API ~Stage(); //!< destructor()

        HYSPEXLIBRARY_API ReturnCode setSpeed( double a_unitsPerSecond ); //!< get speed of stage in units/s \ref getUnits()
        HYSPEXLIBRARY_API ReturnCode getSpeed( double* a_unitsPerSecond ); //!< get speed of stage in units/s \ref getUnits()
        HYSPEXLIBRARY_API ReturnCode getAcceleration( double* a_units ); //!< get acceleration in units/s^2, provided by configuration file.
        HYSPEXLIBRARY_API ReturnCode getDeceleration( double* a_units ); //!< get deceleration in units/s^2, provided by configuration file.
        HYSPEXLIBRARY_API ReturnCode getMaxSpeed( double* a_unitsPerSecond ); //!< get max speed in units/s, provided by configuration file.
        HYSPEXLIBRARY_API ReturnCode getAbsoluteMax( double* a_units ); //!< get max position for stage in units, provided by configuration file.
        HYSPEXLIBRARY_API ReturnCode getAbsoluteMin( double* a_units ); //!< get min position for stage in units, provided by configuration file.
        HYSPEXLIBRARY_API ReturnCode getRecommendedMovementSpeed( double* a_units ); //!< get recommended movement speed of stage, provided by configuration file.
        HYSPEXLIBRARY_API ReturnCode moveRelative( double a_units ); //!< move units relative to current position ( NB: for very small movements it is recommended to use moveAbsolute() instead ) 
        HYSPEXLIBRARY_API ReturnCode moveAbsolute( double a_units ); //!< move to position in units.
        HYSPEXLIBRARY_API ReturnCode moveHome(); //!< perform homing of stage, it is recommended to call \ref setCurrentAsZero() afterwards. 
        HYSPEXLIBRARY_API ReturnCode moveLeft(); //!< move left.
        HYSPEXLIBRARY_API ReturnCode moveRight(); //!< move right.
        HYSPEXLIBRARY_API ReturnCode setCurrentAs( double a_units ); //!< set current position in units.
        HYSPEXLIBRARY_API ReturnCode setCurrentAsZero(); //!< set current position to 0.0
        HYSPEXLIBRARY_API ReturnCode getPosition( double* a_units ); //!< get current position in units.
        HYSPEXLIBRARY_API ReturnCode getEncoderPosition( double* a_units ); //!< get encoder position ( NB: might not be supported by stage )
        HYSPEXLIBRARY_API ReturnCode getRawEncoderPosition( int* a_steps ); //!< get raw encoder position ( no conversion done ) ( NB: might not be supported by stage )
        HYSPEXLIBRARY_API ReturnCode stop(); //!< stop stage. For standa stages this means doing a soft stop followed by a stop.
        HYSPEXLIBRARY_API ReturnCode isMoving( bool* a_moving ); //!< check if stage movement command is currently being executed. Use this to determine if Stage has stopped, or is about to start.
        HYSPEXLIBRARY_API ReturnCode getMoveStatus( bool* a_moving, bool* a_targetSpeedReached ); //!< check if stage is moving and if target speed is reached.
        HYSPEXLIBRARY_API ReturnCode setPeriodicMovementCheckRequired( bool a_enabled, void* a_handle = nullptr ); //!< Require periodic check for a_handle. Will only be disabled when no handles need it.
        HYSPEXLIBRARY_API bool getMoveStatusFromPeriodicChecker( bool* a_moving, bool* a_targetSpeedReached ); //!< Get last status from periodic is moving/ target speed checker. Returns valid true/false.

        HYSPEXLIBRARY_API ReturnCode isConnected( bool* a_connected ); //!< check if stage is connected to controller.

        inline ReturnCode getUnits( std::string& a_string ) const //!< get units as a string. Typical examples are "deg" or "cm". Provided by configuration file.
        {
            char destination[ 32 ];
            ReturnCode retval = getUnits( destination, 32 );
            a_string = std::string( destination );
            return retval;
        }

        inline ReturnCode getType( std::string& a_string ) const //!< get type of stage. Typical examples are "rotation", "y-rotation" or "translation". Provided by configuration file.
        {
            char destination[ 32 ];
            ReturnCode retval = getType( destination, 32 );
            a_string = std::string( destination );
            return retval;
        }

        inline std::string getId() const //!< Stage id string, designed to be human readable.
        {
            char destination[ 256 ];
            getId( destination, 256 );
            return std::string( destination );
        }

        inline ReturnCode getNegativeDirectionLabel( std::string& a_string ) const //!< get label for negative direction ( typically "Left" unless negative_direction = is specified in configuration file. )
        {
            char destination[ 32 ];
            ReturnCode retval = getNegativeDirectionLabel( destination, 32 );
            a_string = std::string( destination );
            return retval;
        }

        inline ReturnCode getPositiveDirectionLabel( std::string& a_string ) const //!< get label for positive direction ( "Right" unless positive_direction = is specified in configuration file. )
        {
            char destination[ 32 ];
            ReturnCode retval = getPositiveDirectionLabel( destination, 32 );
            a_string = std::string( destination );
            return retval;
        }

    protected:
        HYSPEXLIBRARY_API ReturnCode getUnits( char* a_destination, size_t a_maxDestinationSize ) const; //!< see public getUnits
        HYSPEXLIBRARY_API ReturnCode getType( char* a_destination, size_t a_maxDestinationSize ) const; //!< see public getType
        HYSPEXLIBRARY_API void getId( char* a_destination, size_t a_maxDestinationSize ) const; //!< see public getId
        HYSPEXLIBRARY_API ReturnCode getNegativeDirectionLabel( char* a_destination, size_t a_maxDestinationSize ) const; //!< see public getNegativeDirectionLabel
        HYSPEXLIBRARY_API ReturnCode getPositiveDirectionLabel( char* a_destination, size_t a_maxDestinationSize ) const; //!< see public getPositiveDirectionLabel

    private:
        // disallow copy-constructors
        Stage( const Stage& that );
        Stage& operator=( const Stage& that );
        StageImpl* m_pImpl; //!< Pointer to internal implementation.
    };
}

#endif // HYSPEX_STAGE_H
