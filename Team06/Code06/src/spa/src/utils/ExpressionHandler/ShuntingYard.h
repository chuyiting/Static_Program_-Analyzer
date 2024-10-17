#pragma once

#include <iostream>
#include <memory>
#include <stack>

#include "utils/TokenHandler/ParseableToken.h"
#include "utils/TokenHandler/TokenStream.h"

class ShuntingYard {
public:
    static std::shared_ptr<TokenStream> convertToPostfix(std::shared_ptr<TokenStream> tokens);
};
