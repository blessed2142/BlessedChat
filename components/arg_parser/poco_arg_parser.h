#ifndef ARG_PARSER_H
#define ARG_PARSER_H

// LOCAL
#include "Iarg_parser.h"

// POCO
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionSet.h>

class PocoArgParser : IArgParser
{
public:
     PocoArgParser();

     // OVERRIDE
     virtual const std::set<Option>& getOptions();
     virtual bool addOption( const Option& option );
     virtual void removeOption( const Option& option );
     virtual void parse();

     Poco::Util::OptionSet getPocoOptionSet();
private:
     Poco::Util::OptionSet optionSet_;
     std::set<Option> options_;
};

#endif