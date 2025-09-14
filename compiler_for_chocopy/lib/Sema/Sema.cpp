#include "chocopy-llvm/Sema/Sema.h"
#include "chocopy-llvm/AST/ASTContext.h"
#include "chocopy-llvm/AST/RecursiveASTVisitor.h"
#include "chocopy-llvm/AST/Type.h"
#include "chocopy-llvm/Analysis/CFG.h"
#include "chocopy-llvm/Basic/Diagnostic.h"
#include "chocopy-llvm/Lexer/Lexer.h"
#include "chocopy-llvm/Sema/Scope.h"

#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/TypeSwitch.h>

namespace chocopy {
static raw_ostream &operator<<(raw_ostream &Stream, const Type &T) {
  llvm::TypeSwitch<const Type *>(&T)
      .Case([&Stream](const FuncType *FT) {
        Stream << "(";
        for (ValueType *T : FT->getParametersTypes()) {
          Stream << T;
          if (T != FT->getParametersTypes().back())
            Stream << ", ";
        }
        Stream << ") -> " << FT->getReturnType();
      })
      .Case([&Stream](const ClassValueType *CVT) {
        Stream << CVT->getClassName();
      })
      .Case([&Stream](const ListValueType *LVT) {
        Stream << "[";
        Stream << *LVT->getElementType();
        Stream << "]";
      });
  return Stream;
}

static InFlightDiagnostic &&operator<<(InFlightDiagnostic &&D, const Type &T) {
  std::string Err;
  llvm::raw_string_ostream(Err) << T;
  D << Err;
  return std::move(D);
}

class Sema::Analysis : public RecursiveASTVisitor<Analysis> {
  using Base = RecursiveASTVisitor<Analysis>;

  class SemaScope {
  public:
    SemaScope() = delete;
    SemaScope(const SemaScope &) = delete;
    SemaScope(SemaScope &&) = delete;
    SemaScope &operator=(const SemaScope &) = delete;
    SemaScope &operator=(SemaScope &&) = delete;

    SemaScope(Analysis *Self, Scope::ScopeKind Kind = Scope::ScopeKind::Global)
        : Self(Self) {
      std::shared_ptr<Scope> NewScope =
          std::make_shared<Scope>(Self->Actions.getCurScope(), Kind);
      Self->Actions.setCurScope(NewScope);
    }

    ~SemaScope() {
      std::shared_ptr<Scope> S = Self->Actions.getCurScope();
      Self->Actions.actOnPopScope(S.get());
      Self->Actions.setCurScope(S->getParent());
    }

  private:
    Analysis *Self;
    
  };

public:
  Analysis(Sema &Actions)
      : Actions(Actions), Diags(Actions.getDiagnosticEngine()) {}

  bool traverseProgram(Program *P) {
    SemaScope ScopeGuard(this);
    Actions.setGlobalScope(Actions.getCurScope());
    Actions.initializeGlobalScope();
    for (Declaration *D : P->getDeclarations())
      handleDeclaration(D);
    return Base::traverseProgram(P);
  }

  bool traverseClassDef(ClassDef *C) {
    
    Actions.handleDeclaration(C);
    //bool Super = false;
    // Проверка типа родительского класса
    if (!Actions.checkSuperClass(C)) {
      return true;
    }
    SemaScope ScopeGuard(this, Scope::ScopeKind::Class);
    
    
    
    // Обход всех атрибутов и методов
    for (Declaration *D : C->getDeclarations()) {
      handleDeclaration(D);
      if (FuncDef *FD = dyn_cast<FuncDef>(D)) {
        if(!Actions.checkFirstMethodParam(C, FD)) {
          Diags.emitError(FD->getNameId()->getLocation().Start, diag::err_first_method_param)
              << FD->getName();
        }
      }
        
      Base::traverseDeclaration(D);
    }

    // Проверки: атрибуты и override
    Actions.checkClassAttrs(C);
    //Actions.checkClassDef(C);
    
    return true;
  }

  bool traverseFuncDef(FuncDef *F) {
    SemaScope ScopeGuard(this, Scope::ScopeKind::Func);
    Actions.handleDeclaration(F);
    //Actions.actOnFuncDef(F);
    bool isRetWrong = false;
    if(!Actions.checkTypeAnnotation(cast<ClassType>(F->getReturnType()))) {
      isRetWrong = true;
    }
    //добавляем параметры в область видимости
    for (ParamDecl *P : F->getParams()) {
      handleDeclaration(P);
      Actions.checkTypeAnnotation(cast<ClassType>(P->getType()));
      //Actions.actOnDeclRef(P->getVariable());
    }
    
    

    //обрабатываем вложенные объявления
    for (Declaration *D : F->getDeclarations()) {
        handleDeclaration(D);
        Base::traverseDeclaration(D);
    }

    //обрабатываем операторы
    for (Stmt *S : F->getStatements()) {
      if (ReturnStmt *retS = dyn_cast<ReturnStmt>(S)) {
        if (isRetWrong) continue;
        ValueType *retType = Actions.Ctx.getNoneTy();
        if (retS->getValue()) {
          Base::traverseExpr(retS->getValue());
          retType = dyn_cast<ValueType>(retS->getValue()->getInferredType());
        }
        
        //auto r = dyn_cast<Type>(F->getReturnType());
        ValueType *reqRetType = Actions.Ctx.convertAnnotationToVType(F->getReturnType());
        if (retType != reqRetType) {
              Diags.emitError(S->getLocation().Start, diag::err_tc_assign)
              << *reqRetType << *retType;
        }
      } else {
        Base::traverseStmt(S);
      }
        
    }

    // проверяем наличие возврата
    Actions.checkReturnMissing(F);
    //Actions.setCurrentFunction(nullptr);
    return true;
    //return Base::traverseFuncDef(F);
  }

