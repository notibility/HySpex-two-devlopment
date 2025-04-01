#ifndef HYSPEX_STAGE_MANAGER_H
#define HYSPEX_STAGE_MANAGER_H
#pragma once
#include <cstddef>
#include "HySpexLibraryExport.h"

namespace hyspex
{
    class Stage;
    class StageManagerImpl;

    /*!
    * @brief Class for detecting and enumerating stages
    *
    * This class uses the pImpl idiom to reduce external dependencies.
    * We also want to avoid relying on STL for maximum compatibility.
    *
    * EXAMPLE:
    * @code
    * std::string path = "C://hyspex_auxiliary.json";
    *
    * hyspex::StageManager* manager = new hyspex::StageManager();
    *
    * // Read stages config file from path
    * manager->readStagesConfigFile(path.c_str());
    *
    * // Detect stages and set type according to config file.
    * manager->detectStages();
    *
    * // Iterate over available stages (if any)
    * for (int i = 0; i < manager->getStageCount(); i++)
    * {
    *    hyspex::Stage* stage = manager->getStageByIndex(i);
    *    // do operations on stage here.
    * }
    *
    * // destroy StageManager after using stages.
    * delete manager; // will destroy stages as well.
    * @endcode
    */
    class HYSPEXLIBRARY_API StageManager
    {
    public:
        StageManager( const char* a_path ); //!< Constructor, requires path to hyspex_auxiliary.json file, calls readStagesConfigFile.
        StageManager(); //!< Default constructor, if no stages.ini is needed, or if we want to call readStagesConfigFile() later.
        ~StageManager(); //!< Destructor
        void readStagesConfigFile( const char* a_path ); //!< read stages config file, not needed for rotational stage.
        void detectStages(); //!< attempts to load driver to detect Stages.
        size_t getStageCount(); //!< Stages detected.
        Stage* getStageByIndex( size_t a_index ); //!< get stage by index.

    private:
        // disallow copy-constructors
        StageManager( const StageManager& that );
        StageManager& operator=( const StageManager& that );
        StageManagerImpl* m_pImpl;
    };
}

#endif // HYSPEX_STAGE_MANAGER_H
