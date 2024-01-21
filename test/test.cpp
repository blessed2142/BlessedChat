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
               try
               {
                    char lenBuffer[32];
                    int n = ss.receiveBytes(lenBuffer, sizeof(lenBuffer));

                    std::stringstream sstream( lenBuffer );
                    std::bitset<32> lenBits;
                    sstream >> lenBits;
                    const int encodedLen = static_cast<int>( lenBits.to_ulong() ) + 1;
                    std::cout << "EncodedLen: " << encodedLen << std::endl;

                    char messageBuffer[encodedLen];
                    bzero( messageBuffer, encodedLen );
                    ss.sendBytes( lenBuffer, sizeof( lenBuffer) );
                    n = ss.receiveBytes( messageBuffer, sizeof( messageBuffer ) );

                    std::string messageStr( messageBuffer );
                    std::cout << "EncodedMessage: " << messageBuffer << std::endl;
                    // while ( n > 0 )
                    // {
                    // 	std::cout << "Received " << n << " bytes:" << std::endl;
                    // 	msg += buffer;
                    // 	n = ss.receiveBytes(buffer, sizeof(buffer));
                    // }
                    std::string decodedMesage = base64decode( messageStr );
                    std::cout << "Message is: " << std::endl
                              << decodedMesage;
                    ss.sendBytes( decodedMesage.data(), static_cast<int>( decodedMesage.length() ) );
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

int main(int argc, char** argv)
{
	try
	{
		Poco::UInt16 port = NumberParser::parse((argc > 1) ? argv[1] : "2001");

		TCPServer srv(new TCPFactory(), port);
		srv.start();

		std::cout << "TCP server listening on port " << port << '.'
			 << std::endl << "Press Ctrl-C to quit." << std::endl;

		terminator.wait();
	}
	catch (Exception& exc)
	{
		std::cerr << exc.displayText() << std::endl;
		return 1;
	}
// eyJ3YXNzdXAgdGVzdCAxMjMifSDRgNGD0YHRgdC60LjQtSDQsdGD0LrQstGLINC4INGA0YPR
// gdGB0LrQuNC1INGB0LvQvtCy0LA6Ojo=;�dU


     // std::string str = "{badr hari:[][] qqq{}}";

     // std::string original( "{\"wassup test 123\"} русские буквы и русские слова:::" );
     // std::string encoded = base64encode( original );
     // std::string decoded = base64decode( encoded );


     // std::cout << "original: " << original << "\n"
     //           << "encoded:  " << encoded << " size: " << encoded.size() << "\n"
     //           << "encoded:  " << decoded << " size: " << decoded.size() << "\n";

     // std::string str = "Пап привет как дела";
     // std::string encoded = base64encode( str );
     // std::string decoded = base64decode( encoded );
     // std::cout << "original: " << str << "\n"
     //           << "encoded:  " << encoded << "\n"
     //           << "decoded:  " << decoded << "\n";
	return 0;
}
