#include "Poco/Net/TCPServer.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/NumberParser.h"
#include "Poco/Logger.h"
#include "Poco/Process.h"
#include "Poco/NamedEvent.h"
#include <iostream>
#include <components/utils/base64.h>
#include <components/utils/utils.h>


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

int main()
{
     // Открываем сокет
     Poco::Net::SocketAddress sa( "127.0.0.1", 2001 );
     StreamSocket ss( sa );

     // Готовим сообщение
     std::string message( "{\"wassup test 123\"} русские буквы и русские слова:::" );
     std::string encodedMessage = base64encode( message );

     // Отправляем длину сообщения
     ss.sendBytes( std::bitset<32>( encodedMessage.size() ).to_string().data(),
                   std::bitset<32>( encodedMessage.size() ).to_string().size() );
     char buf[256];
     bzero( buf, 256 );
     int rb = ss.receiveBytes( buf, sizeof( buf ) );
     std::cout << "EncodedMessage: " << encodedMessage << " size: " << encodedMessage.size() << std::endl;
     ss.sendBytes( encodedMessage.data(), encodedMessage.size() );

     bzero( buf, 256 );
     rb = ss.receiveBytes( buf, sizeof( buf ) );
     ss.shutdownSend();

     std::cout << "Received " << rb << " bytes. Msg: " << buf << std::endl;
}