#include <Server.hpp>
#include <arg_parser/poco_arg_parser.h>

void addOptions( PocoArgParser& argParser )
{
     std::vector<Option> options_vec =
     {
          { "user", "u", "username", true, false, "value" },
          { "config", "c", "config file", true, false, "file" },
          // { "dbfile", "d", "db file", true, false, "file" },
          // { "multiconnect", "m", "multiconnect", false, false, "" }
     };

     for ( const auto& option : options_vec )
     {
          argParser.addOption( option );
     }
}

int main(int argc, char** argv)
{
     PocoArgParser argParser;
     addOptions( argParser );

     Poco::AutoPtr<Server> server = new Server( argParser.getPocoOptionSet() );
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