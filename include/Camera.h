#ifndef HYSPEX_CAMERA_H
#define HYSPEX_CAMERA_H
#pragma once

#include <string>
#include <vector>
#include <utility>
#include "datatypes.h"
#include "HySpexLibraryExport.h"

#ifdef _WIN32
#define HYSPEX_CB_CALLING_CONVENTION __stdcall
#else
#define HYSPEX_CB_CALLING_CONVENTION
#endif

#ifdef SWIG
#define HYSPEX_DEPRECATED(STR)
#else
#define HYSPEX_DEPRECATED(STR) [[deprecated(#STR)]]
#endif

namespace hyspex
{
    typedef void ( HYSPEX_CB_CALLING_CONVENTION *callBackFnType)( void* p, int eventId );
    typedef void ( HYSPEX_CB_CALLING_CONVENTION *callBackFnTypePar )( void* p, int eventId, int value );
    typedef void ( HYSPEX_CB_CALLING_CONVENTION *imageCallBackFnType )( void* p, ImageOptions a_options, const ImageLine< unsigned short >& a_image );

    class CameraImpl;
    class Stage;

    /*!
    * @brief Common interface class for all Cameras
    *
    * This class uses the pImpl idiom to reduce external dependencies.
    * We also want to avoid relying on STL for maximum compatibility.
    * This class is Thread Safe.
    *
    * This class is returned by CameraManager's getCameraByIndex( <index> ).
    *
    * EXAMPLE:
    * @code
    * hyspex::Camera* camera = manager->getCameraByIndex( 0 );
    *
    * if( camera && camera->detected() )
    * {
    *     camera->init();
    *     camera->initAcquisition();
    *
    *     camera->closeShutter();
    *     camera->startAcquisition();
    *     camera->calculateBackground();
    *
    *     camera->openShutter();
    *     camera->stopAcquisition();
    *     camera->startAcquisition();
    *
    *     for( int i = 0; i < 100; i++ )
    *     {
    *        hyspex::ImageLine< unsigned short > image = camera->getNextImage();
    *
    *        // do stuff with image here.
    *        for( size_t y = 0; y < camera->getSpectralSize(); y++ )
    *        {
    *            for( size_t x = 0; x < camera->getSpatialSize(); x++ )
    *            {
    *                unsigned short value = image.buffer.data[ y * camera->getSpatialSize() + x ];
    *            }
    *        }
    *     }
    *     camera->stopAcquisition();
    * }
    * @endcode
    * This resource is owned by hyspex::CameraManager and should not be deleted by the user.
    */
    class Camera
    {
    public:
        Camera( CameraImpl* a_impl ); //!< Internal constructor not exposed externally.
        virtual ~Camera(); //!< virtual ensures proper destruction from DLL
        HYSPEXLIBRARY_API bool operator<( const Camera& a_rhs ) const; //!< Less than operator.
        HYSPEXLIBRARY_API bool operator==( const Camera& a_rhs ) const; //!< Equal operator.
        HYSPEXLIBRARY_API ReturnCode resetFramegrabber(); //!< if Camera Link based camera, reset frame grabber, otherwise returns HYSPEX_INVALID_ARGUMENTS.

        HYSPEXLIBRARY_API bool readyForInit(); //!< Check if sensor is ready for init. Only needed for Mjolnir S-620 / SWIR-640.
        HYSPEXLIBRARY_API ReturnCode init( unsigned int a_numBuffersRaw = 1024, unsigned int a_numBuffersPreProcessing = 128 ); //!< Initialize sensor and electronics. a_numBuffersRaw is the number of buffers to use for HYSPEX_RAW, while a_numBuffersPreProcessing is for all other modes.
        HYSPEXLIBRARY_API ReturnCode setFramePeriod( unsigned int a_periodUs ); //!< set current frame period in us. ( not persistent after shutdown )
        HYSPEXLIBRARY_API ReturnCode setIntegrationTime( unsigned int a_integrationTimeUs ); //!< set current integration time in us. ( not persistent after shutdown )
        HYSPEXLIBRARY_API ReturnCode setUseExternalTrigger( bool a_external ); //!< Enable this if camera is triggered externally. has no effect if Acquisition is active.
        HYSPEXLIBRARY_API bool getUseExternalTrigger() const; //!< true if camera is triggered externally
        HYSPEXLIBRARY_API ReturnCode setDisableGroundMode( bool a_disable ); //!< Set this to disable ground mode for Industrial and Airborne applications. 
        HYSPEXLIBRARY_API WavelengthRangeId getWavelengthRangeId() const; //!< The wavelength range id. Describes which part of the spectra this camera operates in (VNIR, SWIR etc).
        HYSPEXLIBRARY_API CameraModelId getModelId() const; //!< Unique ID for each camera model. Can be used for model specific behavior (if any).

