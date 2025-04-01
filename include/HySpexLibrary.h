#ifndef HYSPEXLIBRARY_H
#define HYSPEXLIBRARY_H
#pragma once

// All includes needed to use library.
#include "datatypes.h"
#include "CameraManager.h"
#include "Camera.h"
#include "Logger.h"
#include "FileReader.h"
#include "StageManager.h"
#include "Stage.h"
#include "Recorder.h"

/*!
 *  \mainpage
 *  Library to interface and control HySpex cameras.
 *
 *  Currently suppported cameras are:
 *  - SWIR-384
 *  - SWIR-640
 *  - VNIR-1800
 *  - VNIR-1024
 *  - VNIR-3000N
 *  - Mjolnir S-620
 *  - Mjolnir V-1240
 *  - Baldur V-1024N
 *  - Baldur S-640i-N
 *  - Baldur S-384N
 *  - VirtualCamera (Virtual*.set) based on .hyspex recording.
 *  - SimulatorCamera ( production/testing )
 *
 *  The library will load the following libraries (if needed):
 *  - Teledyne DALSA Sapera LT 8.2 ( for Camera Link based cameras ).
 *  - Andor SDK3 ( for cameras that use sensors from Andor ).
 *  - PFRemote related dlls ( for cameras that use sensors from Photon Focus (PF SDK and PF lib) ).
 *  - Spinnaker SDK ( for cameras that use sensors from PtGrey / FLIR ).
 *  - libXIMC ( for Standa stages support ).
 *  - Performax ( for Newmark stages support ).
 *
 *  Notes:
 *  - Installing a version of Teledyne DALSA Sapera LT newer than 8.2 will break compatibility with older (pre 4.x) versions of HySpex Ground.
 *  - This version of the library is intended to be used with a computer that supports Intel(R) AVX(R) instructions. If support for older hardware is desired, please contact us.
 *
 *  (C) 2017-2024 HySpex / Norsk Elektro Optikk AS
 *
 */

#endif // HYSPEXLIBRARY_H
