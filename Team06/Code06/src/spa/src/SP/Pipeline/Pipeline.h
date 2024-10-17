#pragma once

#include <memory>
#include <vector>

#include "SP/Pipeline/PipelineComponent.h"


template<typename I>
class Pipeline {
public:

    I startingInput;

    Pipeline(I input);

    template<typename E>
    Pipeline<E> process(std::shared_ptr<PipelineComponent<I, E>> newComponent);

};


template<typename I>
inline
Pipeline<I>::Pipeline(I input) {
    startingInput = input;
}

template<typename I>
template<typename E>
inline
Pipeline<E> Pipeline<I>::process(std::shared_ptr<PipelineComponent<I, E>> newComponent) {
    return Pipeline<E>(
            newComponent->process(startingInput)
    );
}
