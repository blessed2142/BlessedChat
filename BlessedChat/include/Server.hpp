#ifndef SERVER_HPP
#define SERVER_HPP

#include <Poco/Util/Application.h>
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

Server( const OptionSet& options );
void usage();

protected:
virtual void initialize( Application& self ) override;
virtual void uninitialize() override;
virtual void reinitialize( Application& self ) override;
virtual void defineOptions( OptionSet& options ) override;
void handleHelp( const std::string& name, const std::string& value );
void handleAsync( const std::string& name, const std::string& value );
void handleConfig( const std::string& name, const std::string& value );
void handleDb( const std::string& name, const std::string& value );
void handleUser( const std::string& name, const std::string& value );
void displayHelp();
virtual int main( const ArgVec& args ) override;
void printProperties(const std::string& base);
std::string getConfigValue( const std::string& key );
void parseConfigFile();

private:
	bool                     _helpRequested;
     bool                     multiconnect_;
     std::set<std::string>    userNames_;
     std::string              configPath_;
     std::string              dbPath_;
     OptionSet                options_;
};

#endif