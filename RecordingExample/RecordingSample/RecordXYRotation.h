#ifndef RECORD_XYROTATION_H
#define RECORD_XYROTATION_H
#pragma once
#include "RecordRotation.h"

/*!
* @brief Class for recording with a XY-rotation stage.
*
* This essentially just does a sequence of recordings with a rotation stage, and moves the Y-rotation stage between each recording.
*/
class RecordXYRotation : public RecordRotation
{
public:
    RecordXYRotation( hyspex::Camera* a_camera, hyspex::Stage* a_stageX, hyspex::Stage* a_stageY );
    ~RecordXYRotation();
    void setScanlines( int a_lines ); //! Number of scanlines to scan
    void setPercentOverlapY( double a_percent ); //! overlap between each scanline in percent ( will be converted to degrees between scanlines )
    void setDegreesBetweenScanlines( double a_degrees ); //! degrees between each scanline

protected:
    void own_thread();

private:
    RecordXYRotation() = delete;
    hyspex::Stage* m_stageY;
    int m_numberOfScanlines;
    double m_degreesBetweenScanlines;
};

#endif // RECORD_XYROTATION_H
