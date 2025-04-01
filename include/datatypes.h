#ifndef HYSPEX_DATATYPES_H
#define HYSPEX_DATATYPES_H
#pragma once
#include <cstdint>

// NOLINTBEGIN
#define HYSPEX_RETURN_IF_ERROR( A, B ) do { if ( A < 1 ) { return B; } } while( 0 )
#define HYSPEX_RETURN_IF_ERROR_VAL( A ) do { hyspex::ReturnCode rc_test = A; if ( rc_test < 1 ) { return rc_test; } } while ( 0 )
// NOLINTEND

namespace hyspex
{
    /*!
     * List of calibration matrixes
     */
    typedef enum
    {
        HYSPEX_CALIB_RE = 0,                              //!< Responsivity Correction Matrix (spectral x spatial)
        HYSPEX_CALIB_QE = 1,                              //!< Quantum Efficiency Matrix (spectral)
        HYSPEX_CALIB_QE_ALL = 2,                          //!< Quantum Efficiency Matrix (spectral x spatial)
        HYSPEX_CALIB_FULL_SPECTRAL_PER_BAND = 3,          //!< Full Spectral Calibration per band (spectral)
        HYSPEX_CALIB_FULL_SPECTRAL_PER_PIXEL = 4,         //!< Full Spectral Calibration per pixel (spectral x spatial)
        HYSPEX_CALIB_SPECTRAL_PER_BAND = 5,               //!< Spectral Calibration per band (spectral)
        HYSPEX_CALIB_SPECTRAL_PER_PIXEL = 6,              //!< Spectral Calibration per pixel (spectral x spatial)
        HYSPEX_CALIB_SPECTRAL_DELTA_PER_BAND = 7,         //!< Spectral Delta per band (spectral)
        HYSPEX_CALIB_SPECTRAL_DELTA_PER_PIXEL = 8,        //!< Spectral Delta per pixel (spectral x spatial)
        HYSPEX_CALIB_SPECTRAL_FWHM = 9,                   //!< Spectral FWHM (per band) (spectral)
        HYSPEX_CALIB_SENSOR_MODEL = 10,                   //!< Sensor Model (spatial)
        HYSPEX_CALIB_SPATIAL = 11,                        //!< Spatial Calibration Matrix (spectral x spatial)
        HYSPEX_CALIB_SPATIAL_DELTA = 12,                  //!< Spatial Calibration Delta Matrix (spectral x spatial)
        HYSPEX_CALIB_TEMPERATURE_COMPENSATION_MODEL = 13, //!< Temperature Compensation model (Linear) ( 2 x spectral ( slope and intercept ) )
        HYSPEX_CALIB_RESAMPLED_CENTRAL_PIXELS = 14,       //!< Resampled Central Pixels (resampled spatial)
        HYSPEX_CALIB_RESAMPLED_CENTRAL_WAVELENGTHS = 15,  //!< Resampled Central Wavelengths (resampled spectral)
        HYSPEX_CALIB_RESAMPLED_CENTRAL_WAVELENGTHS_DELTA = 16 //!< Resampled Central Wavelengths delta (resampled spectral)
    } calib_matrix_e;

