//
// Created by 李文禹 on 2022/1/24.
//

#ifndef OBS_STUDIO_BASE_LEXER_H
#define OBS_STUDIO_BASE_LEXER_H

#include "util/lexer.h"

class BaseLexer{
	lexer lex;
public:
	inline BaseLexer(){
		lexer_init(&lex);
	}

	inline ~BaseLexer(){
		lexer_free(&lex);
	}

	operator lexer* (){
		return &lex;
	}
};

inline bool GetToken(lexer* lex,std::string &str,base_token_type type)
{
	base_token token;
	bool succeed = lexer_getbasetoken(lex,&token,IGNORE_WHITESPACE);
	if(!succeed){
		return false;
	}

	if (token.type != type){
		return false;
	}

	str.assign(token.text.array,token.text.len);
	return true;
}

inline bool ExpectToken(lexer *lex, const char *str, base_token_type type)
{
	base_token token;
	if (!lexer_getbasetoken(lex, &token, IGNORE_WHITESPACE))
		return false;
	if (token.type != type)
		return false;

	return strref_cmp(&token.text, str) == 0;
}

#endif //OBS_STUDIO_BASE_LEXER_H
