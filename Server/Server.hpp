#ifndef SERVER_HPP
#define SERVER_HPP

#include <Poco/Util/Application.h>
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionSet.h>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Util/AbstractConfiguration.h>
#include <Poco/AutoPtr.h>
#include <iostream>
#include <sstream>
#include <set>


using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Util::AbstractConfiguration;
using Poco::Util::OptionCallback;
using Poco::AutoPtr;


class Server: public Application
{
public:
	Server(): _helpRequested( false ), multiconnect_( false )
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
		Application::defineOptions( options );

		options.addOption(
			Option( "help", "h", "display help information on command line arguments" )
				.required( false )
				.repeatable( false )
				.callback( OptionCallback<Server>( this, &Server::handleHelp ) ) );

		options.addOption(
			Option( "user", "u", "define user" )
				.required( true )
				.repeatable( true )
				.argument( "value" )
				.callback( OptionCallback<Server>( this, &Server::handleUser ) ) );

		options.addOption(
			Option( "config", "c", "load configuration data from a file" )
				.required( true )
				.repeatable( false )
				.argument( "value" )
				.callback( OptionCallback<Server>( this, &Server::handleConfig ) ) );

		options.addOption(
			Option( "db-file", "d", "load user passwords from a file" )
				.required( false )
				.repeatable( false )
				.argument( "value" )
				.callback( OptionCallback<Server>( this, &Server::handleDb ) ) );

		options.addOption(
			Option("multiconnect", "m", "enable async mode")
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<Server>(this, &Server::handleAsync)));
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
		if ( !_helpRequested )
		{
			logger().information( "users:" );
               size_t i = 0;
               for ( auto& user : userNames_ )
               {
                    std::cout << ++i << ") " << user << std::endl;
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
		}
		return Application::EXIT_OK;
	}

private:
	bool                     _helpRequested;
     bool                     multiconnect_;
     std::set<std::string>    userNames_;
     std::string              configPath_;
     std::string              dbPath_;
};

#endif