    /*!
    * List of possible return codes
    */
    typedef enum
    {
        HYSPEX_OK = 1000,                                    //!< Operation OK.
        HYSPEX_TIMEOUT_REACHED = -1,                         //!< Operation timed out.
        HYSPEX_INVALID_HANDLE = -2,                          //!< Operation not possible because of invalid handle.
        HYSPEX_INVALID_ARGUMENTS = -3,                       //!< Invalid arguments to operation.
        HYSPEX_ALREADY_ACTIVE = -4,                          //!< Operation already active (like starting background or recording while already active).
        HYSPEX_NOT_ACTIVE = -5,                              //!< Operation not active (like stopping background or recording when not active).
        HYSPEX_INIT_ELECTRONICS_FAILED = -1000,              //!< I/O Electronics initialization failed.
        HYSPEX_INIT_SENSOR_FAILED = -1001,                   //!< Sensor initialization failed.
        HYSPEX_INIT_CAMERA_LINK_FAILED = -1002,              //!< Camera Link initialization failed.
        HYSPEX_INIT_SENSOR_NOT_READY = -1003,                //!< Sensor not ready, like sensor not cooled down sufficiently yet.
        HYSPEX_CAMERA_NOT_DETECTED = -2000,                  //!< Camera not detected.
        HYSPEX_CAMERA_NOT_INITIALIZED = -2001,               //!< Camera not initialized, call Camera::init().
        HYSPEX_ELECTRONICS_NOT_INITIALIZED = -2002,          //!< I/O Electronics not initialized.
        HYSPEX_ELECTRONICS_ACK_NOT_RECEIVED = -2003,         //!< I/O Electronics command did not return ACK.
        HYSPEX_ELECTRONICS_SEND_COMMAND_FAILED = -2004,      //!< Sending command to I/O electronics failed.
        HYSPEX_ELECTRONICS_RESPONSE_INVALID = -2005,         //!< Invalid response from I/O Electronics.
        HYSPEX_ELECTRONICS_FEATURE_NOT_SUPPORTED = -2006,    //!< Feature not supported by this I/O Electronics. Might be because it is an older revision, or different type.
        HYSPEX_ELECTRONICS_FDIV_NOT_FOUND = -2007,           //!< Legacy FDIV only: Frequency Divider not found.
        HYSPEX_SENSOR_COMMAND_FAILED = -3000,                //!< Sensor command failed.
        HYSPEX_SETTING_NOT_FOUND = -4000,                    //!< Setting not found.
        HYSPEX_FAILED_TO_SET_VALUE = -4001,                  //!< Failed to set value.
        HYSPEX_IGNORED_TRIGGER_IS_ACTIVE = -4002,            //!< Command ignored because trigger is active.
        HYSPEX_STATE_ACQUISITION_ALREADY_ACTIVE = -5000,     //!< Acquisition already active.
        HYSPEX_STATE_ACQUISITION_WAS_NOT_ACTIVE = -5001,     //!< Acquisition already stopped.
        HYSPEX_STATE_SHUTTER_NOT_SPECIFIED = -5200,          //!< Internal only: shutter did not have expected state.
        HYSPEX_CAMERALINK_SERVER_NOT_FOUND = -6000,          //!< Camera Link - server not found (Camera Link port not found). Like specifying serverindex = 1 when only 1 port is available ( serverindex = 0 ).
        HYSPEX_CAMERALINK_CCF_FILE_NOT_FOUND = -6005,        //!< Camera Link - unable to find .ccf file specified by configfile= option in .set file.
        HYSPEX_CAMERALINK_CREATE_ACQUISITION_FAILED = -6010, //!< Camera Link - unable to create acquisition.
        HYSPEX_CAMERALINK_CREATE_BUFFER_FAILED = -6020,      //!< Camera Link - unable to create buffer.
        HYSPEX_CAMERALINK_CREATE_PROCESSING_FAILED = -6030,  //!< Camera Link - unable to create processing.
        HYSPEX_CAMERALINK_CREATE_TRANSFER_FAILED = -6040,    //!< Camera Link - unable to create transfer.
        HYSPEX_CAMERALINK_GIO_NOT_FOUND          = -6050,    //!< Camera Link - unable to create GIO.
        HYSPEX_CAMERALINK_GIO_INIT_FAILED        = -6051,    //!< Camera Link - GIO initialization failed.
        HYSPEX_ROI_NOT_SUPPORTED = -7000,                        //!< Region of interest (ROI) not supported.
        HYSPEX_ROI_SPATIAL_NOT_SUPPORTED = -7010,                //!< Spatial Region of interest (ROI) not supported.
        HYSPEX_ROI_SPATIAL_EXCEEDED_IMAGE_SIZE = -7011,          //!< Spatial Region of interest (ROI) exceeds spatial size ( image size ).
        HYSPEX_ROI_SPATIAL_NOT_DIVISIBLE_BY_RESTRICTION = -7012, //!< Spatial Region of interest (ROI) must be divisible by a certain factor.
        HYSPEX_ROI_SPECTRAL_NOT_SUPPORTED = -7020,               //!< Spectral Region of interest (ROI) not supported.
        HYSPEX_ROI_SPECTRAL_EXCEEDED_IMAGE_SIZE = -7021,         //!< Spectral Region of interest (ROI) exceeds spectral size (image size ).
        HYSPEX_ROI_SPECTRAL_EXCEEDED_MAX_COUNT = -7022,          //!< number of spectral Region of interests (ROI) exceeded.
        HYSPEX_BINNING_NOT_SUPPORTED = -8000,                    //!< Binning not supported.
        HYSPEX_STAGE_SET_CURRENT_POSITION_FAILED = -10000,       //!< Set current position of stage failed.
        HYSPEX_STAGE_INIT_FAILED = -10001,                       //!< Stage initialization failed.
        HYSPEX_STAGE_GET_DEVICES_FAILED = -10002,                //!< Failed to get stages.
        HYSPEX_STAGE_GET_MOVE_SETTINGS_FAILED = -10100,          //!< Failed to get stage move settings.
        HYSPEX_STAGE_SET_MOVE_SETTINGS_FAILED = -10101,          //!< Failed to set stage move settings.
        HYSPEX_STAGE_GET_ENGINE_SETTINGS_FAILED = -10102,        //!< Failed to get stage engine settings.
        HYSPEX_STAGE_SET_ENGINE_SETTINGS_FAILED = -10103,        //!< Failed to set stage engine settings.
        HYSPEX_STAGE_START_FAILED = -10201,                      //!< Failed to start stage.
        HYSPEX_STAGE_STOP_FAILED = -10202,                       //!< Failed to stop stage.
        HYSPEX_STAGE_OUT_OF_RANGE = -10300,                      //!< Stage parameter out of range, like attempting to set speed beyond max speed.
        HYSPEX_STAGE_GET_ENCODER_VALUE_FAILED = -10400,          //!< Failed to get stage encoder value.
        HYSPEX_STAGE_GET_PRODUCT_INFO_FAILED = -10500,           //!< Failed to get stage information.
        HYSPEX_STAGE_OPEN_FAILED = -10600,                       //!< Unable to open stage.
        HYSPEX_STAGE_FLUSH_FAILED = -10601,                      //!< Unable to flush stage.
        HYSPEX_STAGE_CLOSE_FAILED = -10602,                      //!< Unable to close stage.
        HYSPEX_STAGE_SEND_FAILED = -10603,                       //!< Unable to send command to stage.
        HYSPEX_NOT_IMPLEMENTED = -99999                          //!< Feature not implemented for this particular Camera/Stage.
    } ReturnCode;

