#ifndef BASE64_H
#define BASE64_H

#include <string>
#include <sstream>
#include "Poco/Base64Encoder.h"
#include "Poco/Base64Decoder.h"
#include "Poco/StreamCopier.h"

std::string base64decode( const std::string& str );
std::string base64encode( const std::string& str );

#endif