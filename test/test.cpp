//
// tcpserver.cpp
//
// This sample demonstrates a TCP server.
//
// Copyright (c) 2005-2018, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/TCPServer.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/NumberParser.h"
#include "Poco/Logger.h"
#include "Poco/Process.h"
#include "Poco/NamedEvent.h"
#include <iostream>
#include <components/message_decoder/message_decoder.h>
#include <mutex>


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

namespace
{
     class ClientConnection: public TCPServerConnection
     {
     public:
          ClientConnection(const StreamSocket& s): TCPServerConnection(s)
          {
          }

          void run()
          {
               StreamSocket& ss = socket();
               MessageDecoder mesDec;
               try
               {
                    // char lenBuffer[32];
                    ByteVector lenBuf;
                    lenBuf.resize( sizeof( uint32_t ) );
                    bzero( lenBuf.data(), sizeof( uint32_t ) );
                    // int n = ss.receiveBytes(lenBuffer, sizeof(lenBuffer));
                    int n = ss.receiveBytes( lenBuf.data(), lenBuf.size() );

                    uint32_t dataLen = static_cast<uint32_t>( *lenBuf.data() );
                    std::cout << "Received data length: " << dataLen << std::endl;

                    ByteVector dataBuf;
                    dataBuf.resize( dataLen);
                    bzero( dataBuf.data(), dataBuf.size() );
                    n = ss.receiveBytes( dataBuf.data(), dataBuf.size() );
                    std::string messageStr( dataBuf.begin(), dataBuf.end() );
                    std::cout << "EncodedMessage: " << messageStr << std::endl;

                    std::cout << "Size: " << messageStr.length() << std::endl;
                    std::string decodedMesage = mesDec.deserialize( messageStr );
                    std::cout << "Message is: " << decodedMesage << std::endl;
               }
               catch (Exception& exc)
               {
                    std::cerr << "ClientConnection: " << exc.displayText() << std::endl;
               }
          }
     };

	typedef TCPServerConnectionFactoryImpl<ClientConnection> TCPFactory;
#if defined(POCO_OS_FAMILY_WINDOWS)
	NamedEvent terminator(ProcessImpl::terminationEventName(Process::id()));
#else
	Event terminator;
#endif
}


int main()
{
     // std::mutex logMutex;

     // Blessed::Logger logger( logMutex, "", Blessed::Logger::LogLevel::DEBUG );

     // logger.info << "qq" << 1 << 'c' << "\n";
     return ( 0 );
}

// int main(int argc, char** argv)
// {
// 	try
// 	{
// 		Poco::UInt16 port = NumberParser::parse((argc > 1) ? argv[1] : "2001");

// 		TCPServer srv(new TCPFactory(), port);
// 		srv.start();

// 		std::cout << "TCP server listening on port " << port << '.'
// 			 << std::endl << "Press Ctrl-C to quit." << std::endl;

// 		terminator.wait();
// 	}
// 	catch (Exception& exc)
// 	{
// 		std::cerr << exc.displayText() << std::endl;
// 		return 1;
// 	}
// 	return 0;
// }
