#include "chocopy-llvm/Parser/Parser.h"
#include "chocopy-llvm/AST/ASTContext.h"
#include "chocopy-llvm/Basic/Diagnostic.h"
#include "chocopy-llvm/Sema/Scope.h"
#include "chocopy-llvm/Sema/Sema.h"


#include <llvm/ADT/APInt.h>


#include <stack>


namespace chocopy {
Parser::Parser(ASTContext &C, Lexer &Lex, Sema &Acts)
   : Diags(Lex.getDiagnostics()), Context(C), TheLexer(Lex) {}


Program *Parser::parse() {
 Program *P = parseProgram();
 return P;
}


bool Parser::consumeToken(tok::TokenKind ExpectedTok) {
 if (Tok.is(ExpectedTok)) {
   TheLexer.lex(Tok);
   return true;
 }
 return false;
}


bool Parser::consumeToken() {
 TheLexer.lex(Tok);
 return true;
}


bool Parser::expect(tok::TokenKind ExpectedTok) {
 if (Tok.is(ExpectedTok))
   return true;
 Diags.emitError(Tok.getLocation().Start, diag::err_near_token) << Tok;
 Diags.emitError(Tok.getLocation().Start, diag::err_expected)
     << tok::getTokenName(ExpectedTok);
 return false;
}


bool Parser::expectAndConsume(tok::TokenKind ExpectedTok) {
 return expect(ExpectedTok) && consumeToken();
}


void Parser::skipToNextLine() {
 while (!Tok.is(tok::eof) && !consumeToken(tok::NEWLINE))
   consumeToken();
}


void Parser::emitUnexpected() {
 Diags.emitError(Tok.getLocation().Start, diag::err_unexpected) << Tok;
}


const Token &Parser::getLookAheadToken(int N) {
 assert(N);
 return TheLexer.LookAhead(N - 1);
}


Program *Parser::parseProgram() {
  DeclList Declarations;
  StmtList Statements;
 
 
  auto IsVarDef = [this](Token &Tok) {
    return Tok.isOneOf(tok::identifier, tok::idstring) &&
           TheLexer.LookAhead(0).is(tok::colon);
  };
  auto IsFuncDef = [](Token &Tok) {
   return Tok.is(tok::kw_def);
  };
  auto IsClassDef = [](Token &Tok) {
   return Tok.is(tok::kw_class);
  };
 
 consumeToken();
 
 while (1) {
   if (IsVarDef(Tok)) {
     if (VarDef *V = parseVarDef())
       Declarations.push_back(V);
   } else if (IsFuncDef(Tok)) {
     if (FuncDef *F = parseFuncDef())
       Declarations.push_back(F);
       
       
   } else if (IsClassDef(Tok)){
     if (ClassDef *C = parseClassDef())
       Declarations.push_back(C);
   }  else
     break;
    if (Tok.is(tok::INDENT) || Tok.is(tok::DEDENT)) {
    emitUnexpected();
    skipToNextLine();
    //return nullptr;
    }
   
 }
 
  
  while (IsVarDef(Tok)) {
    if (VarDef *V = parseVarDef())
      Declarations.push_back(V);
    else
      skipToNextLine();
  }

 
  while (Tok.isNot(tok::eof)) {
    if (Stmt *S = parseStmt())
      Statements.push_back(S);
    else {
      skipToNextLine();
    }
      
  }
 
  return Context.createProgram(Declarations, Statements);
 }

Stmt *Parser::parseStmt() {
  if (Tok.is(tok::INDENT) || Tok.is(tok::DEDENT)) {
    emitUnexpected();
    consumeToken();
    return nullptr;
  
  } else if (Tok.is(tok::kw_if)) {
    return parseIfStmt();
  } else if (Tok.is(tok::kw_while)) {
    return parseWhileStmt();
  } else if (Tok.is(tok::kw_for)) {
    return parseForStmt();
  } else if (Tok.is(tok::kw_pass)) {
    consumeToken();
    return nullptr;
  } else if (Tok.is(tok::kw_return)) {
    SMRange Loc(Tok.getLocation());
    consumeToken();
    if (Tok.is(tok::NEWLINE) || Tok.is(tok::eof)) {
      consumeToken();
      return Context.createReturnStmt(Loc);
    }
    if (Expr *E = parseExpr()) {
        if(Tok.is(tok::NEWLINE)){
          consumeToken();
        }
        return Context.createReturnStmt(SMRange(Loc.Start, E->getLocation().End), E);
    
    } else {
      return nullptr;
    }
    
  } else if (Stmt *S = parseAssignOrExpr()) {
      if (auto *ExprS = dyn_cast<ExprStmt>(S)) {
        if (isa<MethodCallExpr>(ExprS->getExpr())) {
            if (expectAndConsume(tok::NEWLINE)) {
                return S;
            }
        }
    }
    if (expectAndConsume(tok::NEWLINE)) {
        return S;
    }
  }

  return nullptr;
}


Stmt *Parser::parseIfStmt() {
  SMRange ifStart = Tok.getLocation();
  if (!expectAndConsume(tok::kw_if)) {
    skipToNextLine();
    return nullptr;
  }

  Expr *ifCond = parseExpr();
  if (!ifCond) { skipToNextLine(); return nullptr; }

  if (!expectAndConsume(tok::colon)) { skipToNextLine(); return nullptr; }
  if (!expectAndConsume(tok::NEWLINE)) { skipToNextLine(); return nullptr; }
  if (!expectAndConsume(tok::INDENT)) { skipToNextLine(); return nullptr; }

  StmtList thenBody;
  while (Tok.isNot(tok::DEDENT) && Tok.isNot(tok::eof)) {
    if (Stmt *S = parseStmt()) thenBody.push_back(S);
    else skipToNextLine();
  }

  SMRange thenEnd = Tok.getLocation();
  consumeToken();

  struct ElifBlock {
    SMRange loc;
    Expr *cond;
    StmtList body;
  };
  std::vector<ElifBlock> elifs;
  while (Tok.is(tok::kw_elif)) {
    SMRange elifStart = Tok.getLocation();
    consumeToken(); // 'elif'

    Expr *elifCond = parseExpr();
    if (!elifCond) { skipToNextLine(); return nullptr; }

    if (!expectAndConsume(tok::colon)) { skipToNextLine(); return nullptr; }
    if (!expectAndConsume(tok::NEWLINE)) { skipToNextLine(); return nullptr; }
    if (!expectAndConsume(tok::INDENT)) { skipToNextLine(); return nullptr; }

    StmtList elifBody;
    while (Tok.isNot(tok::DEDENT) && Tok.isNot(tok::eof)) {
      if (Stmt *S = parseStmt()) elifBody.push_back(S);
      else skipToNextLine();
    }
    SMRange elifEnd = Tok.getLocation();
    consumeToken();

    elifs.push_back({ SMRange(elifStart.Start, elifEnd.End),
                      elifCond,
                      std::move(elifBody) });
  }

  StmtList elseBody;
  SMRange elseEnd = thenEnd;
  bool containsElse = false;
  if (Tok.is(tok::kw_else)) {
    containsElse = true;
    consumeToken();
    if (!expectAndConsume(tok::colon)) { skipToNextLine(); return nullptr; }
    if (!expectAndConsume(tok::NEWLINE)) { skipToNextLine(); return nullptr; }
    if (!expectAndConsume(tok::INDENT)) { skipToNextLine(); return nullptr; }

    while (Tok.isNot(tok::DEDENT) && Tok.isNot(tok::eof)) {
      if (Stmt *S = parseStmt()) elseBody.push_back(S);
      else skipToNextLine();
    }
    elseEnd = Tok.getLocation();
    consumeToken();
  }

StmtList currentElse = std::move(elseBody);

for (int i = int(elifs.size()) - 1; i >= 0; --i) {
  auto &E = elifs[i];
  SMRange nestedLoc(
    E.loc.Start,
    !containsElse
      ? elifs.back().loc.End
      : elseEnd.End
  );
  Stmt *nestedIf = Context.createIfStmt(nestedLoc, E.cond, E.body, currentElse);

  currentElse.clear();
  currentElse.push_back(nestedIf);
}


  SMRange fullLoc(ifStart.Start, 
                  (elifs.empty() 
                     ? elseEnd.End 
                     : (containsElse 
                         ? elseEnd.End 
                         : elifs.back().loc.End)));

  return Context.createIfStmt(fullLoc, ifCond, thenBody, currentElse);
}



Stmt* Parser::parseWhileStmt() {
  SMRange startLoc = Tok.getLocation();
  
  if (!expectAndConsume(tok::kw_while)) {
      skipToNextLine();
      return nullptr;
  }

  Expr* Condition = parseExpr();
  if (!Condition) {
      skipToNextLine();
      return nullptr;
  }

  if (!expectAndConsume(tok::colon)) {
    skipToNextLine();
    return nullptr;
  }

  if (!expectAndConsume (tok::NEWLINE)) {
    skipToNextLine();
    return nullptr;
  }

  if(!expectAndConsume(tok::INDENT)) {
    skipToNextLine();
    return nullptr;
  }

  StmtList Body;
  while (Tok.isNot(tok::DEDENT) && Tok.isNot(tok::eof)) {
      if (Stmt* S = parseStmt()) Body.push_back(S);
      else skipToNextLine();
  }
  SMRange lastLoc = Tok.getLocation();
  consumeToken();

  SMRange Loc(startLoc.Start, lastLoc.End);
  
  return Context.createWhileStmt(Loc, Condition, Body);
}

Stmt* Parser::parseForStmt() {
  SMRange startLoc = Tok.getLocation();

  if (!expectAndConsume(tok::kw_for)) {
      skipToNextLine();
      return nullptr;
  }

  if (!expect(tok::identifier)) {
      skipToNextLine();
      return nullptr;
  }
  DeclRef* decl = Context.createDeclRef(Tok.getLocation(), Tok.getSymbolInfo());
  consumeToken();

  if (!expectAndConsume(tok::kw_in)) {
      skipToNextLine();
      return nullptr;
  }

  Expr* Iterable = parseExpr();
  if (!Iterable) {
      skipToNextLine();
      return nullptr;
  }

  if (!expectAndConsume(tok::colon)) {
    skipToNextLine();
    return nullptr;
  }

  if (!expectAndConsume (tok::NEWLINE)) {
    skipToNextLine();
    return nullptr;
  }

  if(!expectAndConsume(tok::INDENT)) {
    skipToNextLine();
    return nullptr;
  }

  StmtList Body;

  while (Tok.isNot(tok::DEDENT) && Tok.isNot(tok::eof)) {
      if (Stmt* cur = parseStmt()) {
        Body.push_back(cur);
      }
      else skipToNextLine();
  }

  SMRange lastPos = Tok.getLocation();
  consumeToken();

  SMRange Loc(startLoc.Start, lastPos.End);
  
  return Context.createForStmt(Loc, decl, Iterable, Body);
}



Stmt *Parser::parseAssignOrExpr() {
  auto IsTarget = [](const Expr *E) {
    return llvm::isa<DeclRef>(E) || 
           llvm::isa<IndexExpr>(E) || 
           llvm::isa<MemberExpr>(E);
  };

  

 SMLoc SLoc = Tok.getLocation().Start;
 ExprList Targets;
 Expr *E = nullptr;
 do {
   if (E)
     Targets.push_back(E);


   E = parseExpr();
   if (!E)
     return nullptr;
 } while (IsTarget(E) && consumeToken(tok::equal));


 if (!expect(tok::NEWLINE))
   return nullptr;


 SMLoc ELoc = Tok.getLocation().Start;
 SMRange Loc(SLoc, ELoc);
 if (!Targets.empty())
   return Context.createAssignStmt(Loc, Targets, E);


 return Context.createExprStmt(Loc, E);
}

Expr *Parser::parseExpr() {
 return parseTernaryExpr();
}

// Тернарный оператор
Expr *Parser::parseTernaryExpr() {

  Expr *left = parseLogicalOr();
  if (!left) return nullptr;
  if (consumeToken(tok::kw_if)) {
      Expr *cond = parseLogicalOr();
      if (!cond) { skipToNextLine(); return nullptr; } 

      if (!expectAndConsume(tok::kw_else)) {
          skipToNextLine();
          return nullptr;
      }

      // парсим правую часть через рекурсию
      Expr *right = parseTernaryExpr();
      if (!right) return nullptr;

      SMRange Loc(
          left->getLocation().Start, 
          right->getLocation().End
      );

      return Context.createIfExpr(Loc, cond, left, right);
  }

  // если if не найден, возвращаем просто левую часть
  return left;
}

// логическое ИЛИ
Expr *Parser::parseLogicalOr() {
  Expr *L = parseLogicalAnd();
  if (!L) return nullptr;
  while (Tok.is(tok::kw_or)) {
      //SMRange loc = Tok.getLocation();
      consumeToken();
      Expr *R = parseLogicalAnd();
      if (!R) return nullptr;
      SMRange exprRange(L->getLocation().Start, R->getLocation().End);
      L = Context.createBinaryExpr(exprRange, L, BinaryExpr::OpKind::Or, R);
  }
  return L;
}

// логическое И
Expr *Parser::parseLogicalAnd() {
  Expr *L = parseLogicalNot();
  if (!L) return nullptr; 
  while (Tok.is(tok::kw_and)) {
      //SMRange loc = Tok.getLocation();
      consumeToken();
      Expr *R = parseLogicalNot();
      if (!R) return nullptr; 
      SMRange exprRange(L->getLocation().Start, R->getLocation().End);
      L = Context.createBinaryExpr(exprRange, L, BinaryExpr::OpKind::And, R);
  }
  return L;
}

// логическое отрицание
Expr *Parser::parseLogicalNot() {
  if (Tok.is(tok::kw_not)) {
      SMRange loc = Tok.getLocation();
      consumeToken();
      Expr *expr = parseComparison();
      if (!expr) return nullptr; 
      loc.End = expr->getLocation().End;
      return Context.createUnaryExpr(loc, UnaryExpr::OpKind::Not, expr);
  }
  return parseComparison();
}

// операторы сравнения
Expr *Parser::parseComparison() {
  Expr *L = parseBinaryAddOrSub();
  if (!L) return nullptr; 
  auto IsComparisonOp = [](tok::TokenKind kind) {
      return kind == tok::equalequal || kind == tok::exclaimequal ||
             kind == tok::less || kind == tok::lessequal ||
             kind == tok::greater || kind == tok::greaterequal ||
             kind == tok::kw_is;
  };

  if (IsComparisonOp(Tok.getKind())) {
      SMRange loc = L->getLocation();
      BinaryExpr::OpKind op;
      switch (Tok.getKind()) {
          case tok::equalequal:    op = BinaryExpr::OpKind::EqCmp; break;
          case tok::exclaimequal:      op = BinaryExpr::OpKind::NEqCmp; break;
          case tok::less:           op = BinaryExpr::OpKind::LCmp; break;
          case tok::lessequal:     op = BinaryExpr::OpKind::LEqCmp; break;
          case tok::greater:        op = BinaryExpr::OpKind::GCmp; break;
          case tok::greaterequal:  op = BinaryExpr::OpKind::GEqCmp; break;
          case tok::kw_is:          op = BinaryExpr::OpKind::Is; break;
          default: llvm_unreachable("Not a comparison operator");
      }
      consumeToken();
      Expr *R = parseBinaryAddOrSub();
      if (!R) return nullptr; 
      loc.End = R->getLocation().End;
      return Context.createBinaryExpr(loc, L, op, R);
  }
  return L;
}

Expr *Parser::parseBinaryAddOrSub() {
 SMRange Loc = Tok.getLocation();
 Expr *L = parseBinaryMulOrDivOrMod();
 if (!L)
   return nullptr;


 auto IsContinue = [](const Token &T) {
   return T.isOneOf(tok::plus, tok::minus);
 };


 while (IsContinue(Tok)) {
   //SMRange Loc = Tok.getLocation();
   BinaryExpr::OpKind K =
       Tok.is(tok::plus) ? BinaryExpr::OpKind::Add : BinaryExpr::OpKind::Sub;
   consumeToken();

   
   Expr *R = parseBinaryMulOrDivOrMod();
   if (!R)
     return nullptr;
   //auto endLoc = Tok.getLocation().End;
   SMRange exprRange(Loc.Start, R->getLocation().End);
   L = Context.createBinaryExpr(exprRange, L, K, R);
 }
 return L;
}


Expr *Parser::parseBinaryMulOrDivOrMod() {
 SMRange Loc = Tok.getLocation();
 Expr *L = parseUnaryExpr();
 if (!L)
   return nullptr;


 auto IsContinue = [](const Token &T) {
   return T.isOneOf(tok::star, tok::slashslash, tok::percent);
 };


 while (IsContinue(Tok)) {
   BinaryExpr::OpKind K = BinaryExpr::OpKind::Mul;
   if (Tok.is(tok::slashslash))
     K = BinaryExpr::OpKind::FloorDiv;
   else if (Tok.is(tok::percent))
     K = BinaryExpr::OpKind::Mod;
   consumeToken();

   
   Expr *R = parseUnaryExpr();
   if (!R)
     return nullptr;
   //auto endLoc = Tok.getLocation().End;
   //SMRange exprRange(Loc.Start, endLoc);
   SMRange exprRange(Loc.Start, R->getLocation().End);
   L = Context.createBinaryExpr(exprRange, L, K, R);
 }
 return L;
}

//парсит унарный минус
Expr *Parser::parseUnaryExpr() {
  if (Tok.is(tok::minus)) {
    SMRange Loc = Tok.getLocation();
    UnaryExpr::OpKind Kind = UnaryExpr::OpKind::Minus;
    consumeToken();
    Expr *Sub = parseUnaryExpr();
    if (!Sub) return nullptr;
    SMRange exprRange(Loc.Start, Tok.getLocation().End);
    return Context.createUnaryExpr(exprRange, Kind, Sub);
  }
  return parseMemberAccess();
}

bool Parser::parseArgs(ExprList &Args) {
  do {
      Expr *A = parseExpr();
      if (!A) return false;
      Args.push_back(A);
  } while (consumeToken(tok::comma));
  return true;
}

Expr *Parser::parseFnCallDemo() {
  return parseAtomicExpr();
}

Expr *Parser::parseMemberAccess() {
  SMRange startLoc = Tok.getLocation();
  Expr *E = parseAtomicExpr();
  if (!E)
    return nullptr;

  while (Tok.isOneOf(tok::period, tok::l_square, tok::l_paren)) {
    // обрабатываем цепочку .member или [index]
    while (Tok.isOneOf(tok::period, tok::l_square)) {
      if (Tok.is(tok::period)) {
        // .member
        consumeToken(); // '.'
        if (!expect(tok::identifier))
          return nullptr;

        // создаём DeclRef для имени поля/метода
        DeclRef *M = Context.createDeclRef(
          Tok.getLocation(),
          Tok.getSymbolInfo()
        );

        SMRange memberLoc(E->getLocation().Start, Tok.getLocation().End);
        consumeToken(); // имя
        E = Context.createMemberExpr(memberLoc, E, M);

      } else {
        // [index]
        consumeToken(); // '['
        Expr *idx = parseExpr();
        if (!idx) return nullptr;
        SMLoc rsqStart = Tok.getLocation().End;
        if (!expectAndConsume(tok::r_square))
          return nullptr;
        SMRange idxLoc(E->getLocation().Start, rsqStart);
        E = Context.createIndexExpr(idxLoc, E, idx);
      }
    }

    // обрабатываем всевозможные вызовы
    while (Tok.is(tok::l_paren)) {
      consumeToken(); // '('

      ExprList args;
      if (Tok.isNot(tok::r_paren)) {
        do {
          Expr *A = parseExpr();
          if (!A) return nullptr;
          args.push_back(A);
        } while (consumeToken(tok::comma));
      }

      if (!expectAndConsume(tok::r_paren))
        return nullptr;


      SMRange callLoc(startLoc.Start, Tok.getLocation().Start);
      // различаем свободный вызов и метод
      if (auto *ME = llvm::dyn_cast<MemberExpr>(E)) {
        E = Context.createMethodCallExpr(callLoc, ME, args);
      } else if (auto *DR = llvm::dyn_cast<DeclRef>(E)) {
        E = Context.createCallExpr(callLoc, DR, args);
      } else {
        Diags.emitError(Tok.getLocation().Start,
                        diag::err_unexpected)
            << "call on non-callable expression";
        return nullptr;
      }
    }
  }

  return E;
}


//парсер атомарных (базовых) элементов выражений
Expr *Parser::parseAtomicExpr() {
 SMRange Loc = Tok.getLocation();


 if (Tok.is(tok::identifier)) {
   Expr *E = Context.createDeclRef(Loc, Tok.getSymbolInfo());
   consumeToken();
   return E;
 }


 if (Tok.is(tok::integer_literal)) {
   llvm::APInt Val(32, Tok.getLiteralData(), 10);
   Expr *E = Context.createIntegerLiteral(Loc, Val.getSExtValue());
   consumeToken();
   return E;
 }


 if (Tok.is(tok::kw_True)) {
   Expr *E = Context.createBooleanLiteral(Loc, true);
   consumeToken();
   return E;
 }
 if (Tok.is(tok::kw_False)) {
   Expr *E = Context.createBooleanLiteral(Loc, false);
   consumeToken();
   return E;
 }


 if (Tok.is(tok::kw_None)) {
   consumeToken();
   return Context.createNoneLiteral(Loc);
 }


 if (Tok.isOneOf(tok::idstring, tok::string_literal)) {
   StringRef Str = Tok.getLiteralData();
   consumeToken();
   return Context.createStringLiteral(Loc, Str);
 }

  // обработка списков
  if (Tok.is(tok::l_square)) {
    consumeToken(); // '['
    ExprList Elements;

    // Парсим элементы списка
    while (Tok.isNot(tok::r_square) && !Tok.is(tok::eof)) {
        if (Expr *E = parseExpr()) {
            Elements.push_back(E);
            if (!consumeToken(tok::comma)) {
                break;
            }
        } else {
            skipToNextLine();
            return nullptr;
        }
    }

    SMRange endLoc = Tok.getLocation();
    if (!expectAndConsume(tok::r_square)) {
        Diags.emitError(Tok.getLocation().Start, diag::err_expected) << "']'";
        return nullptr;
    }

    Loc = SMRange(Loc.Start, endLoc.End);
    return Context.createListExpr(Loc, Elements);
}

 if (Tok.is(tok::l_paren)) {
   consumeToken();
   Expr *E = parseExpr();
   if (!E || !expectAndConsume(tok::r_paren))
     return nullptr;
   return E;
 }


 emitUnexpected();
 return nullptr;
}


// type = ID | IDSTRING | '[' type ']'
TypeAnnotation *Parser::parseType() {
 SMRange Loc = Tok.getLocation();
 switch (Tok.getKind()) {
 case tok::identifier: {
   StringRef Name = Tok.getSymbolInfo()->getName();
   consumeToken();
   return Context.createClassType(Loc, Name);
 }
 case tok::idstring: {
   SMRange Loc = Tok.getLocation();
   StringRef Name = Tok.getLiteralData();
   consumeToken();
   return Context.createClassType(Loc, Name);
 }
 case tok::l_square: {
   consumeToken();
   if (TypeAnnotation *T = parseType()) {
     if (Tok.is(tok::r_square)) {
       consumeToken();
       Loc = SMRange(Loc.Start, Tok.getLocation().End);
       //consumeToken();
       return Context.createListType(Loc, T);
     }
   }
   return nullptr;
 }
 default:
   return nullptr;
 }
}

FuncDef *Parser::parseFuncDef() {
  SMRange startLoc = Tok.getLocation();

  if (!expectAndConsume(tok::kw_def)) {
    skipToNextLine();
    return nullptr;
  }

  if (!expect(tok::identifier)) {
    emitUnexpected();
    skipToNextLine();
    return nullptr;
  }

  SymbolInfo *funcName = Tok.getSymbolInfo();
  SMRange nameLoc = Tok.getLocation();
  consumeToken(); // имя функции

  if (!expectAndConsume(tok::l_paren)) {
    skipToNextLine();
    return nullptr;
  }

  ParamDeclList Params;

  if (Tok.isNot(tok::r_paren)) {
    while (true) {

      if (!expect(tok::identifier)) {
        skipToNextLine();
        return nullptr;
      }

      SymbolInfo *paramName = Tok.getSymbolInfo();
      SMRange paramLoc = Tok.getLocation();
      consumeToken();

      if (!expectAndConsume(tok::colon)) {
        skipToNextLine();
        return nullptr;
      }

      TypeAnnotation *type = nullptr;
      bool idStringFlag = false;
      if (Tok.is(tok::idstring)) {
          idStringFlag = true;  
          type = Context.createClassType(Tok.getLocation(), Tok.getLiteralData());
          consumeToken();
      } else {
          type = parseType();
      }
      if (!type) {
        skipToNextLine();
        return nullptr;
      }

      Identifier *paramIdent = Context.createIdentifier(paramLoc, paramName);
      SMRange paramFullLoc = Tok.getLocation();
      if (idStringFlag){
        paramFullLoc = SMRange(paramLoc.Start, Tok.getLocation().Start);
      } else {
        paramFullLoc = SMRange(paramLoc.Start, Tok.getLocation().Start);

      }
      
      ParamDecl *paramDecl = Context.createParamDecl(paramFullLoc, paramIdent, type);
      Params.push_back(paramDecl);

      if (!consumeToken(tok::comma))
        break;
    }
  }
  SMRange noneLoc(Tok.getLocation().End, Tok.getLocation().End);
  if (!expectAndConsume(tok::r_paren)) {
    skipToNextLine();
    return nullptr;
  }
  
  TypeAnnotation *returnType = Context.createClassType(noneLoc, "<None>");
  if(Tok.is(tok::arrow)) {
    consumeToken();
    returnType = parseType();
    if (!returnType) {
      skipToNextLine();
      return nullptr;
    }
  }
  
  if (!expectAndConsume(tok::colon)) {
    skipToNextLine();
    return nullptr;
  }

  if (!expectAndConsume(tok::NEWLINE)) {
    skipToNextLine();
    return nullptr;
  }

  if (!expectAndConsume(tok::INDENT)) {
    skipToNextLine();
    return nullptr;
  }

  DeclList Decls;
  StmtList Stmts;
  SMRange endLoc = startLoc;
    // Парсим тело функции
    if (Tok.is(tok::INDENT)) {
      emitUnexpected();
      skipToNextLine();
    }
    while (Tok.isNot(tok::DEDENT) && Tok.isNot(tok::eof)) {
      if (Tok.is(tok::identifier) && getLookAheadToken(1).is(tok::colon)) {
          if (VarDef *V = parseVarDef()) {
              Decls.push_back(V);
              endLoc = V->getLocation();
          } 
      } else if (Tok.is(tok::kw_global)){
          SMRange globalLoc = Tok.getLocation();
          consumeToken();
          Identifier *V = Context.createIdentifier(Tok.getLocation(), Tok.getSymbolInfo());
          SMRange declLoc(globalLoc.Start, Tok.getLocation().End);
          GlobalDecl *G = Context.createGlobalDecl(declLoc, V);
          Decls.push_back(G);
          endLoc = G->getLocation();
          skipToNextLine();
      
      } else if (Tok.is(tok::kw_nonlocal)) {
          SMRange nonLocalLoc = Tok.getLocation();
          consumeToken();
          Identifier *V = Context.createIdentifier(Tok.getLocation(), Tok.getSymbolInfo());
          SMRange declLoc(nonLocalLoc.Start, Tok.getLocation().End);
          NonLocalDecl *N = Context.createNonLocalDecl(declLoc, V);
          Decls.push_back(N);
          endLoc = N->getLocation();
          skipToNextLine();
      } else if (Tok.is(tok::kw_def)) {
        if (FuncDef *F = parseFuncDef()) {
          Decls.push_back(F);
          endLoc = F->getLocation();
        }
      } else if (Stmt *S = parseStmt()) {
          Stmts.push_back(S);
          endLoc = S->getLocation();
      } else {
          skipToNextLine();
      }
  }

  if (Tok.is(tok::DEDENT)) {
      //endLoc = Tok.getLocation();
      consumeToken(tok::DEDENT);
  }

  SMRange fullLoc(startLoc.Start, endLoc.End);

  Identifier *funcIdent = Context.createIdentifier(nameLoc, funcName);

  return Context.createFuncDef(fullLoc, funcIdent, Params, returnType, Decls, Stmts);
}


ClassDef *Parser::parseClassDef() {
  SMRange startLoc = Tok.getLocation();
  if (!expectAndConsume(tok::kw_class)) {
    skipToNextLine();
    return nullptr;
  }

  if (!expect(tok::identifier)) {
    skipToNextLine();
    return nullptr;
  }
  SymbolInfo *nameSym = Tok.getSymbolInfo();
  SMRange nameLoc = Tok.getLocation();
  consumeToken();
  Identifier *nameIdent = Context.createIdentifier(nameLoc, nameSym);

  Identifier *superIdent = nullptr;
  if (consumeToken(tok::l_paren)) {
    if (!expect(tok::identifier)) {
      skipToNextLine();
      return nullptr;
    }
    SymbolInfo *superSym = Tok.getSymbolInfo();
    SMRange superLoc = Tok.getLocation();
    consumeToken();
    superIdent = Context.createIdentifier(superLoc, superSym);

    if (!expectAndConsume(tok::r_paren)) {
      skipToNextLine();
      return nullptr;
    }
  }

  if (!expectAndConsume(tok::colon)) {
    skipToNextLine();
    return nullptr;
  }
  if (!expectAndConsume(tok::NEWLINE)) {
    skipToNextLine();
    return nullptr;
  }
  if (!expectAndConsume(tok::INDENT)) {
    skipToNextLine();
    return nullptr;
  }

  DeclList members;
  SMRange lastLoc = startLoc;
  while (Tok.isNot(tok::DEDENT) && Tok.isNot(tok::eof)) {
    if (Tok.is(tok::identifier) && getLookAheadToken(1).is(tok::colon)) {
      if (VarDef *V = parseVarDef()) {
        members.push_back(V);
        lastLoc = V->getLocation();
        continue;
      }
    }
    if (Tok.is(tok::kw_def)) {
      if (FuncDef *F = parseFuncDef()) {
        members.push_back(F);
        lastLoc = F->getLocation();
        continue;
      }
    }
    skipToNextLine();
  }

  //SMRange dedentLoc = Tok.getLocation();
  consumeToken(tok::DEDENT);

  SMRange fullLoc(startLoc.Start, lastLoc.End);
  return Context.createClassDef(fullLoc,
                                nameIdent,
                                superIdent,
                                members);
}


VarDef *Parser::parseVarDef() {


 if (!expect(tok::identifier)) {
  skipToNextLine();
  return nullptr;
 }
   


 SymbolInfo *Name = Tok.getSymbolInfo();
 SMRange NameLoc = Tok.getLocation();
 consumeToken();


 if (!expectAndConsume(tok::colon)) {
  skipToNextLine();
  return nullptr;
 }
   


 TypeAnnotation *T = parseType();
 if (!expectAndConsume(tok::equal)) {
  skipToNextLine();
  return nullptr;
 }


 if (Literal *L = parseLiteral()) {
   if (expectAndConsume(tok::NEWLINE)) {
     SMLoc ELoc = L->getLocation().End;
     SMRange Loc(NameLoc.Start, ELoc);
     Identifier *V = Context.createIdentifier(NameLoc, Name);
     return Context.createVarDef(Loc, V, T, L);
   }
 }


 return nullptr;
}


Literal *Parser::parseLiteral() {
 SMRange Loc = Tok.getLocation();


 if (consumeToken(tok::kw_None)) {
   return Context.createNoneLiteral(Loc);
 } else if (consumeToken(tok::kw_True)) {
   return Context.createBooleanLiteral(Loc, true);
 } else if (consumeToken(tok::kw_False)) {
   return Context.createBooleanLiteral(Loc, false);
 } else if (Tok.is(tok::integer_literal)) {
   llvm::APInt Value(32, Tok.getLiteralData(), 10);
   consumeToken();
   return Context.createIntegerLiteral(Loc, Value.getSExtValue());
 } else if (Tok.isOneOf(tok::idstring, tok::string_literal)) {
   StringRef Str = Tok.getLiteralData();
   consumeToken();
   return Context.createStringLiteral(Loc, Str);
 } 


 Diags.emitError(Tok.getLocation().Start, diag::err_near_token) << Tok;
 return nullptr;
}
} // namespace chocopy