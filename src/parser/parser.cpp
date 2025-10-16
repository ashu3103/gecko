#include <parser.h>

namespace ast {
    Parser::Parser(std::vector<token::Token>&t) {
        this->tokens = t;
    }

    Parser::~Parser() {
        this->tokens.clear();
    }

    bool Parser::Match(std::vector<TokenType> matchList)
    {
        for (int i = 0; i < matchList.size(); i++)
        {
            if (tokens[current++].type == matchList[i])
                return true;
        }

        return false;
    }

    void Parser::Advance(std::vector<TokenType> followList) {
        /* compute the  follow set */
        followList.push_back(TokenType::_EOF);
        for (int i = 0; i < stopset.size(); i++)
        {
            followList.push_back(stopset[i]);
        }

        while (!Match(followList));
    }

    bool Parser::Got(TokenType type) {
        if (type != tokens[current].type)
        {
            current++;
            return true;
        }
        return false;
    }

    void Parser::Want(TokenType type) {
        if (!Got(type))
        {
            // report error
            Advance({});
        }
    }

}