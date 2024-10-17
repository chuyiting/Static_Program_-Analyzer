//
// Created by Eddy Chu on 2022/10/2.
//

#ifndef SPA_TESTQPSPARSER_H
#define SPA_TESTQPSPARSER_H

#define ST(x) (QueryToken((x), TokenType::StringToken))
#define IT(x) (QueryToken((x), TokenType::IntegerToken))
#define PT(x) (QueryToken((x), TokenType::PunctuatorToken))
#define WT(x) (QueryToken((x), TokenType::WildcardToken))
#define QT(x) (QueryToken((x), TokenType::QuoteToken))
#define DT(x) (QueryToken((x), TokenType::DotToken))
#define AT(x) (QueryToken((x), TokenType::AttributeToken))
#define ET(x) (QueryToken((x), TokenType::EqualToken))

#endif // SPA_TESTQPSPARSER_H
