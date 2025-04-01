#ifndef HYSPEX_RECORDER_H
#define HYSPEX_RECORDER_H
#pragma once
#include "datatypes.h"
#include "HySpexLibraryExport.h"

namespace hyspex
{
    class RecorderImpl;
    class Camera;
    class Stage;

    /*!
    * @brief Class for recording data to file.
    *
    * This class uses the pImpl idiom to reduce external dependencies.
    * We also want to avoid relying on STL for maximum compatibility.
    *
    * NB: deleting StageManager or CameraManager while running recording will cause undefined behavior.
    *
    * EXAMPLE:
    * @code
    * hyspex::Recorder recorder;
    *
    * recorder.setCamera( camera ); 
    * recorder.setStopStageWhenComplete( stage ); // optional
    * recorder.setDestinationPath( "D:\\test.hyspex" );
    *
    * recorder.start();
    * Sleep( 5000 );
    * recorder.stop();
    * @endcode
    */
    class HYSPEXLIBRARY_API Recorder
    {
    public:
        Recorder( bool ); //!< Deprecated: Argument has no effect.
        Recorder(); //!< Constructor.
        ~Recorder();
        ReturnCode setCamera( Camera* a_camera ); //!< Camera to use for recording.
        ReturnCode setWaitForStageTargetSpeedReached( bool a_enabled ); //!< Wait for stage target speed. Default is disabled. Requires that Camera->setStageMetadata() has been set, or it won't have any effect.
        ReturnCode setStopStageWhenComplete( Stage* a_stage ); //!< OPTIONAL: Disabled if set to NULL. Will call a_stage->stop() when recording has been stopped. 
        ReturnCode setDestinationPath( const char* a_destination ); //!< Destination path for recording.
        ReturnCode setAutoStopAfterFrames( int a_frames ); //!< 0 = disable, this determines how many frames to record.
        ReturnCode setSkipFirstFrames( int a_frames ); //!< 0 = disable, this determines how many frames to skip (used for multiple camera recording).
        ReturnCode setTestScan( bool a_enable ); //!< Fake recording.
        ReturnCode setWriteSaturationMatrix( bool a_enable ); //!< enable writing of a saturation matrix binary file + corresponding ENVI header.
        ReturnCode setCalculateBackgroundAtEnd( bool a_enable, unsigned int a_numberOfBackgrounds = 0 ); //!< Enabled by default, calculates background at end of recording, and writes it to the recording file.
        ReturnCode setCalculateBackgroundAtEnd( bool a_enable, unsigned int a_numberOfBackgrounds, bool a_movingAverage ); //!< Enabled by default, calculates background at end of recording, and writes it to the recording file.
        ReturnCode setBlockBackgroundAtEnd( bool a_enable ); //!< Disabled by default. Use this to prevent background calculation to proceed.
        ReturnCode setImageOptions( hyspex::ImageOptions a_options ); //!< Preprocessing done on images before recording. HYSPEX_RAW highly recommended.
        ReturnCode setComment( const char* a_comment ); //!< comment put in recording file (upto 256 characters).
        ReturnCode start(); //!< Start recording.
        ReturnCode stop(); //!< Stop recording.
        RecordingStatus getStatus() const; //!< Recording status, check to determine status.
        uint64_t getFramesWritten() const; //!< Frames written so far. 
        uint64_t getCurrentTimestampHost() const; //!< Timestamp of current (last) recorded frame.
        uint64_t getFirstTimestampHost() const; //!< Timestamp of first recorded frame.

    private:
        // disallow copy-constructors
        Recorder( const Recorder& that );
        Recorder& operator=( const Recorder& that );
        RecorderImpl* m_pImpl;
    };
}

#endif // HYSPEX_RECORDER_H
