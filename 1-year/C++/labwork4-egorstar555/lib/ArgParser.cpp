#include "ArgParser.h"

using namespace ArgumentParser;

ArgParser::ArgParser(const std::string& s)
        : parser_name{s} {
}

int ArgParser::ArgCount() {
    int cnt = 1;
    for (Argument* a: arg_args) {
        if ((a->_type == "string" || a->_type == "integer") && !a->_default_value) {
            ++cnt;
        }
    }
    cnt += _is_help_argument ? 1 : 0;

    return cnt;
}

bool ArgParser::FindValue(const std::string& name, const std::string& args, std::string& value, int& pos) {
    size_t position_arg;
    size_t position_blank;

    position_arg = args.find(name, pos);
    pos = position_arg + 1;
    if (position_arg == std::string::npos) {
        return false;
    }
    position_blank = args.find(" ", position_arg);
    if (position_blank != std::string::npos) {
        value = args.substr(position_arg + name.size(),
                            position_blank - (position_arg + name.size()));
    } else {
        value = args.substr(position_arg + name.size());
    }

    return true;
}

bool ArgParser::StringParse(const std::vector<std::string>& args_sub_str) {
    std::string args;
    for (const std::string& s: args_sub_str) {
        args += s + " ";
    }

    std::string name;
    std::string value;
    int find_args_count = 0;
    int need_args_count = 0;
    bool is_find_args;
    int pos_full = 0;
    int pos_small = 0;
    StringArgument* s;
    for (Argument* a: arg_args) {
        if (a->_type == "string") {
            s = {static_cast<StringArgument*>(a)};
            do {
                name = "--" + s->_full_name + "=";
                is_find_args = FindValue(name, args, value, pos_full);
                if (!is_find_args) {
                    std::string ch(1, s->_short_name);
                    name = "-" + ch + "=";
                    is_find_args = FindValue(name, args, value, pos_small);
                }
                if (is_find_args) {
                    ++find_args_count;
                    s->_value = value;
                    if (s->_store.has_value()) {
                        *(*(s->_store)) = s->_value;
                    }
                    if (s->_multi_value) {
                        (*(*(s->_store_multi))).push_back(s->_value);
                        s->_values.push_back(s->_value);
                    }
                }
            } while (s->_multi_value && is_find_args);
            if (s->_multi_value) {
                need_args_count += s->_min_args_count;
            } else {
                ++need_args_count;
            }
        }
    }

    return find_args_count >= need_args_count;
}

bool ArgParser::IntParse(const std::vector<std::string>& args_sub_str) {
    std::string args;
    for (const std::string& s: args_sub_str) {
        args += s + " ";
    }

    std::string name;
    std::string value;
    int find_args_count = 0;
    int need_args_count = 0;
    bool is_find_args;
    int pos_full = 0;
    int pos_small = 0;

    IntArgument* s;
    for (Argument* a: arg_args) {
        if (a->_type == "integer") {
            s = {static_cast<IntArgument*>(a)};
            if (s->_positional) {
                for (int i = 1; i < args_sub_str.size(); ++i) {
                    if (args_sub_str[i][0] != '-') {
                        s->_value = std::stoi(args_sub_str[i]);
                        (*(*(s->_store_multi))).push_back(s->_value);
                        s->_values.push_back(s->_value);
                        ++find_args_count;
                    }
                }
            } else {
                do {
                    name = "--" + s->_full_name + "=";
                    is_find_args = FindValue(name, args, value, pos_full);
                    if (!is_find_args) {
                        std::string ch(1, s->_short_name);
                        name = "-" + ch + "=";
                        is_find_args = FindValue(name, args, value, pos_small);
                    }
                    if (is_find_args) {
                        ++find_args_count;
                        s->_value = std::stoi(value);
                        if (s->_store.has_value()) {
                            *(*(s->_store)) = s->_value;
                        }
                        if (s->_multi_value) {
                            (*(*(s->_store_multi))).push_back(s->_value);
                            s->_values.push_back(s->_value);
                        }
                    }
                } while (s->_multi_value && is_find_args);
            }
            if (s->_multi_value) {
                need_args_count += s->_min_args_count;
            } else {
                ++need_args_count;
            }
        }
    }

    return find_args_count >= need_args_count;
}

bool ArgParser::FlagParse(const std::vector<std::string>& args_sub_str) {
    std::string args;
    for (const std::string& s: args_sub_str) {
        args += s + " ";
    }

    std::string name;
    std::string value;
    int pos_full = 0;
    bool is_find_full = false;
    FlagArgument* s;
    for (Argument* f: arg_args) {
        if (f->_type == "flag") {
            s = {static_cast<FlagArgument*>(f)};
            name = "--" + s->_full_name;
            s->_value = FindValue(name, args, value, pos_full);
            is_find_full = is_find_full || s->_value;
            if (!s->_value) {
                s->_value = s->_default_value;
            }
            if (s->_store.has_value()) {
                *(*(s->_store)) = s->_value;
            }
            if (!is_find_full) {
                name = "-";
                if (FindValue(name, args, value, pos_full)) {
                    for (Argument* f2: arg_args) {
                        if (f2->_type == "flag") {
                            s = {static_cast<FlagArgument*>(f)};
                            std::string ch(1, s->_short_name);
                            if (value.find(ch) == std::string::npos) {
                                s->_value = s->_default_value;
                            } else {
                                s->_value = true;
                            }
                            if (s->_store.has_value()) {
                                *(*(s->_store)) = s->_value;
                            }
                        }
                    }
                }
            }
        }
    }

    return true;
}

