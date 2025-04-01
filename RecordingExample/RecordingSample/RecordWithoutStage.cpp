#include "stdafx.h"
#include "RecordWithoutStage.h"
#include <Logger.h>
#include <Camera.h>
#include <Stage.h>

RecordWithoutStage::RecordWithoutStage( hyspex::Camera* a_camera ) : RecordBaseThread( a_camera, nullptr )
                                                                   , m_numberOfFrames( 0 )
{

}

RecordWithoutStage::~RecordWithoutStage()
{
}

void RecordWithoutStage::setNumberOfFrames( int a_numberOfFrames )
{
    m_numberOfFrames = a_numberOfFrames;
}

void RecordWithoutStage::own_thread()
{
    bool ok = writeToFile( getFileName( "", hyspex::HYSPEX_RE ), m_numberOfFrames );

    if( !ok )
    {
        HYSPEX_LOG_ERROR( "Recording aborted." );
    }
}

double RecordWithoutStage::unitsPerPixel()
{
    return 0.0; // dummy
}