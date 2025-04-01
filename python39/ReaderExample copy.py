import HySpexLibrary
import numpy as np
reader = HySpexLibrary.FileReader()
reader.open( "E:\\software_devolpment\\1_Baldur_S-384N_SN12013_297us_2025-03-05T174104_raw.hyspex" )

for i in range( 0, reader.getPropertyKeyCount() ):
    print( reader.getPropertyKeyNameByIndex( i ) )
print('-------------------------------------------')
print( reader.getPropertyValue( "default_R" ) )

for i in range( 0, reader.getDoubleMatrixCount() ):
    print( reader.getDoubleMatrixKeyNameByIndex( i ) )
print('-------------------------------------------')
re = reader.getDoubleMatrix( "RE" )
print( re )
bg_start = reader.getDoubleMatrix( "background_start" )

spatial_size = int( reader.getPropertyValue( "spatial_size" ) )
spectral_size = int( reader.getPropertyValue( "spectral_size" ) )

print( "Background - Average DN: " + str( bg_start.sum() / bg_start.size ) )

image = reader.getImage( 5 ).view() # image number 6
image.shape = ( spectral_size, spatial_size )
print( str( image[ 9 ][ 511 ] ) ) # print value at spectral line 10, spatial position 512

# NB: re and bg_start will not refer to valid data once reader is deleted.
del re
del bg_start
del reader
