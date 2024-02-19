#include <algorithm>
#include <iostream>
#include "Poco/Net/TCPServer.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/NumberParser.h"
#include "Poco/Logger.h"
#include "Poco/Process.h"
#include "Poco/NamedEvent.h"
#include <components/message_decoder/message_decoder.h>


using Poco::Net::TCPServer;
using Poco::Net::TCPServerConnectionFilter;
using Poco::Net::TCPServerConnection;
using Poco::Net::TCPServerConnectionFactory;
using Poco::Net::TCPServerConnectionFactoryImpl;
using Poco::Net::StreamSocket;
using Poco::UInt16;
using Poco::NumberParser;
using Poco::Logger;
using Poco::Event;
using Poco::NamedEvent;
using Poco::Process;
using Poco::ProcessImpl;
using Poco::Exception;

typedef std::vector<unsigned char> ByteVector;


int main()
{
     // Открываем сокет
     Poco::Net::SocketAddress sa( "127.0.0.1", 2001 );
     StreamSocket ss( sa );

     // Создаем экземпляр MessageDecoder
     MessageDecoder mesDec;

     // // Готовим сообщение
     std::string message( "{\"wassup test 123\"} русские буквы и русские слова:::" );
     std::string encodedMessage = mesDec.serialize( message );
     // B64encode( message, encodedMessage );
     // std::cout << "en: " << encodedMessage << std::endl;
     // std::string decodedMessage;
     // B64decode( encodedMessage, decodedMessage );
     // std::cout << "de: " << decodedMessage << std::endl;
     // Отправляем длину сообщения 
     ByteVector lenBuf;
     lenBuf.resize( sizeof( uint32_t ) );
     bzero( lenBuf.data(), lenBuf.size() );
     lenBuf[0] = static_cast<uint32_t>( encodedMessage.size() );
     ss.sendBytes( lenBuf.data(), lenBuf.size() );

     // Отправляем сообщение
     ByteVector dataVector( encodedMessage.begin(), encodedMessage.end() );
     ss.sendBytes( dataVector.data(), dataVector.size() );
     ss.shutdownSend();
}