        HYSPEXLIBRARY_API ReturnCode initAcquisition(); //!< Various initialization to prepare before starting Acquisition ( set Integration time of sensor, set frame period to 0 and others ) )
        HYSPEXLIBRARY_API ReturnCode preAcquisition(); //!< Call this before startAcquisition() if it is required that startAcquisition must happen ASAP.
        HYSPEXLIBRARY_API ReturnCode waitForPreAcquisitionComplete( unsigned int a_timeoutMs = 0 ); //!< Check this to ensure that startAcquisition() happens ASAP.
        HYSPEXLIBRARY_API ReturnCode startAcquisition( unsigned int a_timeoutMs = 0 ); //!< Starts trigger with frame period and starts streaming data.
        HYSPEXLIBRARY_API ReturnCode startAcquisitionNoWait(); //!< Will start acquisition, but will not verify that data is actually received. Only supported if using external trigger.
        HYSPEXLIBRARY_API ReturnCode stopAcquisition(); //!< Stops trigger and stops streaming data.

        // Moving average background calculation
        HYSPEXLIBRARY_API ReturnCode setMovingAverageBackgroundCalculationMaxBins( unsigned int a_maxNumberOfBins ); //!< Set maximum number of bins to use for moving average background calculation. Memory usage will be spatialSize * spectralSize * a_numberOfBins.
        HYSPEXLIBRARY_API unsigned int getMovingAverageBackgroundCalculationMaxBins() const; //!< Return max number of bins to use for moving average background calculation.
        HYSPEXLIBRARY_API ReturnCode startMovingAverageBackgroundCalculation(); //!< Starts moving average calculation of background with getNumberOfBackgrounds(). Background will be updated either on stop or update. NB: Does not calculate standard deviation.
        HYSPEXLIBRARY_API ReturnCode stopMovingAverageBackgroundCalculation( unsigned int a_timeoutMs = 0 ); //!< Stop moving average calculation and wait until timeout. Also restores shutter state.
        HYSPEXLIBRARY_API ReturnCode stopMovingAverageBackgroundCalculation( unsigned int a_timeoutMs, bool a_restoreShutterState ); //!< Stop moving average calculation and wait until timeout.
        HYSPEXLIBRARY_API ReturnCode updateMovingAverageBackgroundCalculation( unsigned int a_timeoutMs = 0 ); //!< Update moving average calculation and wait until timeout.
        HYSPEXLIBRARY_API ReturnCode getMovingAverageBackgroundStatistics( double* a_lastAverageDN, double* a_deltaDN ); //!< Get last average DN, and difference in last number of backgrounds.

        HYSPEXLIBRARY_API ReturnCode startCalculatingBackground(); //!< Starts calculating background forever until stopped by stopCalculatingBackground() at which time background is saved.
        HYSPEXLIBRARY_API ReturnCode startContinuousBackgroundCalculation( unsigned int a_numberOfBackgrounds = 0 ); //!< Starts continuous calculation of background which is saved every a_numberOfBackgrounds. Status reported with event HYSPEX_EVENT_BACKGROUND_STATUS_CHANGED. If a_numberOfBackgrounds = 0 it behaves as startCalculatingBackground.
        HYSPEXLIBRARY_API ReturnCode stopCalculatingBackground(); //!< Stops calculating background, and stores result if possible. Also restores shutter state.
        HYSPEXLIBRARY_API ReturnCode stopCalculatingBackground( bool a_restoreShutterState ); //!< Stops calculating background, and stores result if possible.
        HYSPEXLIBRARY_API ReturnCode calculateBackground( unsigned int a_timeoutMs = 0, unsigned int a_numberOfBackgrounds = 0 ); //!< will capture number_of_background images ( if 0 ) with the shutter closed, and then return. NB: requires started acquisition, or it will wait forever.
        HYSPEXLIBRARY_API ReturnCode calculateBackgroundAsync( unsigned int a_timeoutMs = 0, unsigned int a_numberOfBackgrounds = 0 ); //!< will capture number_of_background images ( if 0 ), but will return immediately, call waitForCalculateBackgroundAsync() afterwards for proper cleanup.
        HYSPEXLIBRARY_API ReturnCode waitForCalculateBackgroundAsync(); //!< Will wait for completion of calculateBackgroundAsync().
        HYSPEXLIBRARY_API ReturnCode openShutter(); //!< Open Shutter, will wait until operation is complete.
        HYSPEXLIBRARY_API ReturnCode closeShutter(); //!< Close Shutter, will wait until operation is complete.

        HYSPEXLIBRARY_API ReturnCode setAverageFrames( unsigned short a_averageFrames ); //!< frames to average.
        HYSPEXLIBRARY_API unsigned short getAverageFrames() const; //!< frames to average.

        HYSPEXLIBRARY_API ReturnCode setNumberOfBackgrounds( unsigned int a_numberOfBackgrounds ); //!< Set number of backgrounds to average when doing calculate background, if 0 is specified, it will be reset to default.
        HYSPEXLIBRARY_API unsigned int getNumberOfBackgrounds() const; //!< Get number of backgrounds to average, will be number_of_background from .set file for lens if not set with "setNumberOfBackgrounds"

