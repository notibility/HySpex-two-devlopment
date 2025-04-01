import HySpexLibrary
import numpy

logger = HySpexLibrary.Logger.createInstance()
manager = HySpexLibrary.CameraManager()
manager.readSettingsFromPath( "." )
manager.detectCameras()

for i in range( 0, manager.getCount() ):
    camera = manager.getCameraByIndex( i )
    print( camera.getSerialNumber() )
    camera.init()
    camera.initAcquisition()
    re = camera.getREMatrix()
    print( re )
    camera.setFramePeriod( 10000 )
    camera.setIntegrationTime( 9000 )
    camera.startAcquisition()
    camera.calculateBackground()
    camera.stopAcquisition()
    spectral_size = camera.getSpectralSize()
    spatial_size = camera.getSpatialSize()

    retval, temperature = camera.getSensorTemperature()
    print( temperature )
    
    camera.startAcquisition()
    try:
        while True:
            meta = camera.getNextImage( HySpexLibrary.HYSPEX_RAW, 1000 )
            image = meta.buffer
            
            if image.size == 0:
                continue
            if meta.stat.frame_number % 100 == 0:
                print( "frame_number: " + str( meta.stat.frame_number ) + " behind: " + str( meta.stat.behind_frames ) + " lost frames: " + str( meta.stat.read_lost_frames ) + " missed triggers: " + str( meta.missed_triggers ) + " average DN: " + str( image.sum() / image.size ) )
    except KeyboardInterrupt:
        print( 'aborted by user' )
    camera.releaseImage()

del manager
