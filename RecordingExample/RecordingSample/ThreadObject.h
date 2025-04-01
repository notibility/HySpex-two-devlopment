#ifndef HYSPEX_THREADOBJECT_H
#define HYSPEX_THREADOBJECT_H
#pragma once
#pragma warning( push )
#pragma warning( disable : 4430 4820 4365 4571 4625 4626 4668 4774 5026 5027 4514 4710 )
#include <atomic>
#include <memory>
#include <thread>
#pragma warning( pop )

namespace hyspex
{
    /*!
    * @brief Convenience class for running processing in a separate thread.
    *
    * No publicly available constructor, so you need to subclass to use.
    * 
    * This gives all subclasses a join(), start() and stop() method,
    * and a m_terminate variable that is used to abort processing when stop() is called.
    *
    * When subclassing this, own_thread() must be implemented, as that is the function that is called when a thread is spawned with start().
    */
    class ThreadObject
    {
    public:
        void join(); //!< Join thread (if running), will block until thread terminates.
        void stop(); //!< set m_terminate to true, ensure that this stops the thread or reimplement.
        void start(); //!< spawn thread with own_thread() as entry point.

    protected:
        ThreadObject();
        ~ThreadObject();
        virtual void own_thread() = 0;

        std::atomic_bool m_terminate;
        std::unique_ptr< std::thread > m_thread;
    };
}

#endif // HYSPEX_THREADOBJECT_H