        HYSPEXLIBRARY_API ReturnCode setBackgroundSkipFrames( unsigned int a_backgrounds ); //!< Set number of backgrounds to skip when calculating background.
        HYSPEXLIBRARY_API unsigned int getBackgroundSkipFrames() const;                     //!< Get number of backgrounds to skip when calculating background.

        HYSPEXLIBRARY_API void setIncludeStageMetadata( Stage* a_stage ); //!< disable if null. If enabled, the stage will be polled as early as possible in the image processing chain to include stage movement data.

        HYSPEXLIBRARY_API ReturnCode setEventOutputEnabled( bool a_enabled ); //!< enable/disable Event/Strobe Output if supported by camera.
        HYSPEXLIBRARY_API bool getEventOutputEnabled() const; //!< get Event/Strobe Output state
        HYSPEXLIBRARY_API ReturnCode getEventOutputDivisionFactor( unsigned int* a_divisionFactor ); //!< Get Event/Strobe Output division factor

        // ROI adjustment - spatial
        HYSPEXLIBRARY_API bool getSpatialROISupport( unsigned int* a_divisibleBy = nullptr ) const; //!< check for Spatial ROI support - returns true if supported ( and implemented )
        HYSPEXLIBRARY_API ReturnCode setSpatialROI( unsigned int a_spatialStart, unsigned int a_spatialSize ); //!< set Spatial ROI, need to call UpdateROI() to make effective
        HYSPEXLIBRARY_API ReturnCode getPendingSpatialROI( unsigned int* a_spatialStart, unsigned int* a_spatialSize ) const; //!< Spatial ROI pending update.
        HYSPEXLIBRARY_API ReturnCode getSpatialROI( unsigned int* a_spatialStart, unsigned int* a_spatialSize ) const; //!< current Spatial ROI.
        HYSPEXLIBRARY_API ReturnCode resetSpatialROI(); //!< reset pending Spatial ROI to current ( convenience function ).

        // ROI adjustment - spectral
        HYSPEXLIBRARY_API bool getSpectralROISupport( unsigned int* a_maxRegions = nullptr, unsigned int* a_divisiblyBy = nullptr ) const; //!< check for Spectral ROI support

        /*!
         * Set Spectral ROI, 0: inactive, 1: active
         */
        inline ReturnCode setSpectralROI( const std::vector< int >& a_activeBands )
        {
            // data() is guaranteed to be contiguous in the C++ standard for all types except bool.
            return setSpectralROI( reinterpret_cast< const int* >( a_activeBands.data() ), static_cast< unsigned int >( a_activeBands.size() ) );
        }

        inline std::vector< int > getPendingSpectralROI() const //!< pending Spectral ROI.
        {
            const ConstBuffer< int >& roi = getPendingSpectralROIBuffer();
            return std::vector< int >( roi.data, roi.data + roi.size );
        }

        inline std::vector< int > getSpectralROI() const //!< current Spectral ROI.
        {
            const ConstBuffer< int >& roi = getSpectralROIBuffer();
            return std::vector< int >( roi.data, roi.data + roi.size );
        }

        HYSPEXLIBRARY_API ReturnCode resetSpectralROI(); //!< reset pending Spectral ROI to current. ( convenience function ).

        // ROI adjustment - general
        HYSPEXLIBRARY_API ReturnCode updateROI(); //!< send updated ROI list to camera.
        HYSPEXLIBRARY_API bool getROIEnabled() const; //!< determine if ROI is currently enabled.

        // Binning adjustment
        HYSPEXLIBRARY_API ReturnCode setSpatialBinning( unsigned short a_spatialBinning ); //!< Set spatial binning. Spatial size has to be divisible by spatial binning.
        HYSPEXLIBRARY_API ReturnCode setSpectralBinning( unsigned short a_spectralBinning ); //!< Set spectral binning. Spectral size has to be divisible by spectral binning.
        HYSPEXLIBRARY_API unsigned short getSpatialBinning() const; //!< Get spatial binning
        HYSPEXLIBRARY_API unsigned short getSpectralBinning() const; //!< Get spectral binning
        HYSPEXLIBRARY_API bool getBinningSupported() const; //!< Returns true if binning is supported in the current mode.

        HYSPEXLIBRARY_API ReturnCode setFlippingEnabled( bool a_enabled ); //!< Flip image spatially. NB: Requires HySpex RAD v3.2 or newer for some camera models.
        HYSPEXLIBRARY_API bool getFlippingEnabled() const;                 //!< True if image is flipped spatially.

        // HSNR mode
        HYSPEXLIBRARY_API ReturnCode startHighSNRMode(); //!< Start High SNR mode (HYSPEX_HSNR_RAW)
        HYSPEXLIBRARY_API ReturnCode grabHighSNRImage( unsigned short a_averageFrames ); //!< Grab one HighSNRImage.
        HYSPEXLIBRARY_API ReturnCode waitForHighSNRImageComplete( unsigned int a_timeoutMs = 0 ); //!< wait for one highSNRImage complete.
        HYSPEXLIBRARY_API ReturnCode stopHighSNRMode(); //!< Stop High SNR mode

