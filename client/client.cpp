#include "Poco/Net/TCPServer.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/NumberParser.h"
#include "Poco/Logger.h"
#include "Poco/Process.h"
#include "Poco/NamedEvent.h"
#include <iostream>


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
     Poco::Net::SocketAddress sa( "127.0.0.1", 2001 );
     StreamSocket ss( sa );

     std::string wassup( "wassup" );
     ss.sendBytes( wassup.data(), static_cast<int>( wassup.length() ) );
     char buf[256];
     bzero( buf, 256 );
     ss.shutdownSend();

     int rb = ss.receiveBytes( buf, sizeof( buf ) );
     std::cout << "Received " << rb << " bytes. Msg: " << buf << std::endl;
}