    /*!
    * Acquisition Status codes.
    */
    typedef enum
    {
        HYSPEX_ACQ_STOPPED = 1,                                    //!< Acquisition stopped.
        HYSPEX_ACQ_PENDING = 2,                                    //!< Acquisition pending.
        HYSPEX_ACQ_RUNNING = 3,                                    //!< Acquisition running.
        HYSPEX_ACQ_TERMINATED_DUE_TO_OVERFLOW = -1,                //!< Acquisition terminated due to overflow. If Camera Link check cabling and max frame rate.
        HYSPEX_ACQ_TERMINATED_DUE_TO_TRASH = -2,                   //!< Acquisition terminated due to lost frames. Check CPU load, and if the sensor is using USB 3.0 check cabling.
        HYSPEX_ACQ_TERMINATED_DUE_TO_ERROR = -3,                   //!< Acquisition terminated due to error, cause unknown.
        HYSPEX_ACQ_TERMINATED_DUE_TO_FRAME_PERIOD_MISMATCH = -4,   //!< Acquisition terminated because frame period did not match expected, so a trigger was most likely lost. Check frame period vs. max frame rate.
        HYSPEX_ACQ_TERMINATED_DUE_TO_TIMEOUT = -5,                 //!< Acquisition start timed out. Check cabling (If Camera Link, frames might be expected on a different port). If using external trigger, check trigger.
        HYSPEX_ACQ_TERMINATED_DUE_TO_CRITICAL_READER_TOO_SLOW = -6 //!< A thread using Camera::getNextImageCritical() is not reading fast enough, so acquisition was stopped to prevent losing frames.
    } AcquisitionStatus;