        HYSPEXLIBRARY_API AcquisitionStatus getAcquisitionStatus(); //!< Get acquisition status.
        HYSPEXLIBRARY_API BackgroundStatus getBackgroundStatus(); //!< Get background status.
        HYSPEXLIBRARY_API BackgroundStatus getBackgroundStatus( unsigned int* a_calculatedBackgrounds ) const; //!< Get background status and number of calculated backgrounds
        HYSPEXLIBRARY_API InitStatus getInitStatus(); //!< Get initialization status.
        HYSPEXLIBRARY_API ShutterStatus getShutterStatus(); //!< Get shutter status.
        HYSPEXLIBRARY_API CommunicationStatus getCommunicationStatus(); //!< Get communication status.
        HYSPEXLIBRARY_API CoolingStatus getCoolingStatus() const; //!< Get cooling status for sensor.
        HYSPEXLIBRARY_API ReturnCode isShutterOpen( bool* a_open ); //!< Request shutter status from electronics
        HYSPEXLIBRARY_API bool detected(); //!< Same as below, but with a_requireReadyForInit = true
        HYSPEXLIBRARY_API bool detected( bool a_requireReadyForInit ); //!< Determine if camera electronics and sensor matches settings file, will always return true if Camera was returned from CameraManager.
        HYSPEXLIBRARY_API unsigned int getFramePeriod() const; //!< get current frame period in us. ( not persistent after shutdown )
        HYSPEXLIBRARY_API unsigned int getMinimumFramePeriod() const; //!< Minimum frame period
        HYSPEXLIBRARY_API unsigned int getMaxIntegrationTime( unsigned int a_framePeriodUs ) const; //!< Max integration time given a specific frame period.
        inline std::vector< std::pair< unsigned int, unsigned int > > getValidIntegrationTimeRanges( unsigned int a_framePeriodUs ) const //!< Get valid integration time ranges given a specific frame period.
        {
            unsigned int range_buffer[ 256 ];
            size_t size = 256;
            getValidIntegrationTimeRanges( a_framePeriodUs, range_buffer, &size );

            std::vector< std::pair< unsigned int, unsigned int > > ranges;
            ranges.reserve( size / 2 );
            for( size_t i = 0; i < size / 2; i++ )
            {
                ranges.emplace_back( range_buffer[ i * 2 + 0 ], range_buffer[ i * 2 + 1 ] );
            }
            return ranges;
        }
        HYSPEXLIBRARY_API unsigned int getIntegrationTime() const; //!< get current integration time in us. ( not persistent after shutdown )
        HYSPEXLIBRARY_API unsigned int getDigitalCutOff() const; //!< Digital cut off for sensor
        HYSPEXLIBRARY_API ReturnCode getAmbientTemperature( double* a_temperature ); //!< Deprecated. Use getTemperature() with HYSPEX_TEMPERATURE_AMBIENT_1
        HYSPEXLIBRARY_API ReturnCode getSensorTemperature( double* a_temperature ); //!< Deprecated. Use getTemperature() with HYSPEX_TEMPERATURE_SENSOR
        HYSPEXLIBRARY_API ReturnCode getTemperature( TemperatureProbe a_probe, double* a_temperature ); //!< Get temperature for a_probe

        HYSPEXLIBRARY_API bool getResamplingEnabled() const; //!< Returns true if real-time resampling is enabled.

        // Mjolnir specific functions
        HYSPEXLIBRARY_API ReturnCode getBatteryVoltage( double* a_voltage ); //!< get battery voltage, only applicable for Mjolnir.
        HYSPEXLIBRARY_API ReturnCode setApxState( bool a_on ); //!< Mjolnir only: turn APX on or off.
        HYSPEXLIBRARY_API ReturnCode getApxState( bool* a_on ); //!< Mjolnir only: get APX state ( on or off)

        HYSPEXLIBRARY_API ReturnCode setStatusIndicatorLed( StatusIndicatorLed  a_ledStatus ); //!< Newer Mjolnir only: set status indicator LED state
        HYSPEXLIBRARY_API ReturnCode getStatusIndicatorLed( StatusIndicatorLed* a_ledStatus ); //!< Newer Mjolnir only: get status indicator LED state

        // Auto Acquisition functions ( not supported by all cameras )
        HYSPEXLIBRARY_API ReturnCode setAutoAcquisitionMode( AutoAcquisitionMode a_mode  ); //!< Set auto acquisition mode.
        HYSPEXLIBRARY_API ReturnCode getAutoAcquisitionMode( AutoAcquisitionMode* a_mode ); //!< Get auto acquisition mode.
        HYSPEXLIBRARY_API ReturnCode getAutoAcquisitionState( bool* a_state ); //!< Get auto acquisition state

        // Set Trigger output ( Camera division factor )
        HYSPEXLIBRARY_API ReturnCode setTriggerOutput( TriggerNumber a_trigger, CameraDivisionFactor  a_factor ); //!< Newer electronics only: set trigger output.
        HYSPEXLIBRARY_API ReturnCode getTriggerOutput( TriggerNumber a_trigger, CameraDivisionFactor* a_factor ); //!< Newer electronics only: get trigger output.

