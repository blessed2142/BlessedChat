#include "poco_arg_parser.h"

PocoArgParser::PocoArgParser()
{

}

const std::set<Option>& PocoArgParser::getOptions()
{
     return options_;
}

bool PocoArgParser::addOption( const Option& option )
{
     optionSet_.addOption(
          Poco::Util::Option( option.name, option.shortName,
                              option.description )
                              .required( option.required )
                              .repeatable( option.repeatable )
                              .argument( option.valueType ) );
}

void PocoArgParser::parse()
{
}

Poco::Util::OptionSet PocoArgParser::getPocoOptionSet()
{
     return optionSet_;
}