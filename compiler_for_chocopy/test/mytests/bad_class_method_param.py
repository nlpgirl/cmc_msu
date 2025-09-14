# RUN: %chocopy-llvm --run-sema %s 2>&1 | FileCheck %s.err

class A(object):
    x:int = 1
    y:int = 2

    def set_A(self: "A", val1: int, val2: int) -> object:
        self.x = val1
        self.y = val2

a:A = None

a = A()

a.set_A(True, False)


