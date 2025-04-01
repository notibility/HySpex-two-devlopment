#include "stdafx.h"
#include "RecordTranslation.h"
#include <Camera.h>
#include <Stage.h>
#include <Logger.h>

namespace
{
    // cm per pixel, used to determine how fast the translation stage should move.
    double cmPerPixel( hyspex::Camera* a_camera )
    {
        double fov_radians;

        hyspex::ReturnCode rc = a_camera->getLensFieldOfView( &fov_radians );

        if( rc < 0 )
        {
            return 0.0;
        }

        double working_distance;

        rc = a_camera->getLensWorkingDistance( &working_distance );

        if( rc < 0 )
        {
            return 0.0;
        }

        double fov_cm = 2.0 * working_distance * tan( fov_radians / 2.0 );

        return fov_cm / a_camera->getSpatialSize();
    }
}

RecordTranslation::RecordTranslation( hyspex::Camera* a_camera, hyspex::Stage* a_stage ) : RecordBaseThread( a_camera, a_stage )
{
}

RecordTranslation::~RecordTranslation()
{
}

void RecordTranslation::setStartPosition( double a_cm )
{
    m_startPosition = a_cm;
}

void RecordTranslation::setLength( double a_cm )
{
    m_length = a_cm;
}

void RecordTranslation::own_thread()
{
    m_stage->moveAbsolute( m_startPosition );
    waitForMovementComplete( m_stage );

    bool ok = writeToFile( getFileName( "", hyspex::HYSPEX_RE ), m_startPosition, m_length, unitsPerPixel() );

    if( !ok )
    {
        HYSPEX_LOG_ERROR( "Recording aborted." );
    }
}

double RecordTranslation::unitsPerPixel()
{
    return cmPerPixel( m_camera );
}