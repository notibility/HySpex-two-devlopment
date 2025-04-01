#include "stdafx.h"
#include "RecordBaseThread.h"
#include <Stage.h>
#include <Camera.h>
#include <Windows.h>
#include <Logger.h>
#include <iomanip>

RecordBaseThread::RecordBaseThread( hyspex::Camera* a_camera, hyspex::Stage* a_stage ) : m_recorder( true )
, m_camera( a_camera )
, m_stage( a_stage )
{
    m_recorder.setComment( "Example recording" );
    m_recorder.setImageOptions( hyspex::HYSPEX_RE );
    m_recorder.setWriteSaturationMatrix( true );
    m_recorder.setCamera( m_camera );
}

RecordBaseThread::~RecordBaseThread()
{
}

std::string RecordBaseThread::getFileName( const std::string& a_prefix, hyspex::ImageOptions a_imageOptions )
{
    std::stringstream ss;

    std::string recordHD;

    bool ok = m_camera->getParameter( "RecordHD", recordHD );

    if( !ok )
    {
        HYSPEX_LOG_WARN( "Unable to determine recording harddrive, defaulting to D:" );
        ss << "D";
    }
    else
    {
        ss << recordHD;
    }
    ss << "://";
    ss << a_prefix;
    ss << "_";
    ss << m_camera->getId();
    ss << "_";

    unsigned int integration_time = m_camera->getIntegrationTime();

    ss << integration_time << "us";

    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t( now );
    auto ms = std::chrono::duration_cast< std::chrono::milliseconds >( now.time_since_epoch() ) % 1000;

    struct tm ltime;
#ifdef _MSC_VER
    localtime_s( &ltime, &now_c );
#else
    localtime_r( &now_c, &ltime );
#endif
    ss << "_";
    ss << std::put_time( &ltime, "%FT%H%M%S" );

    if( a_imageOptions == hyspex::HYSPEX_RE )
    {
        ss << "_corr";
    }
    else if( a_imageOptions == hyspex::HYSPEX_RAW )
    {
        ss << "_raw";
    }
    else
    {
        ss << "_custom";
    }

    ss << ".hyspex";
    return ss.str();
}

bool RecordBaseThread::writeToFile( const std::string& a_fileName, int a_numberOfFrames )
{
    return writeToFile( a_fileName, 0.0, 0.0, 0.0, a_numberOfFrames );
}

bool RecordBaseThread::writeToFile( const std::string& a_fileName, double a_startPosition, double a_unitsToRecord, double a_unitsPerPixel )
{
    int number_of_frames = static_cast< int >( a_unitsToRecord / a_unitsPerPixel );
    return writeToFile( a_fileName, a_startPosition, a_unitsToRecord, a_unitsPerPixel, number_of_frames );
}

bool RecordBaseThread::writeToFile( const std::string& a_fileName, double a_startPosition, double a_unitsToRecord, double a_unitsPerPixel, int a_numberOfFrames )
{
    m_recorder.setDestinationPath( a_fileName.c_str() );

    if( m_stage )
    {
        double recording_speed = a_unitsPerPixel * 1000000.0 / ( m_camera->getFramePeriod() * m_camera->getAverageFrames() );

        m_stage->setSpeed( recording_speed );
    }


    m_recorder.setAutoStopAfterFrames( a_numberOfFrames );

    // NB: this order is important to avoid losing frames.
    m_recorder.start();
    m_camera->startAcquisition();

    if( m_stage )
    {
        m_stage->moveAbsolute( a_startPosition + a_unitsToRecord );
    }

    // busy loop that waits for completion.
    while( m_recorder.getStatus() != hyspex::HYSPEX_RECORDING_STOPPED && m_recorder.getStatus() >= 0 && !m_terminate )
    {
        HYSPEX_LOG_INFO( "Recorded: " << m_recorder.getFramesWritten() << " out of: " << a_numberOfFrames );
        Sleep( 500 );
    }

    if( m_terminate && m_stage )
    {
        // stopping stage immediately
        m_stage->stop();
    }

    HYSPEX_LOG_INFO( "Recorded: " << m_recorder.getFramesWritten() << " out of: " << a_numberOfFrames << " ( complete ) " );

    hyspex::RecordingStatus status = m_recorder.getStatus();

    // recording complete
    m_recorder.stop();

    // moving stage back to origin
    if( m_stage )
    {
        HYSPEX_LOG_INFO( "Moving stage back to start..." );

        double movement_speed;
        m_stage->getRecommendedMovementSpeed( &movement_speed );
        m_stage->setSpeed( movement_speed );
        m_stage->moveAbsolute( a_startPosition );

        waitForMovementComplete( m_stage );
    }

    if( status < 0 )
    {
        HYSPEX_LOG_ERROR( "Recording stopped with status: " << status );
        return false;
    }
    else
    {
        return true;
    }
}

// Wait for movement to complete, used to wait for a stage move operation to complete.
void RecordBaseThread::waitForMovementComplete( hyspex::Stage* a_stage )
{
    bool is_moving = true;

    // Waiting for movement to complete. 
    while( is_moving )
    {
        a_stage->isMoving( &is_moving );
        Sleep( 10 ); // 10 ms poll rate
    }
}
