#include "stdafx.h"
#include <HySpexLibrary.h>
#include "CameraReaderThread.h"
#include "RecordXYRotation.h"
#include "RecordRotation.h"
#include "RecordTranslation.h"
#include "RecordWithoutStage.h"

int main()
{
    HYSPEX_LOG_INIT(); // enable logger (optional), will open a console window (more features will be provided in the future)

    hyspex::CameraManager* camera_manager = new hyspex::CameraManager();
    camera_manager->readSettingsFromPath( "" ); // same directory as executable
    camera_manager->detectCameras();

    if( camera_manager->getCount() == 0 )
    {
        HYSPEX_LOG_INFO( "No camera(s) detected." );
    }

    hyspex::StageManager* stage_manager = new hyspex::StageManager();
    stage_manager->readStagesConfigFile( "hyspex_auxiliary.json" );
    stage_manager->detectStages();


    hyspex::Stage* stageX = nullptr;
    hyspex::Stage* stageY = nullptr;

    bool translation = true;

    std::unique_ptr< CameraReaderThread > reader = nullptr;
    hyspex::Camera* camera = nullptr;

    if( camera_manager->getCount() > 0 )
    {
        // picking first camera
        hyspex::Camera* camera = camera_manager->getCameraByIndex( 0 );

        for( int i = 0; i < stage_manager->getStageCount(); i++ )
        {
            hyspex::Stage* stage = stage_manager->getStageByIndex( i );

            std::string type;
            stage->getType( type );

            if( type == "rotation" )
            {
                translation = false;
                stageX = stage;
            }
            else if( type == "translation" )
            {
                translation = true;
                stageX = stage;
            }
            else if( type == "y-rotation" )
            {
                stageY = stage;
            }
        }

        std::string id = camera->getId();

        HYSPEX_LOG_INFO( "Camera with ID: " << id << " detected." );

        camera->init();

        // set integration time and frame period:
        camera->setFramePeriod( 15000 );
        camera->setIntegrationTime( 14800 );

        camera->initAcquisition();

        CameraReaderThread reader( camera ); // can be used to process images in real time

        // we start by calculating background.
        camera->startAcquisition();
        camera->calculateBackground();
        camera->stopAcquisition();

        // Detecting stages

        std::unique_ptr< RecordBaseThread > record = nullptr;

        // based on detected stages, run corresponding record function
        if( stageX )
        {
            if( !translation && stageY )
            {
                // XY Rotation
                std::unique_ptr< RecordXYRotation > r = std::unique_ptr< RecordXYRotation >( new RecordXYRotation( camera, stageX, stageY ) );
                r->setLength( 20.0 );
                r->setPercentOverlapY( 10.0 );
                r->setScanlines( 10 );
                record = std::unique_ptr< RecordBaseThread >( std::move( r ) );
            }
            else if( !translation )
            {
                // Rotation
                std::unique_ptr< RecordRotation > r = std::unique_ptr< RecordRotation >( new RecordRotation( camera, stageX ) );
                r->setLength( 20.0 );
                record = std::unique_ptr< RecordBaseThread >( std::move( r ) );
            }
            else
            {
                // Translation
                std::unique_ptr< RecordTranslation > r = std::unique_ptr< RecordTranslation >( new RecordTranslation( camera, stageX ) );
                r->setLength( 40.0 );
                record = std::unique_ptr< RecordBaseThread >( std::move( r ) );
            }
        }
        else
        {
            HYSPEX_LOG_INFO( "No rotation or translation stage detected." );
            std::unique_ptr< RecordWithoutStage > r = std::unique_ptr< RecordWithoutStage >( new RecordWithoutStage( camera ) );
            r->setNumberOfFrames( 0 ); // 0 = record until stopped.
            record = std::unique_ptr< RecordBaseThread >( std::move( r ) );
        }

        if( record )
        {
            record->start();

            HYSPEX_LOG_INFO( "--- PRESS ANY KEY TO ABORT RECORDING ---" );
            getchar();
            record->stop();
            record->join();
        }

    }

    delete stage_manager;
    delete camera_manager;
    HYSPEX_LOG_INFO( "----- ALL CLEANUP COMPLETE, press any key to continue..." );
    getchar();

    return 0;
}

