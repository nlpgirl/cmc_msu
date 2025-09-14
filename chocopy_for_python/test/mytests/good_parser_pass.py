# RUN: %chocopy-llvm %s -ast-dump | diff %s.ast -

def foo() -> object:
    pass