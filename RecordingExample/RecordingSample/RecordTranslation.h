#ifndef RECORD_TRANSLATION_H
#define RECORD_TRANSLATION_H
#pragma once
#include "RecordBaseThread.h"

/*!
* @brief Class for recording with a translation stage.
*
* This class is used to record data with a translation stage, it will move to the start position before recording.
*/
class RecordTranslation : public RecordBaseThread
{
public:
    RecordTranslation( hyspex::Camera* a_camera, hyspex::Stage* a_stage );
    ~RecordTranslation();
    void setLength( double a_cm ); //! Recording length in cm.
    void setStartPosition( double a_cm ); //! Start position in cm ( default 0.0 ).

protected:
    double unitsPerPixel();
    virtual void own_thread();

private:
    RecordTranslation() = delete;
    std::atomic< double > m_length;
    std::atomic< double > m_startPosition;
};

#endif // RECORD_TRANSLATION_H
