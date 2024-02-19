#include "message_decoder.h"

// STD
#include <sstream>

// POCO
#include <Poco/Base64Encoder.h>
#include <Poco/Base64Decoder.h>
#include <Poco/StreamCopier.h>

MessageDecoder::MessageDecoder() {}

std::string MessageDecoder::serialize( const std::string& str )
{
     std::stringstream strStream;
     strStream.str( "" );

     Poco::Base64Encoder encoder( strStream );
     encoder << str;
     encoder.close();

     return strStream.str();
}

std::string MessageDecoder::deserialize( const std::string& str )
{
     std::stringstream strStream;
     strStream.str( str );

     Poco::Base64Decoder decoder( strStream );
     std::string result;
     Poco::StreamCopier::copyToString( decoder, result );

     return result;
}