    /*!
    * Background Status codes.
    */
    typedef enum
    {
        HYSPEX_BG_INVALID = 1,       //!< Background is invalid, usually because Camera settings changed (integration time, frame period ROI).
        HYSPEX_BG_PENDING = 2,       //!< Pending background calculation, insufficient frames gathered to calculate background.
        HYSPEX_BG_VALID = 3,         //!< Background valid.
        HYSPEX_BG_PENDING_READY = 4, //!< Continuous or Moving Average background calculation
        HYSPEX_BG_EXPIRED = 5,       //!< Background expired. Only possible if Background matrix age monitoring is enabled.
        HYSPEX_BG_ABORTED = -1       //!< Background calculation aborted. This is either because of opening shutter while doing background calculation, or acquisition was stopped.
    } BackgroundStatus;

    /*!
    * Init Status codes.
    */
    typedef enum
    {
        HYSPEX_INIT_PENDING_DETECTION = 1,   //!< Camera detection pending.
        HYSPEX_INIT_PENDING_ELECTRONICS = 2, //!< Pending initialization of I/O electronics.
        HYSPEX_INIT_PENDING_SENSOR = 3,      //!< Pending initialization of Sensor.
        HYSPEX_INIT_PENDING_TRANSPORT = 4,   //!< Pending initialization of frame grabber (if applicable).
        HYSPEX_INIT_OK = 5,                  //!< Camera initialization OK.
        HYSPEX_INIT_FAILED_DETECTION = -1,   //!< Camera detection failed. Internal use only, as the camera will not be returned by CameraManager in this state.
        HYSPEX_INIT_FAILED_ELECTRONICS = -2, //!< Failed to initialize I/O Electronics.
        HYSPEX_INIT_FAILED_SENSOR = -3,      //!< Failed to initialize Sensor. Ensure settings are valid.
        HYSPEX_INIT_FAILED_TRANSPORT = -4,   //!< Failed to initialize frame grabber (if applicable). Check frame grabber configuration (name, 3-tap vs. 10-tap etc).
        HYSPEX_INIT_NOT_STARTED = -999       //!< Canmera::init() has not been called yet. Call Camera::init()
    } InitStatus;

    /*!
    * Shutter Status codes
    */
    typedef enum
    {
        HYSPEX_SHUTTER_OPEN = 1,           //!< Shutter open.
        HYSPEX_SHUTTER_CLOSED = 2,         //!< Shutter closed.
        HYSPEX_SHUTTER_PENDING_OPEN = 11,  //!< Shutter opening.
        HYSPEX_SHUTTER_PENDING_CLOSE = 12, //!< Shutter closing.
        HYSPEX_SHUTTER_FAIL_OPEN = -1,     //!< Shutter open command failed. Shutter state is unknown.
        HYSPEX_SHUTTER_FAIL_CLOSE = -2,    //!< Shutter close command failed. Shutter state is unknown.
        HYSPEX_SHUTTER_UNKNOWN = -999,     //!< Shutter has not been opened or closed yet, so state is unknown. Shutter is opened as part of Camera::init().
    } ShutterStatus;