        // Mjolnir Specific power function
        HYSPEXLIBRARY_API ReturnCode shutdownComputer( ComputerCommand a_command ); //!< Shutdown/restart computer.

        inline std::string getFirmwareVersion() const //!< Get Firmware version from I/O Electronics.
        {
            char destination[ 256 ];
            getFirmwareVersion( destination, 256 );
            return std::string( destination );
        }
        HYSPEXLIBRARY_API ReturnCode getHardwareRevision( uint32_t* a_majorRevision, uint32_t* a_minorRevision ) const; //!< Newer electronics only: Get Hardware revision from I/O Electronics.

        // Lens features
        HYSPEXLIBRARY_API ReturnCode useLensId( unsigned int a_lensID ); //!< ID is 0 .. getLensCount()-1, this will determine which QE,RE, pixelsize etc to use.
        HYSPEXLIBRARY_API unsigned int getLensCount() const; //!< Lenses detected for camera
        HYSPEXLIBRARY_API unsigned int getLensId() const; //!< Current lens Id.
        HYSPEXLIBRARY_API unsigned int getLensSettingMismatchCount() const; //!< return count of mismatched/missing settings in set files (if any, should be 0).

        inline std::string getLensSettingMismatchByIndex( unsigned int a_index ) const //!< return which setting is not matched. 
        {
            char destination[ 256 ];
            getLensSettingMismatchByIndex( a_index, destination, 256 );
            return std::string( destination );
        }

        inline std::string getLensNameFromId( unsigned int a_lensId ) const //!< get lens name from id.
        {
            char destination[ 256 ];
            getLensNameFromId( a_lensId, destination, 256 );
            return std::string( destination );
        }

        inline std::string getLensName() const //!< get current lens name
        {
            return getLensNameFromId( getLensId() );
        }

        HYSPEXLIBRARY_API ReturnCode getLensWorkingDistance( double* a_workingDistanceCm ) const; //!< get working distance in cm for lens. // 0.0 = infinite.
        HYSPEXLIBRARY_API ReturnCode getLensFieldOfView( double* a_fieldOfViewRadians ) const; //!< get field of view in degrees. Will simply be pixelsize_x * spatial size converted to degrees unless field_of_view is provided in settings file.
        HYSPEXLIBRARY_API ReturnCode getLensWorkingDistance( unsigned int a_lensID, double* a_workingDistanceCm ) const; //!< get working distance in cm for lens // 0.0 = infinite.
        HYSPEXLIBRARY_API ReturnCode getLensFieldOfView( unsigned int a_lensID, double* a_fieldOfViewRadians ) const; //!< get field of view in degrees. Will simply be pixelsize_x * spatial size converted to degrees unless field_of_view is provided in settings file.

        HYSPEXLIBRARY_API bool getLensIsFieldOfViewExpander( unsigned int a_lensId ) const; //!< Determine if a_lensId is a Field Of View expander.
        HYSPEXLIBRARY_API bool getLensIsFieldOfViewExpander() const; //!< Determine if current lens is a Field of View expander.

        HYSPEXLIBRARY_API bool getLensHasEqualizationFilter( unsigned int a_lensId ) const; //!< Determine if a_lensId has Equalization (EQ) filter.
        HYSPEXLIBRARY_API bool getLensHasEqualizationFilter() const; //!< Determine if current lens has Equalization (EQ) filter.

        HYSPEXLIBRARY_API unsigned int getLensSettingErrorCount() const; //!< return human readable list of errors found during initialization.
        inline std::string getLensSettingErrorByIndex( unsigned int a_index ) const //!< return string with error message.
        {
            char destination[ 256 ];
            getLensSettingErrorByIndex( a_index, destination, 256 );
            return std::string( destination );
        }

        HYSPEXLIBRARY_API bool getLensInternalNumber( unsigned int* a_lensInternalNumber ) const; //!< Get "lens" parameter from .set file. (LEGACY)
        HYSPEXLIBRARY_API bool getLensInternalNumber( unsigned int a_lensId, unsigned int* a_lensInternalNumber ) const; //!< Get "lens" parameter from .set file. (LEGACY)

        inline std::string getId() const //!< get identication string for Camera ( model name and serial number )
        {
            char destination[ 256 ];
            getId( destination, 256 );
            return std::string( destination );
        }

        inline std::string getSettingsFilePath() const//!< get settings file used.
        {
            char destination[ 512 ];
            getSettingsFilePath( destination, 512 );
            return std::string( destination );
        }

        HYSPEXLIBRARY_API GainMode getGainMode( unsigned int a_lensId ) const; //!< Gain mode is selected by lens id, as calibration is different. Only applicable for some cameras.
        HYSPEXLIBRARY_API GainMode getGainMode() const; //!< Get current gain mode. Only applicable for some cameras.

