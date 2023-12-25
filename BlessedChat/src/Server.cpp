#include <Server.hpp>
#include <arg_parser/poco_arg_parser.h>

Server::Server( const OptionSet& options ):
_helpRequested( false ), multiconnect_( false ), options_( options )
{
}


void Server::usage()
{
     handleHelp( "", "" );
}


void Server::initialize( Application& self )
{
     // loadConfiguration(); // load default configuration files, if present
     Application::initialize( self );
}


void Server::uninitialize()
{
     Application::uninitialize();
}


void Server::reinitialize( Application& self )
{
     Application::reinitialize( self );
}


void Server::defineOptions( OptionSet& options )
{
     Application::defineOptions( options_ );
     options = options_;
     options.addOption(
          PU::Option( "help", "h", "display help information on command line arguments" )
               .required( false )
               .repeatable( false )
               .callback( OptionCallback<Server>( this, &Server::handleHelp ) ) );
}


void Server::handleHelp( const std::string& name, const std::string& value )
{
     _helpRequested = true;
     displayHelp();
     stopOptionsProcessing();
}


void Server::handleAsync( const std::string& name, const std::string& value )
{
     multiconnect_ = true;
}


void Server::handleConfig( const std::string& name, const std::string& value )
{
     configPath_ = value;
}


void Server::handleDb( const std::string& name, const std::string& value )
{
     dbPath_ = value;
}


void Server::handleUser( const std::string& name, const std::string& value )
{
     userNames_.insert( value );
}


void Server::displayHelp()
{
     HelpFormatter helpFormatter( options() );
     helpFormatter.setCommand( commandName() );
     helpFormatter.setUsage( "OPTIONS" );
     helpFormatter.format( std::cout );
}


int Server::main( const ArgVec& args )
{
     parseConfigFile();
	if ( !_helpRequested )
	{
		logger().information( "users:" );
          std::cout << "users: " << std::endl;
          size_t i = 0;
          for ( auto& user : userNames_ )
          {
               std::cout << "user" << ++i << ") " << user << std::endl;
          }
          std::cout << std::endl;
          logger().information( "config_path: " + configPath_ );
          if ( !dbPath_.empty() )
          {
               logger().information( "db_path: " + dbPath_ );
          }
          if ( multiconnect_ == true )
          {
               logger().information( "Server is in async mode " );
          }
          else 
          {
               logger().information( "Server is in sync mode " );
          }
          printProperties( "" );
     }
     return Application::EXIT_OK;
}

void Server::printProperties(const std::string& base)
{
     AbstractConfiguration::Keys keys;
     config().keys(base, keys);
     if (keys.empty())
     {
          if (config().hasProperty(base))
          {
               std::string msg;
               msg.append(base);
               msg.append(" = ");
               msg.append(config().getString(base));
               logger().information(msg);
          }
     }
     else
     {
          for (AbstractConfiguration::Keys::const_iterator it = keys.begin(); it != keys.end(); ++it)
          {
               std::string fullKey = base;
               if (!fullKey.empty()) fullKey += '.';
               fullKey.append(*it);
               printProperties(fullKey);
          }
     }
}

std::string Server::getConfigValue( const std::string& key )
{
     if ( config().hasProperty( key ) )
     {
          return config().getString( key );
     }
     return std::string();
}


void Server::parseConfigFile()
{
     /*   1. Найти файл. Проверить валидность.
          2. Считать опции.                  */
     const std::string cfgOptionName = "config";
     std::string cfgFilePath;
     if ( config().hasOption( cfgOptionName ) )
     {
          cfgFilePath = config().getString( cfgOptionName );
     }
     else
     {
#ifdef MAIN_DIR
          cfgFilePath = MAIN_DIR;
          cfgFilePath.erase( cfgFilePath.begin() );
          cfgFilePath.erase( --cfgFilePath.end() );
          cfgFilePath += "/settings.ini";
#else
          throw Poco::Util::MissingOptionException( "Config file path was not defined" );
#endif
     }
     if ( !std::filesystem::exists( cfgFilePath ) ||
          !std::filesystem::is_regular_file( cfgFilePath ) )
     {
          throw Poco::Util::MissingOptionException( "Couldnt load config file" );
     }
     loadConfiguration( cfgFilePath );
}

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