    /*!
    * Communication Status for Sensor and Electronics
    */
    typedef enum
    {
        HYSPEX_COMM_INIT = 1,                            //!< Camera has not been initialized yet, or is initializing.
        HYSPEX_COMM_OK = 2,                              //!< Communication with sensor and electronics is OK.
        HYSPEX_COMM_FAILED_ELECTRONICS = -1,             //!< I/O Electronics communication failed. Check cabling.
        HYSPEX_COMM_FAILED_SENSOR = -2,                  //!< Sensor communication failed. Check cabling.
        HYSPEX_COMM_FAILED_ELECTRONICS_AND_SENSOR = -3   //!< I/O Electronics and Sensor communication failed. Check cabling.
    } CommunicationStatus;

    /*!
    * Cooling status for Sensor
    */
    typedef enum
    {
        HYSPEX_COOLING_STABLE_OK = 1,             //!< Sensor temperature stable and within accepted range.
        HYSPEX_COOLING_NOT_STABILIZED = 2,        //!< Sensor temperature not stable (yet)
        HYSPEX_COOLING_STABLE_DEGRADED = 3,       //!< Sensor temperature stable, but slightly out of range.
        HYSPEX_COOLING_OUT_OF_RANGE = -1,         //!< Sensor temperature out of range.
        HYSPEX_COOLING_FAILED_COMMUNICATION = -2, //!< Sensor communication failed.
        HYSPEX_COOLING_UNKNOWN = -999             //!< Sensor not initialized yet.
    } CoolingStatus;

    /*!
    * Event types, used by registerNotificationCallback
    */
    typedef enum
    {
        HYSPEX_EVENT_ACQUISITION_STATUS_CHANGED = 1,    //!< Acquisition status changed, check Camera::getAcquisitionStatus().
        HYSPEX_EVENT_COOLING_STATUS_CHANGED = 2,        //!< Cooling status changed, check Camera::getCoolingStatus().
        HYSPEX_EVENT_BACKGROUND_STATUS_CHANGED = 3,     //!< Background status changed, check Camera::getBackgroundStatus().
        HYSPEX_EVENT_INIT_STATUS_CHANGED = 4,           //!< Init status changed, check Camera::getInitStatus().
        HYSPEX_EVENT_SHUTTER_STATUS_CHANGED = 5,        //!< Shutter status changed, check Camera::getShutterStatus();
        HYSPEX_EVENT_COMMUNICATION_STATUS_CHANGED = 6,  //!< Communication status changed. check Camera::getCommunicationStatus().
        HYSPEX_EVENT_MANUAL_SHUTTER_OPEN_REQUEST = 11,  //!< Manual Shutter: open requested.
        HYSPEX_EVENT_MANUAL_SHUTTER_CLOSE_REQUEST = 12, //!< Manual Shutter: close requested.
        HYSPEX_EVENT_ERROR = 100,  //!< Future: unused.
        HYSPEX_EVENT_WARNING = 101 //!< Future: unused.
    } EventType;

    /*!
    * Image options, best performance is provided by HYSPEX_RAW.
    */
    typedef enum
    {
        HYSPEX_RAW    = 0, //!< RAW sensor data (same as HYSPEX_RAW_BP if resampled)
        HYSPEX_BGSUB  = 1, //!< RAW + bad pixel correction + background subtraction
        HYSPEX_RE     = 2, //!< RAW + bad pixel correction + background subtraction + correction for responsivity of sensor ( RE matrix in settings file )
        HYSPEX_RAW_BP = 3, //!< RAW + bad pixel correction
        HYSPEX_HSNR_RAW    = 4, //!< Same as HYSPEX_RAW but for use with grabHighSNRImage()
        HYSPEX_HSNR_RE     = 5, //!< Same as HYSPEX_RE  but for use with grabHighSNRImage()
        HYSPEX_HSNR_RAW_BP = 6, //!< Same as HYSPEX_RAW_BP but for use with grabHighSNRImage()
    } ImageOptions;

