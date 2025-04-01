#ifndef RECORD_ROTATION_H
#define RECORD_ROTATION_H
#pragma once
#include "RecordBaseThread.h"

/*!
* @brief Class for recording with a rotation stage.
*
* This class is used to record data with a rotation stage, it expects that the rotation stage is at the correct position before starting.
*/
class RecordRotation : public RecordBaseThread
{
public:
    RecordRotation( hyspex::Camera* a_camera, hyspex::Stage* a_stage );
    ~RecordRotation();
    void setLength( double a_degrees ); //! Recording length in degrees.

protected:
    double unitsPerPixel();
    virtual void own_thread();
    std::atomic< double > m_length;

private:
    RecordRotation() = delete;
};

#endif // RECORD_ROTATION_H
