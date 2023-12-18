#ifndef SERVER_HPP
#define SERVER_HPP

#include <Poco/Util/Application.h>
#include <Poco/Util/ServerApplication.h>
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionException.h>
#include <Poco/Util/OptionSet.h>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Util/AbstractConfiguration.h>
#include <Poco/AutoPtr.h>
#include <iostream>
#include <sstream>
#include <set>
#include <filesystem>

// using PocoApp = Poco::Util::Application;
// using PocoOption = Poco::Util::Option;

namespace PU = Poco::Util;

using Poco::Util::Application;
// using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Util::AbstractConfiguration;
using Poco::Util::OptionCallback;
using Poco::AutoPtr;


class Server: public Application
{
public:
     Server( const OptionSet& options ): _helpRequested( false ), multiconnect_( false ), options_( options )
     {
     }
     void usage()
     {
          handleHelp( "", "" );
     }

protected:
	void initialize( Application& self )
	{
		// loadConfiguration(); // load default configuration files, if present
		Application::initialize( self );
	}

	void uninitialize()
	{
		Application::uninitialize();
	}

	void reinitialize( Application& self )
	{
		Application::reinitialize( self );
	}

	void defineOptions( OptionSet& options )
	{
		Application::defineOptions( options_ );
          options = options_;
		options.addOption(
			PU::Option( "help", "h", "display help information on command line arguments" )
				.required( false )
				.repeatable( false )
				.callback( OptionCallback<Server>( this, &Server::handleHelp ) ) );
	}

	void handleHelp( const std::string& name, const std::string& value )
	{
		_helpRequested = true;
		displayHelp();
		stopOptionsProcessing();
	}

	void handleAsync( const std::string& name, const std::string& value )
	{
          multiconnect_ = true;
	}

	void handleConfig( const std::string& name, const std::string& value )
	{
          configPath_ = value;
	}

	void handleDb( const std::string& name, const std::string& value )
	{
          dbPath_ = value;
	}

     void handleUser( const std::string& name, const std::string& value )
     {
          userNames_.insert( value );
     }

	void displayHelp()
	{
		HelpFormatter helpFormatter( options() );
		helpFormatter.setCommand( commandName() );
		helpFormatter.setUsage( "OPTIONS" );
		helpFormatter.format( std::cout );
	}

	int main( const ArgVec& args )
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
               // auto it = options_.begin();
               // while ( it != options_.end() )
               // {
               //      std::cout << it->fullName() << " " << it-> << std::endl;
               //      it++;
               // }

		}
		return Application::EXIT_OK;
	}

	void printProperties(const std::string& base)
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

     std::string getConfigValue( const std::string& key )
     {
          if ( config().hasProperty( key ) )
          {
               return config().getString( key );
          }
          return std::string();
     }

     void parseConfigFile()
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

private:
	bool                     _helpRequested;
     bool                     multiconnect_;
     std::set<std::string>    userNames_;
     std::string              configPath_;
     std::string              dbPath_;
     OptionSet                options_;
};

#endif