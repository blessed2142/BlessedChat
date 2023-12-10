#ifndef IARG_PARSER_H
#define IARG_PARSER_H
#include <string>
#include <set>
#include <vector>

struct Option
{
     std::string              name;
     std::string              shortName;
     std::string              description;
     bool                     required;
     bool                     repeatable;
     std::string              valueType;
};

class IArgParser
{
public:
     virtual const std::set<Option>& getOptions() = 0;
     virtual bool addOption( const Option& option ) = 0;
     virtual void parse() = 0;
};


#endif