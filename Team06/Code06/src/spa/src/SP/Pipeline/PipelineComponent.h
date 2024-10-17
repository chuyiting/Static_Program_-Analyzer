#pragma once

template<class I, class O>
class PipelineComponent {
public:
    virtual O process(I input) = 0;
};
