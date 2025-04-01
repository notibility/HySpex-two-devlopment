#include <HySpexLibrary.h> // includes all HySpex library header files

#include <Windows.h> // for Sleep

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

    for( size_t i = 0; i < camera_manager->getCount(); i++ )
    {
        hyspex::Camera* camera = camera_manager->getCameraByIndex( i );

        std::string id = camera->getId();

        HYSPEX_LOG_INFO( "Camera with ID: " << id << " detected." );

        camera->init();

        camera->initAcquisition();

        camera->closeShutter();
        camera->startAcquisition();
        camera->calculateBackground();
        camera->openShutter();


        for( int j = 0; j < 100; j++ )
        {
            hyspex::ImageLine< unsigned short > raw_image = camera->getNextImage( hyspex::HYSPEX_RAW, 0 );
            hyspex::ImageLine< unsigned short > corrected_image = camera->getNextImage( hyspex::HYSPEX_RE, 0 );

            // determine average digital number without RE and background correction
            double accumulated = 0.0;

            for( size_t y = 0; y < camera->getSpectralSize(); y++ )
            {
                for( size_t x = 0; x < camera->getSpatialSize(); x++ )
                {
                    accumulated += raw_image.buffer.data[ y * camera->getSpatialSize() + x ];
                }
            }
            double avg = accumulated / ( camera->getSpectralSize() * camera->getSpatialSize() );

            HYSPEX_LOG_INFO( "Average DN without Background subtraction and RE matrix correction: " << avg );

            accumulated = 0.0;

            for( size_t k = 0; k < corrected_image.buffer.size; k++ )
            {
                accumulated += corrected_image.buffer.data[ k ];
            }

            avg = accumulated / ( camera->getSpectralSize() * camera->getSpatialSize() );

            HYSPEX_LOG_INFO( "Average DN with Background subtraction and RE matrix correction: " << avg );
        }
        camera->releaseImage(); // thread no longer capturing frames, releasing.
        camera->stopAcquisition();
    }
    // The Camera manager will also free the resources used by the Camera(s).
    delete camera_manager;

    hyspex::StageManager* stage_manager = new hyspex::StageManager();
    stage_manager->readStagesConfigFile( "hyspex_auxiliary.json" );
    stage_manager->detectStages();

    if( stage_manager->getStageCount() == 0 )
    {
        HYSPEX_LOG_INFO( "No stage(s) detected." );
    }

    for( size_t i = 0; i < stage_manager->getStageCount(); i++ )
    {
        hyspex::Stage* stage = stage_manager->getStageByIndex( i );
        double current_position;
        hyspex::ReturnCode rc = stage->getPosition( &current_position );

        if( rc == hyspex::HYSPEX_OK )
        {
            HYSPEX_LOG_INFO( "Stage position: " << current_position ); // cm for translation stages, degrees for rotation stages.

            /* example running motor
            stage->moveRelative( -10.0 );

            bool moving = true;
            while( moving )
            {
                rc = stage->isMoving( &moving );
                if( rc != hyspex::HYSPEX_OK )
                {
                    moving = true;
                }
                Sleep( 100 );
            }
            */
        }
        else
        {
            HYSPEX_LOG_ERROR( "Unable to read position from stage." );
        }
    }

    delete stage_manager;

    HYSPEX_LOG_INFO( "Waiting 15 seconds." );
    Sleep( 15000 );

}