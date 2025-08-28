#pragma once

#include <iostream>
#include <vector>
#include <optional>

namespace ArgumentParser {
    class ArgParser {
    private:
        class Argument;

        class StringArgument;

        class IntArgument;

        class FlagArgument;

        int ArgCount();

        bool FindValue(const std::string& full_name, const std::string& args, std::string& value, int& pos);

        bool StringParse(const std::vector<std::string>& args_sub_str);

        bool IntParse(const std::vector<std::string>& args_sub_str);

        bool FlagParse(const std::vector<std::string>& args_sub_str);

        std::vector<Argument*> arg_args;

        std::string _full_name_help_argument;
        char _short_name_help_argument;
        std::string _help_help_argument;
        bool _is_help_argument = false;
        bool _is_help_argument_use = false;
        std::string parser_name;
        bool _is_string_args = false;
        bool _is_int_args = false;
        bool _is_flag_args = false;

    public:

        ArgParser(const std::string& s);

        bool Parse(int argc, char** argv);

        bool Parse(const std::vector<std::string>& args_sub_str);

        StringArgument& AddStringArgument(const std::string& full_name, const std::string& help = "");

        StringArgument& AddStringArgument(char short_name, const std::string& full_name, const std::string& help = "");

        const std::string& GetStringValue(const std::string& full_name, size_t index = 0);

        IntArgument& AddIntArgument(const std::string& full_name, const std::string& help = "");

        IntArgument& AddIntArgument(char short_name, const std::string& full_name, const std::string& help = "");

        int GetIntValue(const std::string& full_name, size_t index = 0);

        FlagArgument& AddFlag(const std::string& full_name, const std::string& help = "");

        FlagArgument& AddFlag(char short_name, const std::string& full_name, const std::string& help = "");

        bool GetFlag(const std::string& full_name);

        void AddHelp(char short_name, const std::string& full_name, const std::string& help);

        const std::string HelpDescription();

        bool Help() const;

        ~ArgParser() {
            for (auto& argument: arg_args) {
                delete argument;
            }
        };
    };

    class ArgParser::Argument {
        friend ArgParser;
    public:
        Argument(
                char short_name,
                const std::string& full_name,
                const std::string& help,
                const std::string& type)
                : _short_name{short_name}, _full_name{full_name}, _help{help}, _type{type} {}

    protected:
        char _short_name = ' ';
        bool _default_value = false;
        bool _multi_value = false;
        bool _positional = false;
        size_t _min_args_count = 0;
        std::string _full_name;
        std::string _help;
        std::string _type;
    };

    class ArgParser::StringArgument : public Argument {
        friend ArgParser;
    public:
        StringArgument(
                char short_name,
                const std::string& full_name,
                const std::string& help,
                const std::string& type) :
                Argument(short_name, full_name, help, type) {}

        StringArgument& Default(const std::string& value) {
            _value = value;
            _default_value = true;

            return *this;
        }

        StringArgument& StoreValue(std::string& value) {
            _store = &value;
            value = _value;

            return *this;
        }

        StringArgument& MultiValue(size_t min_args_count = 0) {
            _multi_value = true;
            _min_args_count = min_args_count;

            return *this;
        }

        StringArgument& Positional() {
            _positional = true;

            return *this;
        }

    private:
        std::optional<std::string*> _store;
        std::optional<std::vector<std::string>*> _store_multi;
        std::vector<std::string> _values;
        std::string _value;
    };

    class ArgParser::IntArgument : public Argument {
        friend ArgParser;
    public:
        IntArgument(
                char short_name,
                const std::string& full_name,
                const std::string& help,
                const std::string& type) :
                Argument(short_name, full_name, help, type) {}

        IntArgument& Default(int value) {
            _value = value;
            _default_value = true;

            return *this;
        }

        IntArgument& MultiValue(size_t min_args_count = 0) {
            _multi_value = true;
            _min_args_count = min_args_count;

            return *this;
        }

        IntArgument& Positional() {
            _positional = true;

            return *this;
        }

        IntArgument& StoreValue(int& value) {
            _store = &value;
            value = _value;

            return *this;
        }

        IntArgument& StoreValues(std::vector<int>& int_values) {
            _store_multi = &int_values;

            return *this;
        }

    private:
        std::optional<int*> _store;
        std::optional<std::vector<int>*> _store_multi;
        std::vector<int> _values;
        int _value;
    };

    class ArgParser::FlagArgument : public Argument {
        friend ArgParser;
    public:
        FlagArgument(
                char short_name,
                const std::string& full_name,
                const std::string& help,
                const std::string& type) :
                Argument(short_name, full_name, help, type) {}

        FlagArgument& Default(bool value) {
            _value = value;
            _default_value = true;

            return *this;
        }

        FlagArgument& StoreValue(bool& value) {
            _store = &value;
            value = _value;

            return *this;
        }

    private:
        bool _value = false;
        std::optional<bool*> _store;
    };
}