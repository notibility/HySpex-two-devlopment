#ifndef CAMERA_READER_THREAD_H
#define CAMERA_READER_THREAD_H
#pragma once
#include "ThreadObject.h"

namespace hyspex
{
	class Camera;
}

/*!
* @brief Example class that reads images from Camera
*
* This class grabs frames from the camera, and will print "Saturation Detected!" if the image is saturated.
*/
class CameraReaderThread : public hyspex::ThreadObject
{
public:
	CameraReaderThread( hyspex::Camera* a_camera );
	~CameraReaderThread();

protected:
	CameraReaderThread() = delete;
	void own_thread();
private:
	hyspex::Camera* m_camera;
};

#endif // CAMERA_READER_THREAD_H
