#include "stdafx.h"
#include "RecordRotation.h"
#include <Logger.h>
#include <Camera.h>
#include <Stage.h>

namespace
{
    // Used for rotation stages: calculates the degrees per pixel, used to determine how fast we should rotate the stage to get the correct aspect ratio.
    double degreesPerPixel( hyspex::Camera* a_camera )
    {
        double pixelsize_x;
        bool ok = a_camera->getParameter( "pixelsize_x", &pixelsize_x );

        if( !ok )
        {
            HYSPEX_LOG_ERROR( "Unable to get pixelsize_x" );
            return 0.0;
        }

        double pixelsize_y;
        ok = a_camera->getParameter( "pixelsize_y", &pixelsize_y );

        if( !ok )
        {
            HYSPEX_LOG_ERROR( "Unable to get pixelsize_y" );
            return 0.0;
        }

        const int scale = static_cast< int >( floor( pixelsize_y / pixelsize_x + 0.5 ) );

        return ( pixelsize_y * 180.0 ) / ( M_PI * scale );
    }
}

RecordRotation::RecordRotation( hyspex::Camera* a_camera, hyspex::Stage* a_stage ) : RecordBaseThread( a_camera, a_stage )
{

}

RecordRotation::~RecordRotation()
{
}

void RecordRotation::setLength( double a_degrees )
{
    m_length = a_degrees;
}

void RecordRotation::own_thread()
{
    double current_position;
    m_stage->getPosition( &current_position );

    bool ok = writeToFile( getFileName( "", hyspex::HYSPEX_RE ), current_position, m_length, unitsPerPixel() );

    if( !ok )
    {
        HYSPEX_LOG_ERROR( "Recording aborted." );
    }
}

double RecordRotation::unitsPerPixel()
{
    return degreesPerPixel( m_camera );
}