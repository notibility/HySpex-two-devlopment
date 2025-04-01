#include "stdafx.h"
#include "CameraReaderThread.h"
#include <Camera.h>
#include <Logger.h>
#include <deque>
#include <chrono>
#include <vector>
using namespace hyspex;

CameraReaderThread::CameraReaderThread(hyspex::Camera* a_camera) : m_camera( a_camera )
{
}

CameraReaderThread::~CameraReaderThread()
{
}

void CameraReaderThread::own_thread()
{
	if( !m_camera )
	{
		return;
	}

	std::string id = m_camera->getId();
	std::thread::id tid = std::this_thread::get_id();

	int frame_number = 0;

	auto now = std::chrono::steady_clock::now();
	auto now_ns = std::chrono::time_point_cast< std::chrono::nanoseconds >( now );
	uint64_t timestamp_host = now_ns.time_since_epoch().count();

	const size_t spatial_size = m_camera->getSpatialSize();
	const size_t spectral_size = m_camera->getSpectralSize();

	while( !m_terminate )
	{
		ImageLine< unsigned short > raw_image = m_camera->getNextImage( HYSPEX_RAW, 500 ); // RAW data, timeout 500 ms.

        if( raw_image.buffer.size == 0 )
        {
            // got timeout on wait, retry.
            continue;
        }

        bool saturated = false;
        for( int i = 0; i < raw_image.saturated.size; i++ )
        {
            if( raw_image.saturated.data[ i ] > 0 )
            {
                saturated = true;
            }
        }

        if( saturated )
        {
            HYSPEX_LOG_INFO( "Saturation detected!" );
        }
	}
	HYSPEX_LOG_DEBUG( tid << " EXITTING function...");
	m_camera->releaseImage();
}