  bool traverseVarDef(VarDef *V) {
    Base::traverseLiteral(V->getValue());
    Actions.actOnVarDef(V);
    return true;
  }

  bool traverseAssignStmt(AssignStmt *A) {
      Base::traverseExpr(A->getValue());
      
      auto targets = A->getTargets();
      int targetsLen = targets.size();
      std::vector<bool> flags(targetsLen);
      int count = 0;
      for (auto it = targets.begin(); it != targets.end(); ++it) {
          Expr *T = *it;

          Base::traverseExpr(T);

          // проверяем, что цель допустима для присваивания
          bool isValidTarget = Actions.checkAssignTarget(T);
          flags[count] = isValidTarget;
          // только для допустимых целей проверяем типы
          /*
          if (isValidTarget) {
              Actions.checkAssignment(T, A->getValue());
          }
          */
          ++count;
      }
      int i = 0;
      for (auto it = targets.begin(); it != targets.end(); ++it) {
        if(flags[i++]) {
          Expr *T = *it;
          Actions.checkAssignment(T, A->getValue());
        }
      }
      return true;
  }
  bool traverseReturnStmt(ReturnStmt *R) {
    Base::traverseExpr(R->getValue());
    Actions.checkReturnStmt(R);
    //llvm::report_fatal_error("Return is not supported! Add support...");
    return true;
  }

  /// Expressions:
  bool traverseBinaryExpr(BinaryExpr *B) {
    Base::traverseExpr(B->getLeft());
    Base::traverseExpr(B->getRight());
    Actions.actOnBinaryExpr(B);
    return true;
  }

  bool traverseUnaryExpr(UnaryExpr *U) {
    Base::traverseExpr(U->getOperand());
    Actions.actOnUnaryExpr(U);
    return true;
}

  bool traverseCallExpr(CallExpr *C) {
    for (Expr *P : C->getArgs()) {
      Base::traverseExpr(P);
      if (DeclRef *R = dyn_cast<DeclRef>(P))
        Actions.actOnDeclRef(R);
    }
      
    Actions.checkCallExpr(C);
    return true;
  }

  bool traverseDeclRef(DeclRef *DR) {
    Actions.actOnDeclRef(DR);
    return true;
  }

  bool traverseBooleanLiteral(BooleanLiteral *B) {
    B->setInferredType(Actions.Ctx.getBoolTy());
    return true;
  }

  bool traverseIntegerLiteral(IntegerLiteral *I) {
    I->setInferredType(Actions.Ctx.getIntTy());
    return true;
  }

  bool traverseNoneLiteral(NoneLiteral *N) {
    N->setInferredType(Actions.Ctx.getNoneTy());
    return true;
  }

  bool traverseStringLiteral(StringLiteral *S) {
    S->setInferredType(Actions.Ctx.getStrTy());
    return true;
  }

  bool traverseMemberExpr(MemberExpr *M) {
    Base::traverseExpr(M->getObject());
    Actions.checkMemberExpr(M);
    return true;
  }


  bool traverseMethodCallExpr(MethodCallExpr *M) {
    Base::traverseExpr(M->getMethod()->getObject());
    // Сначала обходим аргументы
    for (Expr *Arg : M->getArgs()) {
      Base::traverseExpr(Arg);
    }
      
  
    // Проверка вызова метода
    Actions.checkMethodCallExpr(M);
  
    return true;
  }


  bool traverseIndexExpr(IndexExpr *I) {
    Base::traverseIndexExpr(I);
    Actions.checkIndexExpr(I);
    return true;
  }

