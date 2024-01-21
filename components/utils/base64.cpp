#include "base64.h"

std::string base64encode( const std::string& str )
{
     std::stringstream strStream;
     strStream.str( "" );

     Poco::Base64Encoder encoder( strStream );
     encoder << str;
     encoder.close();

     return strStream.str();
}

std::string base64decode( const std::string& str )
{
     std::stringstream strStream;
     strStream.str( str );

     Poco::Base64Decoder decoder( strStream );
     std::string result;
     Poco::StreamCopier::copyToString( decoder, result );

     return result;
}