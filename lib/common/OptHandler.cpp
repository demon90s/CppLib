#include "OptHandler.h"
#include "common/string_functions.h"
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

OptHandler::OptHandler() : parse_error_(ParseError::NoError), error_opt_('\0')
{

}

bool OptHandler::Parse(int argc, char **argv, const std::vector<Opt> &opts)
{
    this->Clear();

    std::string optstring(":");
    for (const Opt &opt : opts) {
        optstring += opt.opt;
        if (opt.has_param) {
            optstring += ':';
        }
    }

    char *optstring_tmp = (char*)malloc(optstring.length() + 1);
    strcpy(optstring_tmp, optstring.c_str());

    char opt;
    while ((opt = getopt(argc, argv, optstring_tmp)) != -1) {
        switch(opt) {
            case '?' : 
                error_opt_ = optopt;
                parse_error_ = ParseError::UnknowOpts;
                break;
            case ':' :
                error_opt_ = optopt;
                parse_error_ = ParseError::NoArgument;
                break;

            default:
                if (optarg) {
                    opt_param_cache_.emplace(opt, optarg);
                }
                else {
                    opt_param_cache_.emplace(opt, "");
                }
                break;
        }
    }

    free(optstring_tmp);

    return parse_error_ == ParseError::NoError;
}

std::string OptHandler::ErrorDesc() const
{
    std::string error_desc;
    if (parse_error_ == ParseError::NoError) {
        error_desc = MakeString(ParseError::NoError);
    }
    else if (parse_error_ == ParseError::UnknowOpts) {
        error_desc = MakeString(ParseError::UnknowOpts);
        error_desc += ": ";
        error_desc += error_opt_;
    }
    else if (parse_error_ == ParseError::NoArgument) {
        error_desc = MakeString(ParseError::NoArgument);
        error_desc += ": ";
        error_desc += error_opt_;
    }

    return error_desc;
}

bool OptHandler::HasOpt(char opt) const
{
    return opt_param_cache_.count(opt) > 0;
}

const std::string& OptHandler::GetOptParam(char opt) const
{
    auto it = opt_param_cache_.find(opt);
    if (it == opt_param_cache_.end())
        throw std::runtime_error("OptHandler::GetOptParam no opt");

    return it->second;
}

void OptHandler::Clear()
{
    opt_param_cache_.clear();
    parse_error_ = ParseError::NoError;
    error_opt_ = '\0';
}