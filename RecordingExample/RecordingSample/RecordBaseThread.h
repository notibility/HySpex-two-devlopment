#ifndef RECORD_BASE_THREAD_H
#define RECORD_BASE_THREAD_H
#pragma once
#include <string>
#include "threadobject.h"
#include <recorder.h>

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

namespace hyspex
{
    class Camera;
    class Stage;
}

/*!
* @brief Base class for all recording variants, 
*
* This class is responsible for writing the actual image to file, moving and waiting for stage.
* Need to be subclassed to use.
*/
class RecordBaseThread : public hyspex::ThreadObject
{
public:
    ~RecordBaseThread();

protected:
    RecordBaseThread( hyspex::Camera* a_camera, hyspex::Stage* a_stage ); // Need to subclass this to construct.
    void waitForMovementComplete( hyspex::Stage* a_stage ); //! Wait forever until stage has stopped.
    bool writeToFile( const std::string& a_fileName, int a_numberOfFrames ); //! recording without stage.
    bool writeToFile( const std::string& a_fileName, double a_startPosition, double a_unitsToRecord, double a_unitsPerPixel ); //! recording with number of frames calculated from camera.
    bool writeToFile( const std::string& a_fileName, double a_startPosition, double a_unitsToRecord, double a_unitsPerPixel, int a_numberOfFrames ); //! recording with number of frames specified.
    virtual std::string getFileName( const std::string& a_prefix, hyspex::ImageOptions a_imageOptions ); //! get filename of the format _<type>_<serialnumber>_<integration_time>_<date/time>_<corr/raw>.hyspex

    virtual void own_thread() = 0; //! function to call when thread is spawned with start()
    virtual double unitsPerPixel() = 0; //! units per pixel, degrees for rotation, cm for translation.

    hyspex::Camera* m_camera;
    hyspex::Stage* m_stage;

private:
    RecordBaseThread() = delete;
    hyspex::Recorder m_recorder;
};

#endif // RECORD_OPERATION_THREAD_H
