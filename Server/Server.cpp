#include "Server.hpp"

int main(int argc, char** argv)
{
	Poco::AutoPtr<Server> server = new Server;
	try
	{
		server->init(argc, argv);
	}
	catch (Poco::Exception& exc)
	{
          server->logger().log(exc);
          server->usage();
          return Poco::Util::Application::EXIT_CONFIG;
	}
	return server->run();
}