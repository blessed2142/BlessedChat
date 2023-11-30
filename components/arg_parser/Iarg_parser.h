#ifndef IARG_PARSER_H
#define IARG_PARSER_H
#include <string>
#include <set>
#include <vector>

struct OptionName
{
     std::string name;
     std::string shortName;
};

struct Option
{
     OptionName               optionName;
     std::string              description;
     bool                     required;
     bool                     repeatable;
     std::vector<std::string> value;
};

class IArgParser
{
public:
     virtual const std::set<Option>& getOptions() = 0;
     virtual bool addOption( const Option& option ) = 0;
     virtual void removeOption( const Option& option ) = 0;
     virtual void parse() = 0;
};


#endif