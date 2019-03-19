#pragma once

#include <string>
#include <vector>
#include <unordered_map>

// 一个解析命令行选项的工具，封装 getopt

struct Opt {
    char opt;           // 选项
    bool has_param;     // 是否有参数
};

class OptHandler {
public:
    OptHandler();

    enum class ParseError {
        NoError,
        UnknowOpts,
        NoArgument,
    };
    bool Parse(int argc, char **argv, const std::vector<Opt> &opts);

    std::string ErrorDesc() const;

    bool HasOpt(char opt) const;
    const std::string& GetOptParam(char opt) const;

private:
    void Clear();

    std::unordered_map<char, std::string> opt_param_cache_;
    ParseError parse_error_;
    char error_opt_;
};
