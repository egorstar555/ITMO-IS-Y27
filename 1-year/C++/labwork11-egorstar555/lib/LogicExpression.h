#pragma once

#include <stack>
#include <unordered_map>
#include <set>
#include <sstream>
#include <algorithm>
#include <vector>

class LogicExpression {
public:
    std::vector<std::string> ParseExpression(const std::string& query) {
        std::vector<std::string> args;
        std::string word;
        std::istringstream iss(query);

        while (iss >> word) {
            args.push_back(word);
        }

        std::vector<std::string> parsed_expression;

        for (size_t i = 0; i < args.size(); ++i) {
            if (args[i].empty()) {
                continue;
            }
            size_t begin_pos = AddOpeningBracket(parsed_expression, args[i]);
            AddOherPart(parsed_expression, args[i], 0, begin_pos);
        }
        GetExpression(parsed_expression);
        return expression;
    }

    std::set<std::string>& GetKeyWords() {
        return key_words;
    }

    static bool IsValidSyntax(const std::string& query) {
        std::string word;
        bool isOperator = false;
        bool isOneTerm = true;
        size_t isBracketCorrect = 0;

        for (char ch: query) {
            if (ch == '(') {
                ++isBracketCorrect;
            } else if (ch == ')') {
                if (isBracketCorrect <= 0) {
                    return false;
                }
                --isBracketCorrect;
            } else if (std::isspace(ch)) {
                isOneTerm = false;
                if (!word.empty()) {
                    if (word == "AND" || word == "OR") {
                        if (isOperator) {
                            return false;
                        }
                        isOperator = true;
                    } else {
                        isOperator = false;
                    }
                    word.clear();
                }
            } else {
                word += ch;
            }
        }

        return (!word.empty() && (word != "AND" || word != "OR") && (isOperator || isOneTerm) && isBracketCorrect == 0);
    }

private:
    void GetExpression(std::vector<std::string>& parsed_expression) {
        std::stack<std::string> operators;
        for (auto& sumbol: parsed_expression) {
            if (sumbol == "(") {
                operators.push(sumbol);
            } else if (sumbol == ")") {
                if (operators.empty()) {
                    throw std::runtime_error("Invalid expression\n");
                }

                while (operators.top() != "(") {
                    expression.push_back(operators.top());
                    operators.pop();
                }
                operators.pop();
            } else if (!priority_of_operations.contains(sumbol)) {
                std::transform(sumbol.begin(), sumbol.end(), sumbol.begin(), ::tolower);
                expression.push_back(sumbol);
                key_words.insert(sumbol);
            } else {
                while (!operators.empty() && operators.top() != "(" &&
                       priority_of_operations[operators.top()] < priority_of_operations[sumbol]) {
                    expression.push_back(operators.top());
                    operators.pop();
                }
                operators.push(sumbol);
            }
        }
        while (!operators.empty()) {
            expression.push_back(operators.top());
            operators.pop();
        }
    }

    size_t AddOpeningBracket(std::vector<std::string>& res_args, std::string& args) {
        size_t i = 0;
        while (args[i] == '(') {
            res_args.emplace_back("(");
            ++i;
        }
        return i;
    }

    void
    AddOherPart(std::vector<std::string>& res_args, std::string& arg, size_t closing_bracket_count, size_t& begin_pos) {
        if (arg.size() - 1 - closing_bracket_count >= 0 && arg[arg.size() - 1 - closing_bracket_count] == ')') {
            AddOherPart(res_args, arg, ++closing_bracket_count, begin_pos);
            res_args.emplace_back(")");
        } else {
            if (!arg.substr(begin_pos, arg.size() - closing_bracket_count - begin_pos).empty()) {
                res_args.emplace_back(arg.substr(begin_pos, arg.size() - closing_bracket_count - begin_pos));
            }
        }
    }

    std::unordered_map<std::string, size_t> priority_of_operations = {{"AND", 0},
                                                                      {"OR",  1}};
    std::vector<std::string> expression;
    std::set<std::string> key_words;
};