    /*!
    * Recording status codes
    */
    typedef enum
    {
        HYSPEX_RECORDING_PENDING_START = 0, //!< Pending start of recording
        HYSPEX_RECORDING_STARTED = 1,       //!< Recording started
        HYSPEX_RECORDING_PENDING_STOP = 2,  //!< Recording is pending stop
        HYSPEX_RECORDING_STOPPED = 3,       //!< Recording stopped
        HYSPEX_RECORDING_BACKGROUND_AT_END_BLOCKED = 4,  //!< Background at end is blocked, setBlockBackgroundAtEnd( false ) to continue.
        HYSPEX_RECORDING_STOPPED_BY_LOST_FRAMES = -4,    //!< Recording stopped because we have lost frames.
        HYSPEX_RECORDING_STOPPED_DUE_TO_WRITE_FAIL = -5, //!< Recording stopped because writing to disk failed.
        HYSPEX_RECORDING_STOPPED_DUE_TO_OVERFLOW = -6    //!< Recording stopped because we are unable to write fast enough.
    } RecordingStatus;


    /*!
    * Gain Modes, depends on sensor.
    */
    typedef enum
    {
        HYSPEX_GAINMODE_LOW = 0, //!< Low gain mode
        HYSPEX_GAINMODE_MEDIUM = 1, //!< Medium gain mode (only S-640i)
        HYSPEX_GAINMODE_HIGH = 2, //!< High gain mode
        HYSPEX_GAINMODE_NONE = -1, //!< Camera does not have any configurable gain mode.
        HYSPEX_GAINMODE_INVALID = -2, //!< The selected gain mode is invalid.
    } GainMode;

    /*!
    * Wavelength Range Id. E.g. VNIR, SWIR or SWIRi.
    */
    typedef enum
    {
        HYSPEX_WRID_VNIR = 0, //!< VNIR typically  400 - 1000 nm
        HYSPEX_WRID_SWIR = 1, //!< SWIR typically 1000 - 2500 nm
        HYSPEX_WRID_SWIRi = 2, //!< SWIR typically 1000 - 1700 nm
        HYSPEX_WRID_UNDEFINED = -1  //!< Not Applicable, Not defined.
    } WavelengthRangeId;

    /*!
     * Camera Model ID
     */
    typedef enum
    {
        HYSPEX_CAMERA_MODEL_IACOBUS   = 0,       //!< IACOBUS camera.
        HYSPEX_CAMERA_MODEL_SIMULATOR = 1,       //!< Simulator camera.
        HYSPEX_CAMERA_MODEL_VNIR1024  = 100,     //!< VNIR-1024
        HYSPEX_CAMERA_MODEL_VNIR1600  = 101,     //!< VNIR-1600 (not supported.)
        HYSPEX_CAMERA_MODEL_VNIR1800  = 102,     //!< VNIR-1800
        HYSPEX_CAMERA_MODEL_VNIR3000N = 103,     //!< VNIR-3000N
        HYSPEX_CAMERA_MODEL_SWIR384   = 104,     //!< SWIR-384
        HYSPEX_CAMERA_MODEL_SWIR640   = 105,     //!< SWIR-640
        HYSPEX_CAMERA_MODEL_MJOLNIR_V1240 = 200, //!< Mjolnir V-1240
        HYSPEX_CAMERA_MODEL_MJOLNIR_S620  = 201, //!< Mjolnir S-620
        HYSPEX_CAMERA_MODEL_BALDUR_V1024N = 300, //!< Baldur V-1024N
        HYSPEX_CAMERA_MODEL_BALDUR_S384N  = 301, //!< Baldur S-384N
        HYSPEX_CAMERA_MODEL_BALDUR_S640IN = 302, //!< Baldur S-640iN
        HYSPEX_CAMERA_MODEL_UNKNOWN = -1         //!< Unknown/Other/Prototype
    } CameraModelId;

