#ifndef MESSAGE_DECODER_H
#define MESSAGE_DECODER_H

// POCO
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionSet.h>

class MessageDecoder
{
public:

     MessageDecoder();
     ~MessageDecoder() = default;

     std::string serialize( const std::string& str );
     std::string deserialize( const std::string& str );
};

#endif