bool ArgParser::Parse(const std::vector<std::string>& args_sub_str) {
    bool res = true;
    if (args_sub_str.size() == ArgCount() && args_sub_str.size() == 1) {
        return true;
    }
    std::string ch(1, _short_name_help_argument);
    if (_is_help_argument &&
        (args_sub_str[1] == "--" + _full_name_help_argument || args_sub_str[1] == "-" + ch)) {
        _is_help_argument_use = true;
        return true;
    }
    if (_is_string_args) {
        res = StringParse(args_sub_str);
    }
    if (_is_int_args) {
        res = IntParse(args_sub_str);
    }
    if (_is_flag_args) {
        res = FlagParse(args_sub_str);
    }

    return res;
}

bool ArgParser::Parse(int argc, char** argv) {
    std::vector<std::string> parse;
    for (int i = 0; i < argc; i++) {
        parse.push_back(argv[i]);
    }

    return Parse(parse);
}

ArgParser::StringArgument& ArgParser::AddStringArgument(const std::string& full_name, const std::string& help) {
    return AddStringArgument(' ', full_name, help);
}

ArgParser::StringArgument& ArgParser::AddStringArgument(char short_name,
                                                        const std::string& full_name,
                                                        const std::string& help) {
    StringArgument* x = new StringArgument(short_name, full_name, help, "string");

    arg_args.push_back(x);
    _is_string_args = true;

    return *x;
}

const std::string& ArgParser::GetStringValue(const std::string& full_name, size_t index) {
    for (auto& string_argument: arg_args) {
        if (string_argument->_type == "string") {
            StringArgument* s{static_cast<StringArgument*>(string_argument)};
            if (string_argument->_full_name == full_name) {
                if (string_argument->_multi_value) {
                    return s->_values[index];
                } else {
                    return s->_value;
                }
            }
        }
    }

    return std::string();
}

ArgParser::IntArgument& ArgParser::AddIntArgument(const std::string& full_name, const std::string& help) {
    return AddIntArgument(' ', full_name, help);
}

ArgParser::IntArgument&
ArgParser::AddIntArgument(char short_name, const std::string& full_name, const std::string& help) {
    IntArgument* x = new IntArgument(short_name, full_name, help, "integer");
    arg_args.push_back(x);
    _is_int_args = true;

    return *x;
}

int ArgParser::GetIntValue(const std::string& full_name, size_t index) {
    for (auto& i: arg_args) {
        if (i->_type == "integer") {
            IntArgument* int_argument{static_cast<IntArgument*>(i)};

            if (int_argument->_full_name == full_name) {
                if (int_argument->_multi_value) {
                    return int_argument->_values[index];
                } else {
                    return int_argument->_value;
                }
            }
        }
    }

    return 0;
}

ArgParser::FlagArgument& ArgParser::AddFlag(const std::string& full_name, const std::string& help) {
    return AddFlag(' ', full_name, help);
}

ArgParser::FlagArgument&
ArgParser::AddFlag(char short_name, const std::string& full_name, const std::string& help) {
    FlagArgument* x = new FlagArgument(short_name, full_name, help, "flag");

    arg_args.push_back(x);
    _is_flag_args = true;

    return *x;
}

bool ArgParser::GetFlag(const std::string& full_name) {
    for (auto& f: arg_args) {
        if (f->_type == "flag") {
            FlagArgument* flag_argument{static_cast<FlagArgument*>(f)};
            if (flag_argument->_full_name == full_name) {
                return flag_argument->_value;
            }
        }
    }

    return false;
}

void ArgParser::AddHelp(char short_name, const std::string& full_name, const std::string& help) {
    _is_help_argument = true;
    _full_name_help_argument = full_name;
    _short_name_help_argument = short_name;
    _help_help_argument = help;
}

const std::string ArgParser::HelpDescription() {
    std::string res = parser_name + '\n' + _help_help_argument + '\n' + '\n';

    StringArgument* string_argument;
    IntArgument* int_argument;
    FlagArgument* flag_argument;
    for (auto& a: arg_args) {
        if (a->_type == "string") {
            string_argument = {static_cast<StringArgument*>(a)};
            res = res + '-' + string_argument->_short_name + ", --" + string_argument->_full_name +
                  "=<string>,  " + string_argument->_help;
            if (string_argument->_min_args_count != 0) {
                res += " [repeated, min args = " + std::to_string(string_argument->_min_args_count) + "]";
            }
            if (string_argument->_default_value) {
                res += " [default = " + string_argument->_value + "]";
            }
            res += "\n";
        }
        if (a->_type == "integer") {
            int_argument = {static_cast<IntArgument*>(a)};
            res = res + '-' + int_argument->_short_name + ", --" + int_argument->_full_name +
                  "=<int>,  " + int_argument->_help;
            if (int_argument->_min_args_count != 0) {
                res += " [repeated, min args = " + std::to_string(int_argument->_min_args_count) + "]";
            }
            if (int_argument->_default_value) {
                res += " [default = " + std::to_string(int_argument->_value) + "]";
            }
            res += "\n";
        }
        if (a->_type == "flag") {
            flag_argument = {static_cast<FlagArgument*>(a)};
            res = res + '-' + flag_argument->_short_name + ", --" + flag_argument->_full_name +
                  "=<flag>,  " + flag_argument->_help;

            if (flag_argument->_default_value) {
                if (flag_argument->_value) {
                    res += " [default = true]\n";
                } else {
                    res += " [default = false]\n";
                }
            }
        }
    }

    res = res + "\n\n" + '-' + _short_name_help_argument + ", --" + _full_name_help_argument +
          " Display this help and exit\n";

    return res;
}

bool ArgParser::Help() const {
    return _is_help_argument_use;
}