    /*!
    * Temperature Probe
    */
    typedef enum
    {
        HYSPEX_TEMPERATURE_SENSOR = 0,    //!< Temperature for Camera sensor
        HYSPEX_TEMPERATURE_AMBIENT_1 = 1, //!< Ambient temperature sensor 1
        HYSPEX_TEMPERATURE_AMBIENT_2 = 2  //!< Ambient temperature sensor 2 (NB: Mjolnir only, might not be connected.)
    } TemperatureProbe;

    /*! Auto Acquisition Mode (not supported by all cameras)
     *
     * Follow mode: Recording will be started on a rising flank and stopped on a falling flank
     * Toggle mode: Recording will be started on a rising flank and stopped on the next rising flank.
     *
     */
    typedef enum
    {
        HYSPEX_AUTOACQUISITION_FOLLOW_DISABLED = 0, //!< Follow mode, Disabled (default)
        HYSPEX_AUTOACQUISITION_FOLLOW_ENABLED  = 1, //!< Follow mode, Enabled
        HYSPEX_AUTOACQUISITION_TOGGLE_DISABLED = 2, //!< Toggle mode, Disabled
        HYSPEX_AUTOACQUISITION_TOGGLE_ENABLED  = 3  //!< Toggle mode, Enabled
    } AutoAcquisitionMode;

    /*! Computer power commands (Mjolnir Only)
     *
     */
    typedef enum
    {
        HYSPEX_COMPUTER_POWER_OFF = 0,       //!< Shut down computer gracefully.
        HYSPEX_COMPUTER_FORCE_POWER_OFF = 1, //!< Force computer off.
        HYSPEX_COMPUTER_RESTART = 2,         //!< Restart computer gracefully.
        HYSPEX_COMPUTER_FORCE_RESTART = 3    //!< Force computer off, then restart.
    } ComputerCommand;

    /*!
     * Trigger number (newer electronics board only)
     */
    typedef enum
    {
        HYSPEX_TRIGGER_VNIR  = 0, //!< Trigger for VNIR
        HYSPEX_TRIGGER_SWIR  = 1, //!< Trigger for SWIR
        HYSPEX_TRIGGER_CAM3  = 2, //!< Trigger for SWIR2/CAM3
        HYSPEX_TRIGGER_EVENT = 3, //!< Trigger for Event
    } TriggerNumber;

    /*!
     * Camera Divider division factor (newer electronics board only)
     */
    typedef enum
    {
        HYSPEX_DIVISION_OFF  = 0, //!< Camera division factor - disable output
        HYSPEX_DIVISION_F1   = 1, //!< Camera division factor (F/1)
        HYSPEX_DIVISION_F2   = 2, //!< Camera division factor (F/2)
        HYSPEX_DIVISION_F4   = 3, //!< Camera division factor (F/4)
        HYSPEX_DIVISION_F8   = 4, //!< Camera division factor (F/8)
        HYSPEX_DIVISION_F16  = 5, //!< Camera division factor (F/16)
        HYSPEX_DIVISION_F32  = 6, //!< Camera division factor (F/32)
        HYSPEX_DIVISION_F64  = 7, //!< Camera division factor (F/64)
        HYSPEX_DIVISION_F128 = 8, //!< Camera division factor (F/128)
        HYSPEX_DIVISION_F256 = 9, //!< Camera division factor (F/256)
    } CameraDivisionFactor;

    /*!
    * Status Indicator LED ( on chassis, newer Mjolnir only )
    */
    typedef enum
    {
        HYSPEX_LED_OFF     = 0, //!< LED off
        HYSPEX_LED_BLUE    = 1, //!< LED Blue    (B)
        HYSPEX_LED_GREEN   = 2, //!< LED Green   (G)
        HYSPEX_LED_CYAN    = 3, //!< LED Cyan    (B+G)
        HYSPEX_LED_RED     = 4, //!< LED Red     (R)
        HYSPEX_LED_MAGENTA = 5, //!< LED Magenta (R+B)
        HYSPEX_LED_YELLOW  = 6, //!< LED Yellow  (R+G)
        HYSPEX_LED_WHITE   = 7, //!< LED White   (R+G+B)
    } StatusIndicatorLed;

