#include "stdafx.h"
#include <functional>
#include "ThreadObject.h"

namespace hyspex
{
    ThreadObject::ThreadObject() : m_terminate( true )
                                 , m_thread( nullptr )
    {
    }

    ThreadObject::~ThreadObject()
    {
        stop();
        join();
    }

    void ThreadObject::join()
    {
        if( m_thread && m_thread->joinable() )
        {
            m_thread->join();
            m_thread.reset( nullptr );
            m_terminate = true;
        }
    }

    void ThreadObject::stop()
    {
        m_terminate = true;
    }

    void ThreadObject::start()
    {
        if( !m_thread && m_terminate )
        {
            m_terminate = false;
            m_thread = std::unique_ptr< std::thread >( new std::thread( std::bind( &ThreadObject::own_thread, this ) ) );
        }
    }
}