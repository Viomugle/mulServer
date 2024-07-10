#pragma once
#include <string>
#include "tools.h"
#include <unordered_map>
#include "ReadBuffer.h"
#include <string_view>
#include <optional>
#include <ranges>
#include <vector>
class ReadBuffer;
class HTTP{
    public:
        DEFAULT(HTTP)
        std::optional<std::unordered_map<std::string,std::string>> parseHTTP(ReadBuffer& _rb); 
};