    /*!
    * Convenience struct for returning read-only data.
    */
    template< typename T >
    struct ConstBuffer
    {
        uint64_t size; //!< Number of elements in data.
        const T* data; //!< Pointer to read-only data from camera.
    };

    /*!
    * Structure used for images from camera, includes some statistics, which parameters were used and timestamp.
    */
    template< typename T >
    struct ImageLine
    {
        ConstBuffer< T > buffer; //!< Image data, this data is read-only and should be copied if modifications are needed. The data is valid until getNextImage() is called from the same thread.
        ConstBuffer< T > saturated; //!< Vector with saturation status for each spatial pixel. The value is number of saturated pixels in each spectral line. The size is equal to spatial_size.
        ConstBuffer< T > max_saturation; //!< Vector with max saturation value for each spatial pixel. The size is equal to spatial_size.

        /*!
        *  Image statistics, can be used to detect lost frames.
        */


        typedef struct
        {
            uint64_t frame_number; //!< Frame number. just an incrementing counter. total trigger pulses is frame_number + missed_triggers when using external trigger.
            uint64_t read_lost_frames; //!< Frames lost while reading. If reading is critical, always check for 0.
            uint32_t behind_frames; //!< Frames behind the last incoming frame. If this exceeds buffer size, we will overflow.
            uint32_t processing_time_spent_us; //!< Total processing time since receiving data. Will give a good indication on when to expect reading to start losing frames. ( wait time not included ).
        } stat_t;

        /*!
        *  Stage metadata, will be enabled if Camera::setIncludeStageMetaData is not null.
        *  This can be used to improve synchronization between Stage and Camera.
        */
        typedef struct
        {
            bool valid; //!< Valid if setIncludeStageMetadata was set.
            bool moving; //!< Stage->getMoveStatus() value of a_isMoving 
            bool target_speed_reached; //!< Stage->getMoveStatus() value of a_targetSpeedReached 
        } stage_t;

        stat_t stat; //!< Statistics
        stage_t stage; //!< Stage metadata at time of capture
        uint64_t timestamp_ns; //!< Timestamp in nanoseconds, only usable for relative time measurements.
        uint64_t timestamp_host_ns; //!< Timestamp in nanoseconds at capture time. usable for absolute time measurements.
        uint32_t spectral_size; //!< Spectral size ( number of spectral bands )
        uint32_t spatial_size; //!< Spatial size ( number of spatial pixels per band )
        uint64_t missed_triggers; //!< Missed triggers ( only when using external trigger )
        bool aborted_write; //!< If writer is blocked due to critical reader, the last written frame will set this to true.
    };

    /*!
    * struct containing bad pixel x,y offset and where to find pixel for correction purposes.
    */
    typedef struct
    {
        short x; //!< Bad pixel spatial position
        short y; //!< Bad pixel spectral position
        short x_left; //!< Pixel to use for correction spatially ( left )
        short x_right; //!< Pixel to use for correction spatially ( right )
        short y_up; //!< Pixel to use for correction spectrally ( up )
        short y_down; //!< Pixel to use for correction spectrally ( down )
    } bad_pixel_t;


    /*!
     * struct containing important calibration properties.
     */
    typedef struct
    {
        double pixelsize_x;       //!< Spatial pixel size in radians.
        double pixelsize_y;       //!< Slit size in radians.
        double aperture_size;     //!< Aperture size in radians (meters).
        double SF;                //!< Scaling factor (Max Pixel Value / Full Well Capacity)
        uint32_t dw;              //!< Scaling factor used for responsivity correction.
        uint16_t max_pixel_value; //!< Max pixel value ( 2^(16-bitshift)-1 )
        uint16_t bitshift;        //!< Bit shift.
    } calibration_parameters_t;
}
#endif // HYSPEX_DATATYPES_H
