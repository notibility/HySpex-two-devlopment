#ifndef RECORD_WITHOUT_STAGE_H
#define RECORD_WITHOUT_STAGE_H
#pragma once
#include "RecordBaseThread.h"

/*!
* @brief Class for recording without a stage.
*
* This simply does a recording without a stage.
*/
class RecordWithoutStage : public RecordBaseThread
{
public:
    RecordWithoutStage( hyspex::Camera* a_camera );
    ~RecordWithoutStage();
    void setNumberOfFrames( int a_numberOfFrames ); //! number of frames to record, 0 = forever

protected:
    double unitsPerPixel();
    virtual void own_thread();
    std::atomic_int m_numberOfFrames;

private:
    RecordWithoutStage() = delete;
};

#endif // RECORD_WITHOUT_STAGE_H
