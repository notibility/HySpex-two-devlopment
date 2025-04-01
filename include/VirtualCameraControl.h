#ifndef HYSPEX_VIRTUAL_CAMERA_CONTROL_H
#define HYSPEX_VIRTUAL_CAMERA_CONTROL_H
#pragma once
#include "HySpexLibraryExport.h"
#include <cstdint>

#ifdef _WIN32
#define HYSPEX_CB_CALLING_CONVENTION __stdcall
#else
#define HYSPEX_CB_CALLING_CONVENTION
#endif

namespace hyspex
{
    class VirtualCameraControlImpl;
    class Camera;
    class Stage;
    /*!
    * @brief Class for controlling Virtual Camera behavior.
    *
    * The purpose of this class is to control the behavior of the Virtual Camera.
    * For instance, it might be desireable to match Stage movement to a specific image,
    * or that the camera always sends out a specific image.
    *
    * This class uses the pImpl idiom to reduce external dependencies.
    * We also want to avoid relying on STL for maximum compatibility.
    */
    class HYSPEXLIBRARY_API VirtualCameraControl
    {
        public:
            //! Virtual Camera Control call back function type.
            typedef uint64_t ( HYSPEX_CB_CALLING_CONVENTION *VirtualCameraControlCallBackFnType )( void* a_userData, //!< User data (for instance a class instance)
                                                                                                   uint64_t a_imageCounter, //!< Current image counter ( resets to 0 when acquisition is started )
                                                                                                   uint64_t a_totalImages );  //!< Total number of images ( same as getImageCount() )
            VirtualCameraControl( Camera* a_camera = nullptr ); //!< Constructor. Requires which camera to control.
            ~VirtualCameraControl();
            bool getCallbackIsSet() const; //!< Returns true if callback is set.
            bool setCallback( VirtualCameraControlCallBackFnType a_func, void* a_userData ); //!< Callback to use for determining which image to acquire.
            void clearCallback(); //!< Remove callback.
            uint64_t getImageCount() const; //!< Get total number of images

        private:
            // disallow copy-constructors
            VirtualCameraControl( const VirtualCameraControl& that );
            VirtualCameraControl& operator=( const VirtualCameraControl& that );
            VirtualCameraControlImpl* m_pImpl;
    };

    /*!
     * @brief Convenience class to handle callbacks from VirtualCameraControl.
     *
     */
    class VirtualCameraControlCallback
    {
        public:
            VirtualCameraControlCallback() : m_callback( VirtualCameraControlCallback::callback_router )
            {
            }

            //! Set camera to use for callback (NB: must be a Virtual camera)
            bool setCameraForCallback( hyspex::Camera* a_camera )
            {
                if( m_control )
                {
                    delete m_control;
                    m_control = nullptr;
                }

                if( a_camera )
                {
                    m_control = new VirtualCameraControl( a_camera );
                    return m_control->setCallback( m_callback, this );
                }
                return false;
            }

            //! Get total image count.
            uint64_t getImageCount()
            {
                if( m_control )
                {
                    return m_control->getImageCount();
                }
                return 0;
            }

            //! Get image number to output.
            virtual uint64_t getImageNumber( uint64_t /* a_imageCounter */, uint64_t /* a_totalImages */ )
            {
                // implement this so that we do not get virtual pointer nullreference error.
                return 0;
            };

            virtual ~VirtualCameraControlCallback()
            {
                if( m_control )
                {
                    delete m_control;
                    m_control = nullptr;
                }
            };

        protected:
            //! Route callback to correct instance.
            static uint64_t callback_router( void* a_handle, uint64_t a_imageCounter, uint64_t a_totalImages )
            {
                VirtualCameraControlCallback* cb = reinterpret_cast< VirtualCameraControlCallback* >( a_handle );
                return cb->getImageNumber( a_imageCounter, a_totalImages );
            }

            VirtualCameraControl::VirtualCameraControlCallBackFnType m_callback{ nullptr }; //!< Callback function
            VirtualCameraControl* m_control{ nullptr }; //!< Virtual Camera Control instance.
    };
}

#endif // HYSPEX_VIRTUAL_CAMERA_CONTROL_H
