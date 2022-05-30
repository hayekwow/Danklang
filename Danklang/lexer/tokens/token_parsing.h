#pragma once

#include <iostream>

#include "token_metadata.h"

std::pair<Token, std::string> get_token(std::ifstream& input_stream);