        HYSPEXLIBRARY_API size_t getSpectralSize() const; //!< Get number of spectral bands.
        HYSPEXLIBRARY_API size_t getSpatialSize() const; //!< Get number of spatial pixels.
        HYSPEXLIBRARY_API size_t getMaxSpatialSize() const; //!< Get max number of spatial pixels. This will be identical to SpatialSize if no Spatial ROI is in effect.
        HYSPEXLIBRARY_API size_t getMaxSpectralSize() const; //!< Get max number of spectral bands. This will be identical to SpectralSize if no Spectral ROI is in effect.
        HYSPEXLIBRARY_API unsigned short getMaxPixelValue() const; //!< Max pixel value, 2^bpp-1 (bits per pixel).
        HYSPEXLIBRARY_API unsigned int getSerialNumber() const; //!< Get camera serial number
        HYSPEXLIBRARY_API bool getParameter( const char* a_key, unsigned int* a_value ) const; //!< Get value from current setting file as unsigned int.
        inline bool getParameter( const char* a_key, std::string& a_value ) const //!< Get value from current setting file as string ( no conversion ).
        {
            char destination[ 256 ];
            bool retval = getParameter( a_key, destination, 256 );
            a_value = std::string( destination );
            return retval;
        }
        HYSPEXLIBRARY_API bool getParameter( const char* a_key, bool* a_value ) const; //!< Get value from current setting file as bool.
        HYSPEXLIBRARY_API bool getParameter( const char* a_key, double* a_value ) const; //!< GEt value from current setting file as double.
        HYSPEXLIBRARY_API const ConstBuffer< int >& getBadPixels() const; //!< Get bad pixel list based on settings file.
        HYSPEXLIBRARY_API const ConstBuffer< bad_pixel_t >& getBadPixelsWithCalculatedCorrections() const; //!< Get list of bad pixels with correction coordinates.
        HYSPEXLIBRARY_API const ConstBuffer< int >& getBadPixelsMatrix() const; //!< Get Matrix that is 1 for bad pixels, 0 for good ones.
        HYSPEXLIBRARY_API const ConstBuffer< double >& getREMatrix() const; //!< Get RE matrix. Also available from getCalibrationMatrix( HYSPEX_CALIB_RE )

        HYSPEXLIBRARY_API const ConstBuffer< double >& getCalibrationMatrix( calib_matrix_e a_matrix ) const; //!< Get calibration matrix by enum.
        HYSPEXLIBRARY_API bool getCalibrationMatrixAvailable( calib_matrix_e a_matrix ) const; //!< returns true if matrix is available.

        HYSPEXLIBRARY_API calibration_parameters_t getCalibrationParameters() const; //!< Get Calibration parameters

        HYSPEXLIBRARY_API size_t getResampledSpatialSize() const; //!< Get resampled spatial size.
        HYSPEXLIBRARY_API size_t getResampledSpectralSize() const; //!< Get resampled spectral size.

        HYSPEXLIBRARY_API const ImageLine< unsigned short >& getNextImage( ImageOptions a_options = HYSPEX_RAW, uint32_t a_timeoutMs = 0 ); //!< Get next image from camera, this will keep an index for each thread that calls this function, so that several threads can read from the same camera, but at different speeds.
        HYSPEXLIBRARY_API const ImageLine< unsigned short >& getNextImageCritical( ImageOptions a_options = HYSPEX_RAW, uint32_t a_timeoutMs = 0 ); //!< Get next image from camera, this will keep an index for each thread that calls this function, so that several threads can read from the same camera, but at different speeds. Writer will stop if reading too slow.
        HYSPEXLIBRARY_API ReturnCode applyMatrixCorrections( unsigned short* a_rawInput, const int a_size, ImageOptions a_options = HYSPEX_RE, bool a_removeBadPixels = true ); //!< Apply matrix corrections on a buffer at a_rawInput with size a_size.
        HYSPEXLIBRARY_API void releaseImage(); //!< release image for current thread, should only be used before exitting thread.

        HYSPEXLIBRARY_API const ConstBuffer< double >& getBackgroundMatrix() const; //!< Get background matrix as calculated by calculateBackground(), will be all 0.0 before calling calculateBackground().
        HYSPEXLIBRARY_API ReturnCode setBackgroundMatrixAgeLimit( unsigned int a_ageLimitMs ); //!< Sets the background age limit in mSec, monitored by monitorBackgroundMatrixAge(...).
        HYSPEXLIBRARY_API unsigned int getBackgroundMatrixAgeLimit(); //!< Returns the current background age limit in mSec set by setBackgroundMatrixAgeLimit(...). Returns a default value if not set.
        HYSPEXLIBRARY_API int64_t getBackgroundMatrixAge(); //!< Returns the age of the most recent Background in mSec. Returns -1 if invalid (I.e. NOT set).
        HYSPEXLIBRARY_API void monitorBackgroundMatrixAge( bool a_enable ); //!< Monitors background age and reports status through event HYSPEX_EVENT_BACKGROUND_STATUS_CHANGED.
        HYSPEXLIBRARY_API const ConstBuffer< double >& getBackgroundStdDeviationMatrix() const; //!< Get standard deviation for background matrix as calculated by calculateBackground(), will be all 0.0 before calling calculateBackground().
        HYSPEXLIBRARY_API double getBackgroundAverageDN() const; //!< get average DN of background. will be 0.0 before calling calculateBackground().
        HYSPEXLIBRARY_API const ConstBuffer< double >& getBackgroundAverageDNSpatial() const; //!< get average DN of background for each spatial position. will be 0.0 before calling calculateBackground().
        HYSPEXLIBRARY_API ReturnCode setSaturationBands( unsigned int a_startBand, unsigned int a_endBand ); //!< Set bands to use for saturation flag ( all bands by default ).

