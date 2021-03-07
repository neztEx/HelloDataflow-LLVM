#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"

using namespace llvm;
using namespace std;

#define DEBUG_TYPE "Liveness"

using namespace llvm;

namespace {
struct Liveness : public FunctionPass {
    static char ID;
    
    Liveness() : FunctionPass(ID) {}

    
    
    
    
    
    bool runOnFunction(Function &F) override {
	    
        errs() << "----- entry -----\n";
        for (auto& basic_block : F)
        {
           
        } // end for block
        return false;
    } // end runOnFunction

    

}; // end of struct Liveness
}  // end of anonymous namespace

char Liveness::ID = 0;
static RegisterPass<Liveness> X("Liveness", "Liveness Pass",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);
