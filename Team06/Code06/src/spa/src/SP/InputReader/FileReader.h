#pragma once

#include <string>

#include "SP/Pipeline/PipelineComponent.h"

class FileReader : public PipelineComponent<std::string, std::string> {

public:
    FileReader();

    std::string process(std::string filepath);

};