        HYSPEXLIBRARY_API void registerNotificationCallback( callBackFnType a_func, void* a_handle ); //!< register callback that notifies when a status changes ( like Acquisition Status, Background Status, Shutter Status )
        HYSPEXLIBRARY_API void unregisterNotificationCallback( callBackFnType a_func ); //!< unregister callback registered with registerNotificationCallback
        HYSPEXLIBRARY_API void registerNotificationCallback( callBackFnTypePar a_func, void* a_handle ); //!< register callback that notifies when a status changes ( like Acquisition Status, Background Status, Shutter Status ) including value parameter
        HYSPEXLIBRARY_API void unregisterNotificationCallback( callBackFnTypePar a_func ); //!< unregister callback registered with registerNotificationCallback
        HYSPEXLIBRARY_API void registerImageCallback( imageCallBackFnType a_func, ImageOptions a_options, void* a_handle ); //!< register callback that is called every time an image is received. ( convenience function that calls getNextImage() internally ).
        HYSPEXLIBRARY_API void unregisterImageCallback( imageCallBackFnType a_func ); //!< unregister callback registered with registerImageCallback.

        HYSPEXLIBRARY_API bool getUseManualShutter(); //!< Check if we are using manual shutter, returns true if shutter = 0 or electronics = 0.
        HYSPEXLIBRARY_API ReturnCode setManualShutterResponseRequired( bool a_required ); //!< enabling this will block open/close shutter until setManualShutterStateOpen is set correctly.
        HYSPEXLIBRARY_API ReturnCode setManualShutterStateOpen( bool a_open ); //!< set current manual shutter state, will return HYSPEX_INVALID_ARGUMENTS if we do not have manual shutter.

        HYSPEXLIBRARY_API ReturnCode setUseLegacyFrequencyDivider( bool a_enable, const char* a_frameGrabber, int a_inputResourceIndex = 1, int a_outputResourceIndex = 0 ); //!< Enabling this will attempt to detect and use separate frequency divider.
        HYSPEXLIBRARY_API ReturnCode setUseLegacyFrequencyDivider( bool a_enable, const char* a_frameGrabber, int a_inputResourceIndex, int a_outputResourceIndex, bool a_forceSWIRFrequencySameAsVNIR ); //!< Enabling this will attempt to detect and use separate frequency divider.
        HYSPEXLIBRARY_API bool getUseLegacyFrequencyDivider(); //!< Will return true if legacy frequency divider is successfully enabled.
        CameraImpl* getInternal(); //!< internal use only ( Implementation internals )

        HYSPEX_DEPRECATED(use getUseExternalTrigger())
        HYSPEXLIBRARY_API bool useExternalTrigger() const; //!< Deprecated, identical to getUseExternalTrigger(), will be removed in next major release

        HYSPEX_DEPRECATED(use getCalibrationMatrix( HYSPEX_CALIB_QE ))
        HYSPEXLIBRARY_API const ConstBuffer< double >& getQEMatrix() const; //!< Deprecated, same as getCalibrationMatrix( HYSPEX_CALIB_QE )

        HYSPEX_DEPRECATED(use getCalibrationMatrix( HYSPEX_CALIB_SPECTRAL_PER_BAND ))
        HYSPEXLIBRARY_API const ConstBuffer< double >& getSpectralCalibrationMatrix() const; //!< Deprecated, same as getCalibrationMatrix( HYSPEX_CALIB_SPECTRAL_PER_BAND )

        HYSPEX_DEPRECATED(use getCalibrationMatrix( HYSPEX_CALIB_FULL_SPECTRAL_PER_BAND ))
        HYSPEXLIBRARY_API const ConstBuffer< double >& getFullSpectralCalibrationMatrix() const; //!< Deprecated, same as getCalibrationMatrix( HYSPEX_CALIB_FULL_SPECTRAL_PER_BAND )

        HYSPEX_DEPRECATED(use getCalibrationMatrix( HYSPEX_CALIB_SPECTRAL_DELTA_PER_BAND ))
        HYSPEXLIBRARY_API const ConstBuffer< double >& getSpectralDeltaMatrix() const; //!< Deprecated, same as getCalibrationMatrix( HYSPEX_CALIB_SPECTRAL_DELTA_PER_BAND )

