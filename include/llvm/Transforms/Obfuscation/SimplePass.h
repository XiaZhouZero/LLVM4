#include "llvm/IR/User.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Constants.h"
#include "llvm/Pass.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Support/Format.h"

using namespace std;

namespace llvm {
    ModulePass *createSimplePass();
    ModulePass *createSimplePass(bool flag);
}