  bool traverseListExpr(ListExpr *E) {
    Base::traverseListExpr(E);
    Actions.checkListExpr(E);
    return true;
  }
  /// Types
  bool visitClassType(ClassType *C) {
    /// @todo: Here should be your code
    llvm::report_fatal_error("Classes are not supported! Add support...");
    Actions.checkTypeAnnotation(C);
    return true;
  }

private:
  void handleDeclaration(Declaration *D) {
    StringRef Name = D->getName();
    Scope *S = Actions.getCurScope().get();
    

    if (isa<GlobalDecl>(D)) {
      if (!Actions.checkGlobalDecl(cast<GlobalDecl>(D)))
        return;
      Actions.IdResolver.addDecl(D);
      //return;
    } else if (isa<NonLocalDecl>(D)) {
      if (!Actions.checkNonlocalDecl(cast<NonLocalDecl>(D)))
        return;
      Actions.IdResolver.addDecl(D);
      //return;
    } else if (S != Actions.getGlobalScope().get() && (isa<VarDef>(D) || isa<FuncDef>(D) || isa<ParamDecl>(D))) {
      StringRef Name = D->getName();
      // ищем класс с таким именем в глобальной области
      if (Actions.lookupName(Actions.getGlobalScope().get(), Name)) {
          Declaration *Existing = Actions.lookupName(Actions.getGlobalScope().get(), Name);
          if (isa<ClassDef>(Existing)) {
              Diags.emitError(D->getLocation().Start, diag::err_bad_shadow)
                  << Name;
              return;
          }
      }
    }
    if (Actions.lookupName(S, D->getSymbolInfo())) {
      Diags.emitError(D->getNameId()->getLocation().Start, diag::err_dup_decl) << Name;
      return;
    }
    //if (isa<VarDef>(D) || isa<FuncDef>(D) || isa<ParamDecl>(D)) {
      Actions.handleDeclaration(D);
    /*
    } else {
      llvm::report_fatal_error("Unsupported kind of declaration! Add support...");
    
      }
    */
    //Actions.handleDeclaration(D);
  }

private:
  Sema &Actions;
  DiagnosticsEngine &Diags;
  //FuncDef *CurFunc = nullptr;
};


Sema::Sema(Lexer &L, ASTContext &C)
    : TheLexer(L), Diags(TheLexer.getDiagnostics()), Ctx(C) {}

void Sema::initialize() {
  ClassDef *ObjCD = Ctx.getObjectClass();
  ClassDef *IntCD = Ctx.getIntClass();
  ClassDef *StrCD = Ctx.getStrClass();
  ClassDef *BoolCD = Ctx.getBoolClass();
  ClassDef *NoneCD = Ctx.getNoneClass();

  FuncDef *PrintFD = Ctx.getPrintFunc();
  FuncDef *InputFD = Ctx.getInputFunc();
  FuncDef *LenFD = Ctx.getLenFunc();

  IdResolver.addDecl(ObjCD);
  IdResolver.addDecl(IntCD);
  IdResolver.addDecl(StrCD);
  IdResolver.addDecl(BoolCD);
  IdResolver.addDecl(NoneCD);

  IdResolver.addDecl(PrintFD);
  IdResolver.addDecl(InputFD);
  IdResolver.addDecl(LenFD);
}

void Sema::initializeGlobalScope() {
  ClassDef *ObjCD = Ctx.getObjectClass();
  ClassDef *IntCD = Ctx.getIntClass();
  ClassDef *StrCD = Ctx.getStrClass();
  ClassDef *BoolCD = Ctx.getBoolClass();
  ClassDef *NoneCD = Ctx.getNoneClass();

  FuncDef *PrintFD = Ctx.getPrintFunc();
  FuncDef *InputFD = Ctx.getInputFunc();
  FuncDef *LenFD = Ctx.getLenFunc();

  GlobalScope->addDecl(ObjCD);
  GlobalScope->addDecl(IntCD);
  GlobalScope->addDecl(StrCD);
  GlobalScope->addDecl(BoolCD);
  GlobalScope->addDecl(NoneCD);

  GlobalScope->addDecl(PrintFD);
  GlobalScope->addDecl(InputFD);
  GlobalScope->addDecl(LenFD);
}

void Sema::handleDeclaration(Declaration *D) {
  /// @todo: Here should be your code

  if (CurScope->isDeclInScope(D))
    return;
  CurScope->addDecl(D);
  IdResolver.addDecl(D);
}

void Sema::run() {
  Analysis V(*this);
  V.traverseAST(Ctx);
}

void Sema::actOnPopScope(Scope *S) {
  auto Decls = S->getDecls();
  for (Declaration *D : Decls)
    IdResolver.removeDecl(D);
}

bool Sema::checkNonlocalDecl(NonLocalDecl *NLD) {
  StringRef Name = NLD->getName();

  Scope *S = CurScope->getParent().get();
  while (S && S != GlobalScope.get()) {
    Declaration *D = lookupName(S, NLD->getSymbolInfo());
    if (D) {
      if (!isa<VarDef>(D)) {
        Diags.emitError(NLD->getLocation().Start, diag::err_not_nonlocal) << Name;
        return false;
      }
      return true;
    }
    S = S->getParent().get();
  }

  Diags.emitError(NLD->getLocation().Start, diag::err_not_nonlocal) << Name;
  return false;
}

bool Sema::checkGlobalDecl(GlobalDecl *GD) {
  StringRef Name = GD->getName();
  Declaration *G = lookupName(GlobalScope.get(), GD->getSymbolInfo());

  // Не нашли
  if (!G) {
    Diags.emitError(GD->getLocation().Start, diag::err_not_global) << Name;
    return false;
  }

  // Есть, но не переменная
  if (!isa<VarDef>(G)) {
    Diags.emitError(GD->getLocation().Start, diag::err_not_global) << Name;
    return false;
  }

  if (lookupName(getCurScope().get(), GD->getSymbolInfo())) {
    Diags.emitError(GD->getNameId()->getLocation().Start, diag::err_dup_decl) << Name;
    return false;
  }

  return true;
}


bool Sema::checkSuperClass(ClassDef *C) {
  StringRef SuperName = C->getSuperClass()->getName();

  // ищем суперкласс только среди уже добавленных до текущего класса
  Declaration *SuperDecl = nullptr;
  for (Declaration *D : GlobalScope->getDecls()) {
    if (D == C) break;
    if (D->getName() == SuperName) {
      SuperDecl = D;
      break;
    }
  }

  if (!SuperDecl) {
    Diags.emitError(C->getSuperClass()->getLocation().Start,
                    diag::err_supclass_not_def)
        << SuperName;
    return false;
  }

  if (!isa<ClassDef>(SuperDecl)) {
    Diags.emitError(C->getSuperClass()->getLocation().Start,
                    diag::err_supclass_isnot_class)
        << SuperName;
    return false;
  }

  if (SuperName == "int" || SuperName == "bool" ||
      SuperName == "str" || SuperName == "None") {
    Diags.emitError(C->getSuperClass()->getLocation().Start,
                    diag::err_supclass_is_special_class)
        << SuperName;
    return false;
  }

  return true;
}


bool Sema::checkClassAttrs(ClassDef *C) {
  llvm::DenseSet<StringRef> SeenHere;

  for (Declaration *D : C->getDeclarations()) {
    //if (VarDef *V = dyn_cast<VarDef>(D)) {
      StringRef Name = D->getName();
      if (Name == "__init__") {
        auto FD = dyn_cast<FuncDef>(D);
        auto params = FD->getParams();
        if (params.size() != 1) {
          Diags.emitError(D->getLocation().Start, diag::err_method_override)
                  << Name;
        }
      }

      // Проверка на переопределение атрибута суперкласса
      ClassDef *Super = getSuperClass(C);
      while (Super->getName() != "object") {
        for (Declaration *SD : Super->getDeclarations()) {
          //if (VarDef *SV = dyn_cast<VarDef>(SD)) {
          if (SD->getName() == Name) {
          if (!(isa<FuncDef>(SD) && isa<FuncDef>(D))) {
            
              Diags.emitError(D->getLocation().Start, diag::err_redefine_attr)
                  << Name;
            
          } else {
            checkMethodOverride(dyn_cast<FuncDef>(SD), dyn_cast<FuncDef>(D));
          }
        }
        }
        // переход выше по иерархии
        Super = getSuperClass(Super);
      }

  }

  return true;
}



bool Sema::checkMethodOverride(FuncDef *OM, FuncDef *M) {
  // Проверка возвращаемых типов
  ValueType *OMRetTy = Ctx.convertAnnotationToVType(OM->getReturnType());
  ValueType *MRetTy = Ctx.convertAnnotationToVType(M->getReturnType());
  if (OMRetTy != MRetTy) {
    Diags.emitError(M->getLocation().Start, diag::err_method_override)
        << M->getName();
    return false;
  }

  // Сравнение параметров
  auto OMParams = OM->getParams();
  auto MParams = M->getParams();

  if (OMParams.size() < 1 || MParams.size() < 1) {
    return true;
  }

  auto OMFormal = OMParams.slice(1);
  auto MFormal  = MParams.slice(1);

  if (OMFormal.size() != MFormal.size()) {
    Diags.emitError(M->getLocation().Start, diag::err_method_override)
        << M->getName();
    return false;
  }

  for (size_t i = 0; i < OMFormal.size(); ++i) {
    ValueType *OMTy = Ctx.convertAnnotationToVType(OMFormal[i]->getType());
    ValueType *MTy  = Ctx.convertAnnotationToVType(MFormal[i]->getType());

    if (OMTy != MTy) {
      Diags.emitError(M->getLocation().Start, diag::err_method_override)
          << M->getName();
      return false;
    }
  }

  return true;
}


bool Sema::checkClassDef(ClassDef *D) {
  /// @todo: Here should be your code
  return true;
}


bool Sema::checkFirstMethodParam(ClassDef *CD, FuncDef *FD) {
  auto params = FD->getParams();
  if (params.size() < 1) return false;
  ParamDecl *FirstParam = params[0];

  if (FirstParam->getName() != "self")
    return false;

  ClassType *AnnType = dyn_cast<ClassType>(FirstParam->getType());
  if (!AnnType)
    return false;

  if (AnnType->getClassName() != CD->getName())
    return false;
  return true;
}

bool Sema::checkAssignment(Expr *Left, Expr *Right) {
  ValueType *LTy = nullptr;
  ValueType *RTy = nullptr;
  //auto *annotLTy = Left->get;
  // обработка правой части
  if (isa<NoneLiteral>(Right)) {
    RTy = Ctx.getNoneTy();
  } else if (CallExpr *CE = dyn_cast<CallExpr>(Right)) {
    checkCallExpr(CE);
    RTy = dyn_cast<ValueType>(CE->getInferredType());
  } else if (IndexExpr *IU = dyn_cast<IndexExpr>(Right)) {
    checkIndexExpr(IU);
    RTy = dyn_cast<ValueType>(IU->getList()->getInferredType());
  } else if (IfExpr *IE = dyn_cast<IfExpr>(Right)) {
    Type *Then = IE->getThenExpr()->getInferredType();
    Type *Else = IE->getElseExpr()->getInferredType();
    Type *Cond = dyn_cast<BinaryExpr>(IE->getCondExpr())->getInferredType();
    if (!((Cond == dyn_cast<Type>(Ctx.getBoolTy())) 
          && (dyn_cast<ValueType>(Then) == LTy) 
          && (dyn_cast<ValueType>(Else) == LTy))) {
        //RTy = dyn_cast<ValueType>(Then);
      RTy = Ctx.getObjectTy();
      //return false;
    }
  } else {
    RTy = dyn_cast<ValueType>(Right->getInferredType());
  }

  // обработка левой части
  if (IndexExpr *IE = dyn_cast<IndexExpr>(Left)) {
    if (auto *listType = dyn_cast<ListValueType>(IE->getList()->getInferredType())) {
      LTy = listType->getElementType();
    }
  } else if (MemberExpr *ME = dyn_cast<MemberExpr>(Left)) {
    LTy = dyn_cast<ValueType>(ME->getInferredType());
  } else {
    LTy = dyn_cast<ValueType>(Left->getInferredType());
  }


  if (!LTy || !RTy) {
    return false;
  }

  if (RTy->isNone()) {
    if (LTy->isInt() || LTy->isBool() || LTy->isStr()) {
      Diags.emitError(Left->getLocation().Start, diag::err_tc_assign)
          << *LTy << *RTy;
      return false;
    }
    return true;
  }
  /*
  if (RTy->isEmpty()) {
    if(dyn_cast<ListValueType>(LTy)) {
      return true;
    }
  }
    */

  if (RTy != LTy) {
    Diags.emitError(Left->getLocation().Start, diag::err_tc_assign)
        << *LTy << *RTy;
    return false;
  }
  
  return true;
}

bool Sema::checkAssignTarget(Expr *E) {
  if (E->getInferredType() == Ctx.getObjectTy()) {
    return false;
  }
  //if (GlobalDecl *GD = dyn_cast<GlobalDecl>)
  if (DeclRef *DR = dyn_cast<DeclRef>(E)) {
    auto It = IdResolver.begin(DR->getSymbolInfo());
    if (It == IdResolver.end() || !CurScope->isDeclInScope(*It)) {
      Diags.emitError(DR->getLocation().Start, diag::err_bad_local_assign)
          << DR->getName();
      return false;
    }
  } else if (MemberExpr *ME = dyn_cast<MemberExpr>(E)) {
    return checkMemberExpr1(ME);

  } else if (IndexExpr *IE = dyn_cast<IndexExpr>(E)) {
    return checkIndexExpr1(IE);
  } else {
    llvm::report_fatal_error("Unsupported assignement target! Add support...");
    return false;
  }
  return true;
}
/*
bool Sema::checkCallExpr(CallExpr *C) {

  return true;
}
*/

bool Sema::checkCallExpr(CallExpr *C) {
  // обрабатываем вызываемое выражение
  Expr *Callee = C->getFunction();
  DeclRef *FuncRef = dyn_cast<DeclRef>(Callee);
  /*
  if (!FuncRef) {
    Diags.emitError(C->getLocation().Start, diag::err_call)
        << "<unknown>";
    C->setInferredType(Ctx.getObjectTy());
    return false;
  }
  */
  // ищем объявление
  Declaration *D = lookupDecl(FuncRef);
  if (!D || (!isa<FuncDef>(D) && !isa<ClassDef>(D))) {
    // Нет такой функции или класса
    Diags.emitError(FuncRef->getLocation().Start, diag::err_call)
        << FuncRef->getName();
    C->setInferredType(Ctx.getObjectTy());
    return false;
  }

  // обработка функции
  if (FuncDef *FD = dyn_cast<FuncDef>(D)) {
    auto Params = FD->getParams();
    auto Args = C->getArgs();

    if (Params.size() != Args.size()) {
      Diags.emitError(C->getLocation().Start, diag::err_num_arguments)
          << std::to_string(Params.size()) << std::to_string(Args.size());
      C->setInferredType(Ctx.getObjectTy());
      return false;
    }

    bool Ok = true;
    for (size_t i = 0; i < Args.size(); ++i) {
      Expr *Arg = Args[i];
      //Base::traverseExpr(Arg);

      ValueType *ArgTy = dyn_cast<ValueType>(Arg->getInferredType());
      ValueType *ParamTy = Ctx.convertAnnotationToVType(Params[i]->getType());

      if (!ArgTy || ArgTy != ParamTy) {
        //возможно ошибка, должно быть Arg->getLocation().Start
        Diags.emitError(C->getLocation().Start, diag::err_tc_argument)
        << *ParamTy << *ArgTy << std::to_string(static_cast<int>(i));
        Ok = false;
        if(!Ok) return false;
      }
    }

    //устанавливаем возвращаемый тип
    C->setInferredType(Ctx.convertAnnotationToVType(FD->getReturnType()));
    return Ok;
  }

  // обработка вызова конструктора класса
  if (ClassDef *CD = dyn_cast<ClassDef>(D)) {
    C->setInferredType(Ctx.getClassVType(CD->getName()));
    return true;
  }

  return false;
}




bool Sema::checkIndexExpr(IndexExpr *E) {
  // получаем тип индексируемого выражения
  Type *listType = E->getList()->getInferredType();
  int falseListType = false;
  // проверяем, что тип поддерживает индексацию
  if (!ListValueType::classof(listType)) {
      Diags.emitError(E->getLocation().Start, diag::err_cannot_index)
          << *listType;
      falseListType = true;
  }
  if (DeclRef::classof(E->getList())) {
    if (!IntegerLiteral::classof(E->getIndex())) {
      Diags.emitError(E->getLocation().Start, diag::err_index_not_int)
          << *dyn_cast<Type>(E->getIndex()->getInferredType());
      return false;
    }
    //DeclRef curDeclRef = E->getList();
    if (falseListType) return false;
  } 
  return true;
}

bool Sema::checkIndexExpr1(IndexExpr *E) {
  // получаем тип индексируемого выражения
  Type *listType = E->getList()->getInferredType();

  // проверяем, что тип поддерживает индексацию
  if (!ListValueType::classof(listType) && listType != Ctx.getEmptyTy()) {
      return false;
  }
  if (DeclRef::classof(E->getList())) {
    if (!IntegerLiteral::classof(E->getIndex())) {
      return false;
    }
    return true;
  } else {
    if (ListExpr::classof(E->getList())) {
      checkListExpr(dyn_cast<ListExpr>(E->getList()));
    }
    return false;
  }
  return true;
}

bool Sema::checkMemberExpr(MemberExpr *ME) {
  Expr *Obj = ME->getObject();
  DeclRef *Member = ME->getMember();
  ValueType *ObjTy = dyn_cast<ValueType>(Obj->getInferredType());

  if (!ObjTy || !isa<ClassValueType>(ObjTy)) {
    Diags.emitError(ME->getLocation().Start, diag::err_no_attr)
        << Member->getName() << "<non-class>";
    return false;
  }

  auto *ClassTy = cast<ClassValueType>(ObjTy);
  Declaration *ClassDecl = lookupName(GlobalScope.get(), ClassTy->getClassName());
  if (!ClassDecl || !isa<ClassDef>(ClassDecl)) {
    Diags.emitError(ME->getLocation().Start, diag::err_no_attr)
        << Member->getName() << ClassTy->getClassName();
    return false;
  }

  ClassDef *CD = cast<ClassDef>(ClassDecl);
  Declaration *Found = findDeclaration(CD, Member);
  if (!Found) {
    Diags.emitError(ME->getLocation().Start, diag::err_no_attr)
        << Member->getName() << CD->getName();
    return false;
  }

  if (VarDef *VD = dyn_cast<VarDef>(Found)) {
    Member->setInferredType(Ctx.convertAnnotationToVType(VD->getType()));
    ME->setInferredType(Ctx.convertAnnotationToVType(VD->getType()));
    return true;
  }


  Diags.emitError(ME->getLocation().Start, diag::err_no_attr)
      << Member->getName() << CD->getName();
  return false;
}

bool Sema::checkMemberExpr1(MemberExpr *ME) {
  Expr *Obj = ME->getObject();
  DeclRef *Member = ME->getMember();
  ValueType *ObjTy = dyn_cast<ValueType>(Obj->getInferredType());

  if (!ObjTy || !isa<ClassValueType>(ObjTy)) {
    return false;
  }

  auto *ClassTy = cast<ClassValueType>(ObjTy);
  Declaration *ClassDecl = lookupName(GlobalScope.get(), ClassTy->getClassName());
  if (!ClassDecl || !isa<ClassDef>(ClassDecl)) {
    return false;
  }

  ClassDef *CD = cast<ClassDef>(ClassDecl);
  Declaration *Found = findDeclaration(CD, Member);
  if (!Found) {
    return false;
  }

  if (VarDef *VD = dyn_cast<VarDef>(Found)) {
    Member->setInferredType(Ctx.convertAnnotationToVType(VD->getType()));
    ME->setInferredType(Ctx.convertAnnotationToVType(VD->getType()));
    return true;
  }

  return false;
}



bool Sema::checkMethodCallExpr(MethodCallExpr *M) {
  MemberExpr *ME = M->getMethod();
  Expr *Obj = ME->getObject();
  DeclRef *Member = ME->getMember();

  ValueType *ObjTy = dyn_cast<ValueType>(Obj->getInferredType());

  if (!ObjTy || !isa<ClassValueType>(ObjTy)) {
    Diags.emitError(M->getLocation().Start, diag::err_no_method)
        << Member->getName() << "<non-class>";
    return false;
  }

  auto *ClassTy = cast<ClassValueType>(ObjTy);
  Declaration *ClassDecl = lookupName(GlobalScope.get(), ClassTy->getClassName());
  if (!ClassDecl || !isa<ClassDef>(ClassDecl)) {
    Diags.emitError(M->getLocation().Start, diag::err_no_method)
        << Member->getName() << ClassTy->getClassName();
    return false;
  }

  ClassDef *CD = cast<ClassDef>(ClassDecl);
  Declaration *Found = findDeclaration(CD, Member);
  if (!Found || !isa<FuncDef>(Found)) {
    Diags.emitError(M->getLocation().Start, diag::err_no_method)
        << Member->getName() << CD->getName();
    return false;
  }

  FuncDef *MethodDef = cast<FuncDef>(Found);
  ArrayRef<ParamDecl *> Params = MethodDef->getParams();
  ArrayRef<Expr *> Args = M->getArgs();

  // Проверка количества аргументов
  if (Params.size() == 0 || Params.size() - 1 != Args.size()) {
    Diags.emitError(M->getLocation().Start, diag::err_num_arguments)
        << std::to_string(Params.size() - 1) << std::to_string(Args.size());
    return false;
  }
  //проверка аргументов
  bool containsWrongArgs = false;
  for (size_t i = 0; i < Args.size(); ++i) {
    ValueType *ArgTy = dyn_cast<ValueType>(Args[i]->getInferredType());
    ValueType *ParamTy = Ctx.convertAnnotationToVType(Params[i + 1]->getType());
    if (ArgTy != ParamTy) {
      /*предполагаемый правильный вид ошибки:
      Diags.emitError(Args[i]->getLocation().Start, diag::err_tc_argument)
          << *ParamTy << *ArgTy << std::to_string(i);
      */
      Diags.emitError(M->getLocation().Start, diag::err_tc_argument)
          << *ParamTy << *ArgTy << std::to_string(i+1);
      containsWrongArgs = true;
    }
  }
  if(containsWrongArgs) return false;

  M->setInferredType(Ctx.convertAnnotationToVType(MethodDef->getReturnType()));
  return true;
}


bool Sema::checkReturnStmt(ReturnStmt *S) {
  /// @todo: Here should be your code
  if (CurScope == GlobalScope) {
    Diags.emitError(S->getLocation().Start, diag::err_bad_return_top);
    return false;
  }
  //ValueType *retType = nullptr;
  if (S->getValue() == nullptr) {
    //retType = Ctx.getNoneTy();
    return true;
  }
  //retType = dyn_cast<ValueType>(S->getValue()->getInferredType());
  
  return true;
}

bool Sema::checkReturnMissing(FuncDef *F) {
  ValueType *RetTy = Ctx.convertAnnotationToVType(F->getReturnType());
  if (RetTy == Ctx.getObjectTy()) {
    return true;
  }
  if (RetTy == Ctx.getNoneTy())
    return true;

  if (!alwaysReturns(F->getStatements())) {
    Diags.emitError(F->getLocation().Start, diag::err_maybe_falloff_nonvoid)
        << F->getName();
    return false;
  }

  return true;
}

bool Sema::alwaysReturns(ArrayRef<Stmt *> Stmts) {
  for (size_t i = 0; i < Stmts.size(); ++i) {
    Stmt *S = Stmts[i];

    if (isa<ReturnStmt>(S)) {
      return true;
    }

    if (IfStmt *If = dyn_cast<IfStmt>(S)) {
      auto Then = If->getThenBody();
      auto Else = If->getElseBody();

      bool thenReturns = alwaysReturns(Then);
      bool elseReturns = alwaysReturns(Else);

      if (thenReturns && elseReturns) {
        return true;
      } else {
        continue;
      }
    }

    if (isa<WhileStmt>(S)) {
      continue;
    }

  }

  return false;
}




bool Sema::checkTypeAnnotation(ClassType *C) {
  Scope *S = getCurScope().get();
  Declaration *D =
      lookupClass(S, C) ? lookupClass(S, C) : lookupClass(GlobalScope.get(), C);
  if (!D || !ClassDef::classof(D)) {
    if (D && (D->getName() == "bool" || D->getName() == "int" ||
              D->getName() == "str"))
      return true;
    Diags.emitError(C->getLocation().Start, diag::err_invalid_type_annotation)
        << C->getClassName();
    return false;
  }
  return true;
}

bool Sema::checkListExpr(ListExpr *E) {
  if (E->getElements().empty()) {
    E->setInferredType(dyn_cast<Type>(Ctx.getEmptyTy()));
    
  } else {
    auto vec = E->getElements();
    Type *firstElemType = vec[0]->getInferredType();

    for (const auto &cur : vec) {
      if (cur->getInferredType() == firstElemType)
        continue;

      else {
        E->setInferredType(dyn_cast<Type>(Ctx.getObjectTy()));
        //Diags.emitError(cur->getLocation().Start, diag::err_list_element_type_mismatch);
        return true;
      }
    }
    E->setInferredType(
        dyn_cast<Type>(Ctx.getListVType(dyn_cast<ValueType>(firstElemType))));
  }
  return true;
}


void Sema::actOnVarDef(VarDef *V) {
  auto &RTy = *cast<ValueType>(V->getValue()->getInferredType());
  auto &LTy = *cast<ValueType>(Ctx.convertAnnotationToVType(V->getType()));
  if (ClassType::classof(V->getType())) {
    ClassType *CT = cast<ClassType>(V->getType());
    checkTypeAnnotation(CT);
  }
  if (!(RTy <= LTy))
    Diags.emitError(V->getLocation().Start, diag::err_tc_assign) << LTy << RTy;
}

void Sema::actOnBinaryExpr(BinaryExpr *B) {
  ValueType &LTy = *cast<ValueType>(B->getLeft()->getInferredType());
  ValueType &RTy = *cast<ValueType>(B->getRight()->getInferredType());

  ListValueType *LListTy;
  ListValueType *RListTy;

  bool Err = false;

  switch (B->getOpKind()) {
  case BinaryExpr::OpKind::Add:
    if (LTy.isInt() || RTy.isInt()) {
      Err = &LTy != &RTy;
      B->setInferredType(Ctx.getIntTy());
    } else if (LTy.isStr() || RTy.isStr()) {
      Err = &LTy != &RTy;
      B->setInferredType(Ctx.getStrTy());
    } else {
      LListTy = dyn_cast<ListValueType>(&LTy);
      RListTy = dyn_cast<ListValueType>(&RTy);
      if (LListTy && RListTy) {
        Err = LListTy->getElementType() != RListTy->getElementType();
      }
      else
        Err = true;
      B->setInferredType(Err ? static_cast<ValueType *>(Ctx.getObjectTy())
                             : static_cast<ValueType *>(LListTy));
    }
    break;
  case BinaryExpr::OpKind::Sub:
  case BinaryExpr::OpKind::Mul:
  case BinaryExpr::OpKind::Mod:
  case BinaryExpr::OpKind::FloorDiv:
    Err = !LTy.isInt() || !RTy.isInt();
    B->setInferredType(Ctx.getIntTy());
    break;

  case BinaryExpr::OpKind::And:
  case BinaryExpr::OpKind::Or:
    Err = !LTy.isBool() || !RTy.isBool();
    B->setInferredType(Ctx.getBoolTy());
    break;
    

  case BinaryExpr::OpKind::EqCmp:
  case BinaryExpr::OpKind::NEqCmp:
    Err = (&LTy != &RTy) || (!LTy.isInt() && !LTy.isBool() && !LTy.isStr());
    B->setInferredType(Ctx.getBoolTy());
    break;

  case BinaryExpr::OpKind::LEqCmp:
  case BinaryExpr::OpKind::GEqCmp:
  case BinaryExpr::OpKind::LCmp:
  case BinaryExpr::OpKind::GCmp:
    Err = (&LTy != &RTy) || (!LTy.isInt() && !LTy.isStr());
    B->setInferredType(Ctx.getBoolTy());
    break;

  case BinaryExpr::OpKind::Is: {
    ValueType &LTy = *cast<ValueType>(B->getLeft()->getInferredType());
    ValueType &RTy = *cast<ValueType>(B->getRight()->getInferredType());

    bool Valid = true;

    // Проверяем, что типы не являются примитивами (int, bool, str)
    if (LTy.isInt() || LTy.isBool() || LTy.isStr() || 
        RTy.isInt() || RTy.isBool() || RTy.isStr()) {
        Valid = false;
    }
    if (Valid) {
        B->setInferredType(Ctx.getBoolTy());
    } else {
        Err = true;
    }
    break;
  }
  }

  if (Err) {
      Diags.emitError(B->getLocation().Start, diag::err_tc_binary)
      << B->getOpKindStr() << LTy << RTy;
    }


}

void Sema::actOnUnaryExpr(UnaryExpr *E) {
  ValueType &Ty = *cast<ValueType>(E->getOperand()->getInferredType());

  switch (E->getOpKind()) {
  case UnaryExpr::OpKind::Minus:
    if (!Ty.isInt()) 
      Diags.emitError(E->getLocation().Start, diag::err_tc_unary) << "-" << Ty;
    break;
  case UnaryExpr::OpKind::Not:
    if (!Ty.isBool()) 
      Diags.emitError(E->getLocation().Start, diag::err_tc_unary) << "not" << Ty;
    break;
  }
}

void Sema::actOnDeclRef(DeclRef *DR) {
  Declaration *D = lookupDecl(DR);

  if (!D) {
    SMLoc Loc = DR->getLocation().Start;
    Diags.emitError(Loc, diag::err_not_variable) << DR->getName();
    DR->setInferredType(Ctx.getObjectTy());
    return;
  }

  if (FuncDef *FD = dyn_cast<FuncDef>(D)) {
    ValueTypeList ParamTypes;
    for (ParamDecl *P : FD->getParams()) {
      ParamTypes.push_back(Ctx.convertAnnotationToVType(P->getType()));
    }

    ValueType *ReturnType = Ctx.convertAnnotationToVType(FD->getReturnType());

    FuncType *FT = Ctx.getFuncType(ParamTypes, ReturnType);

    DR->setDeclInfo(FD);
    DR->setInferredType(FT);
    return;
  }
  auto convertToValueTy = [DR, this](auto *D) {
    ValueType *VT = Ctx.convertAnnotationToVType(D->getType());
    DR->setDeclInfo(D);
    DR->setInferredType(VT);
  };


  llvm::TypeSwitch<Declaration *>(D)
      .Case<ParamDecl>(convertToValueTy)
      .Case<VarDef>(convertToValueTy)
      //.Case<FuncDef>(convertToValueTy)
      .Default([](auto) {
        llvm::report_fatal_error("Unsupported declaration! Add support...");
      });
}

Scope *Sema::getScopeForDecl(Scope *S, Declaration *D) {
  do {
    if (S->isDeclInScope(D))
      return S;
  } while ((S = S->getParent().get()));
  return nullptr;
}

ClassDef *Sema::getSuperClass(ClassDef *C) {
  Identifier *SuperID = C->getSuperClass();

  if (!SuperID)
    return nullptr;

  StringRef SuperName = SuperID->getName();

  // bщем суперкласс только в глобальной области видимости
  Declaration *Decl = lookupName(GlobalScope.get(), SuperName);
  if (!Decl)
    return nullptr;

  return dyn_cast<ClassDef>(Decl);
}


bool Sema::isSameType(TypeAnnotation *TyA, TypeAnnotation *TyB) {
  /// @todo: Here should be your code
  llvm::report_fatal_error("Unsupported feature! Add support...");
  return false;
}

Declaration *Sema::lookupName(Scope *S, SymbolInfo *SI) {
  auto Decls = S->getDecls();
  auto It = llvm::find_if(
      Decls, [SI](Declaration *D) { return D->getSymbolInfo() == SI; });
  if (It != Decls.end())
    return *It;
  return nullptr;
}

Declaration *Sema::lookupName(Scope *S, StringRef Name) {
  auto Decls = S->getDecls();
  auto It = llvm::find_if(Decls, [Name](Declaration *D) {
      return D->getName() == Name;
  });
  return (It != Decls.end()) ? *It : nullptr;
}

Declaration *Sema::lookupDecl(DeclRef *DR) {
  SymbolInfo *SI = DR->getSymbolInfo();
  IdentifierResolver::iterator I = IdResolver.begin(SI);
  IdentifierResolver::iterator E = IdResolver.end();

  Declaration *D = nullptr;

  for (; !D && I != E; ++I) {
    if (GlobalDecl *GD = dyn_cast<GlobalDecl>(*I)) {
      D = lookupName(GlobalScope.get(), GD->getSymbolInfo());
      return cast<VarDef>(D);
    }

    if (isa<NonLocalDecl>(*I))
      continue;
    D = *I;
  }

  return D;
}

Declaration *Sema::lookupClass(Scope *S, ClassType *CT) {
  auto Decls = S->getDecls();
  auto It = llvm::find_if(Decls, [CT](Declaration *D) {
    return D->getName() == CT->getClassName();
  });
  if (It != Decls.end())
    return *It;
  return nullptr;
}

Declaration *Sema::findDeclaration(ClassDef *C, DeclRef *M) {
  for (Declaration *D : C->getDeclarations()) {
    if (D->getName() == M->getName())
      return D;
  }
  if (ClassDef *SC = getSuperClass(C))
    return findDeclaration(SC, M);
  else
    return nullptr;
}
} // namespace chocopy