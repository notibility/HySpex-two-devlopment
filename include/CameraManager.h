#ifndef HYSPEX_CAMERAMANAGER_H
#define HYSPEX_CAMERAMANAGER_H
#pragma once
#include <string>
#include "HySpexLibraryExport.h"

namespace hyspex
{
    class Camera;
    class CameraManagerImpl;

    /*!
    * @brief Class for detecting and enumerating cameras based on .set files
    *
    * This class uses the pImpl idiom to reduce external dependencies.
    * We also want to avoid relying on STL for maximum compatibility.
    *
    * EXAMPLE:
    * @code
    * std::string path = "C://settings//";
    * 
    * hyspex::CameraManager* manager = new hyspex::CameraManager();
    *
    * // Read settings from path
    * manager->readSettingsFromPath(path.c_str());
    *
    * // Detect cameras based on setting-files.
    * manager->detectCameras();
    *
    * // Iterate over available cameras (if any)
    * for (int i = 0; i < manager->getCount(); i++)
    * {
    *    hyspex::Camera* camera = manager->getCameraByIndex(i);
    *    // do operations on camera here.
    * }
    *
    * // destroy CameraManager after using cameras.
    * delete manager; // will destroy cameras as well.
    * @endcode
    */
    class CameraManager
    {
    public:
        HYSPEXLIBRARY_API CameraManager( const char* a_path ); //!< Constructor, requires path to settings file, calls readSettingsFromPath().
        HYSPEXLIBRARY_API CameraManager(); //!< Default constructor.
        HYSPEXLIBRARY_API ~CameraManager(); //!< Destructor

        HYSPEXLIBRARY_API void setRecommendedProcessPriorities( bool a_setHighPriorityClass = true, bool a_disablePowerThrottling = true ); //!< This is a convenience function to change process priority class and power throttling to recommended values.

        HYSPEXLIBRARY_API void readSettingsFromPath( const char* a_path ); //!< Scan path for setting files.
        HYSPEXLIBRARY_API void detectCameras(); //!< Same as below, but a_requireReadyForInit = true.
        HYSPEXLIBRARY_API void detectCameras( bool a_requireReadyForInit ); //!< Detect cameras based on all found settings files. If a_requireReadyForInit == false, Camera::readyForInit() must be checked prior to Camera::init().
        HYSPEXLIBRARY_API size_t getExpectedCount(); //!< number of expected cameras.

        HYSPEXLIBRARY_API size_t getCount(); //!< number of detected cameras.
        HYSPEXLIBRARY_API Camera* getCameraByIndex( size_t a_index ); //!<  Get camera instance by index, NB: this is owned by CameraManager and should not be deleted by the user. will return NULL if index is out of range.

        /*!
         * return library version string.
         */
        static inline std::string getLibraryVersion()
        {
            char destination[ 256 ];
            getLibraryVersion( destination, sizeof( destination ) );
            return std::string( destination );
        }
    protected:
        static void HYSPEXLIBRARY_API getLibraryVersion( char* a_destination, size_t a_maxDestSize ); //!< see public getLibraryVersion

    private:

        // disallow copy-constructors
        CameraManager( const CameraManager& that );
        CameraManager& operator=( const CameraManager& that );
        CameraManagerImpl* m_pImpl; //!< Pointer to internal implementation.
    };
}

#endif // HYSPEX_CAMERAMANAGER_H