        HYSPEX_DEPRECATED(use getCalibrationMatrix( HYSPEX_CALIB_SPECTRAL_PER_BAND ))
        HYSPEXLIBRARY_API const ConstBuffer< double >& getSpectralCalibrationPerBand() const; //!< Deprecated, same as getCalibrationMatrix( HYSPEX_CALIB_SPECTRAL_PER_BAND )

        HYSPEX_DEPRECATED(use getCalibrationMatrix( HYSPEX_CALIB_FULL_SPECTRAL_PER_BAND ))
        HYSPEXLIBRARY_API const ConstBuffer< double >& getFullSpectralCalibrationPerBand() const; //!< Deprecated, same as getCalibrationMatrix( HYSPEX_CALIB_FULL_SPECTRAL_PER_BAND )

        HYSPEX_DEPRECATED(use getCalibrationMatrix( HYSPEX_CALIB_SPECTRAL_DELTA_PER_BAND ))
        HYSPEXLIBRARY_API const ConstBuffer< double >& getSpectralDeltaPerBand() const; //!< Deprecated, same as getCalibrationMatrix( HYSPEX_CALIB_SPECTRAL_DELTA_PER_BAND )

        HYSPEX_DEPRECATED(use getCalibrationMatrix( HYSPEX_CALIB_SPECTRAL_PER_PIXEL ))
        HYSPEXLIBRARY_API const ConstBuffer< double >& getSpectralCalibrationPerPixel() const; //!< Deprecated, same as getCalibrationMatrix( HYSPEX_CALIB_SPECTRAL_PER_PIXEL )

        HYSPEX_DEPRECATED(use getCalibrationMatrix( HYSPEX_CALIB_FULL_SPECTRAL_PER_PIXEL ))
        HYSPEXLIBRARY_API const ConstBuffer< double >& getFullSpectralCalibrationPerPixel() const; //!< Deprecated, same as getCalibrationMatrix( HYSPEX_CALIB_FULL_SPECTRAL_PER_PIXEL )

        HYSPEX_DEPRECATED(use getCalibrationMatrix( HYSPEX_CALIB_SPECTRAL_DELTA_PER_PIXEL ))
        HYSPEXLIBRARY_API const ConstBuffer< double >& getSpectralDeltaPerPixel() const; //!< Deprecated, same as getCalibrationMatrix( HYSPEX_CALIB_SPECTRAL_DELTA_PER_PIXEL )

        HYSPEX_DEPRECATED(use getCalibrationMatrix( HYSPEX_CALIB_TEMPERATURE_COMPENSATION_MODEL ))
        HYSPEXLIBRARY_API const ConstBuffer< double >& getTemperatureCompensationModel() const; //!< Deprecated, same as getCalibrationMatrix( HYSPEX_CALIB_TEMPERATURE_COMPENSATION_MODEL )

    protected:
        HYSPEXLIBRARY_API ReturnCode setSpectralROI( const int* a_arrayActiveBands, unsigned int a_size ); //!< see public setSpectralROI
        HYSPEXLIBRARY_API const ConstBuffer< int >& getPendingSpectralROIBuffer() const; //!< see public getPendingSpectralROI
        HYSPEXLIBRARY_API const ConstBuffer< int >& getSpectralROIBuffer() const; //!< see public getSpectralROI
        HYSPEXLIBRARY_API void getValidIntegrationTimeRanges( unsigned int a_framePeriodUs, unsigned int* a_destination, size_t* a_size ) const; //!< see public getValidIntegrationTimeRanges

        HYSPEXLIBRARY_API void getFirmwareVersion( char* a_destination, size_t a_maxDestinationSize ) const; //!< See public getFirmvareVersion
        HYSPEXLIBRARY_API bool getParameter( const char* a_key, char* a_value, uint64_t a_maxDestinationSize ) const; //!< see public getParameter
        HYSPEXLIBRARY_API void getId( char* a_destination, size_t a_maxDestinationSize ) const; //!< Get camera ID string, requires a destination to put string and the max size of the destination
        HYSPEXLIBRARY_API void getSettingsFilePath( char* a_destination, size_t a_maxDestinationSize ) const; //!< see public getSettingsFilePath
        HYSPEXLIBRARY_API void getLensNameFromId( unsigned int a_lensId, char* a_destination, size_t a_maxDestinationSize ) const; //!< see public getLensNameFromId
        HYSPEXLIBRARY_API void getLensSettingMismatchByIndex( unsigned int a_index, char* a_destination, size_t a_maxDestinationSize ) const; //!< see public getLensSettingMismatchByIndex
        HYSPEXLIBRARY_API void getLensSettingErrorByIndex( unsigned int a_index, char* a_destination, size_t a_maxDestinationSize ) const; //!< see public getLensSettingErrorByIndex

    private:
        // disallow copy-constructors
        Camera( const Camera& that );
        Camera& operator=( const Camera& that );
        CameraImpl* m_pImpl; //!< Pointer to internal implementation.
    };
}

#endif // HYSPEX_CAMERA_H
