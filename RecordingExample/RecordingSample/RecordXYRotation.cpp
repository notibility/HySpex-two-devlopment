#include "stdafx.h"
#include "RecordXYRotation.h"
#include <Stage.h>
#include <Camera.h>
#include <Logger.h>

RecordXYRotation::RecordXYRotation( hyspex::Camera* a_camera, hyspex::Stage* a_stageX, hyspex::Stage* a_stageY ) : RecordRotation( a_camera, a_stageX )
                                                                                                                 , m_stageY( a_stageY )
                                                                                                                 , m_numberOfScanlines( 1 )
                                                                                                                 , m_degreesBetweenScanlines( 0.0 )
{
}

RecordXYRotation::~RecordXYRotation()
{
}

void RecordXYRotation::setScanlines( int a_numberOfScanlines )
{
    m_numberOfScanlines = a_numberOfScanlines;
}

void RecordXYRotation::setPercentOverlapY( double a_percent )
{
    double fov_radians;

    m_camera->getLensFieldOfView( &fov_radians );

    double fov_degrees = fov_radians * 180.0 / M_PI;

    setDegreesBetweenScanlines( fov_degrees * a_percent / 100.0 );
}

void RecordXYRotation::setDegreesBetweenScanlines( double a_degrees )
{
    m_degreesBetweenScanlines = a_degrees;
}

void RecordXYRotation::own_thread()
{
    double startX;
    m_stage->getPosition( &startX );

    double startY;
    m_stageY->getPosition( &startY );

    std::string filename = getFileName( "", hyspex::HYSPEX_RE );



    for( int i = 0; i < m_numberOfScanlines && !m_terminate; i++ )
    {
        std::string prefix = std::to_string( i );
        prefix += "_of_";
        prefix += std::to_string( m_numberOfScanlines );

        prefix += filename;

        bool ok = writeToFile( prefix, startX, m_length, unitsPerPixel() );

        if( !ok )
        {
            HYSPEX_LOG_ERROR( "Recording aborted." );
        }
        double movement_speed;
        m_stageY->getRecommendedMovementSpeed( &movement_speed );
        m_stageY->setSpeed( movement_speed );
        m_stageY->moveRelative( m_degreesBetweenScanlines );
        waitForMovementComplete( m_stageY );
    }

    // Moving back to origin.
    m_stageY->moveAbsolute( startY );
    waitForMovementComplete( m_stageY );
}
