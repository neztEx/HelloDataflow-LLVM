#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/Support/FormatVariadic.h"
#include <string>
#include <map>

using namespace llvm;
using namespace std;

#define DEBUG_TYPE "ValueNumbering"

using namespace llvm;

namespace {
struct ValueNumbering : public FunctionPass {
    string func_name = "test";
    static char ID;
    
    map<llvm::Value *,int> valTable;
    map<string, int> redTable;
    int valueNumber = 0;
    string op = "";
    string repeat = "";

    ValueNumbering() : FunctionPass(ID) {}

    bool runOnFunction(Function &F) override {
	    
        errs() << "ValueNumbering: " << F.getName() << "\n";
        // if (F.getName() != func_name) return false;

        for (auto& basic_block : F)
        {
            for (auto& inst : basic_block)
            {
                // errs() << inst << "\n";
                // errs() << inst.getOperand(0) << "\n";
                if (inst.getOpcode() == Instruction::Alloca){
                    // errs() << formatv("{0,-40}", inst);
                    continue;
                }

                if(inst.getOpcode() == Instruction::Load){
                    // errs() << "Load ";

                    if (valTable.find(inst.getOperand(0)) == valTable.end()){
                        valTable[inst.getOperand(0)] = ++valueNumber;
                    }
                    // if (valTable.find(inst.getOperandList()->getUser()) == valTable.end()){
                    //     valTable[inst.getOperandList()->getUser()] = valTable[inst.getOperand(0)];
                    // }
                    valTable[inst.getOperandList()->getUser()] = valTable[inst.getOperand(0)];
                    // errs() << valueNumber << "\n";

                    errs() << formatv("{0,-40}", inst) << valTable[inst.getOperandList()->getUser()] << " = " << valTable[inst.getOperandList()->getUser()] << "\n";
                }
                if(inst.getOpcode() == Instruction::Store){
                    // errs() << "Store ";
                    //check if operand0 exist in map, if it doesn't create value number and assigned op0 = op1
                    if (valTable.find(inst.getOperand(0)) == valTable.end()){
                        valTable[inst.getOperand(0)] = ++valueNumber;
                        valTable[inst.getOperand(1)] = valTable[inst.getOperand(0)];
                        // errs() << valueNumber << "\n";
                        errs() << formatv("{0,-40}", inst) << valTable[inst.getOperand(0)] << " = " << valTable[inst.getOperand(1)] << "\n";
                    }
                    else{
                        //if value does exist, then op1 = op2 output instruction
                        valTable[inst.getOperand(1)] = valTable[inst.getOperand(0)];
                        // errs() << valueNumber << "\n";
                        errs() << formatv("{0,-40}", inst) << valTable[inst.getOperand(0)] << " = " << valTable[inst.getOperand(1)] << "\n";
                    }

                }
                if (inst.isBinaryOp())
                {
                    // errs() << "Op Code:" << inst.getOpcodeName()<<"\n";
                    if(inst.getOpcode() == Instruction::Add){
                        // errs() << "Add ";
                        op = "Add";
                        if (valTable.find(inst.getOperand(0)) == valTable.end()){
                            valTable[inst.getOperand(0)] = ++valueNumber;
                            // errs() << valueNumber << "\n";
                        }
                        if (valTable.find(inst.getOperand(1)) == valTable.end()){
                            valTable[inst.getOperand(1)] = ++valueNumber;
                            // errs() << valueNumber << "\n";
                        }
                        string temp = to_string(valTable[inst.getOperand(0)]) + op + to_string(valTable[inst.getOperand(1)]);

                        if (redTable.find(temp) != redTable.end()){
                             valTable[inst.getOperandList()->getUser()] = redTable[temp];
                        }
                        else if (valTable.find(inst.getOperandList()->getUser()) == valTable.end()){
                            valTable[inst.getOperandList()->getUser()] = ++valueNumber;
                            // errs() << valueNumber << "\n";
                        }

                    }
                    if(inst.getOpcode() == Instruction::Sub){
                        // errs() << "Sub ";
                        op = "Sub";
                        if (valTable.find(inst.getOperand(0)) == valTable.end()){
                            valTable[inst.getOperand(0)] = ++valueNumber;
                            // errs() << valueNumber << "\n";
                        }
                        if (valTable.find(inst.getOperand(1)) == valTable.end()){
                            valTable[inst.getOperand(1)] = ++valueNumber;
                            // errs() << valueNumber << "\n";
                        }
                        string temp = to_string(valTable[inst.getOperand(0)]) + op + to_string(valTable[inst.getOperand(1)]);

                        if (redTable.find(temp) != redTable.end()){
                             valTable[inst.getOperandList()->getUser()] = redTable[temp];
                        }
                        else if (valTable.find(inst.getOperandList()->getUser()) == valTable.end()){
                            valTable[inst.getOperandList()->getUser()] = ++valueNumber;
                            // errs() << valueNumber << "\n";
                        }


                    }
                    if(inst.getOpcode() == Instruction::Mul){
                        // errs() << "Mult ";
                        op = "Mult";
                        if (valTable.find(inst.getOperand(0)) == valTable.end()){
                            valTable[inst.getOperand(0)] = ++valueNumber;
                            // errs() << valueNumber << "\n";
                        }
                        if (valTable.find(inst.getOperand(1)) == valTable.end()){
                            valTable[inst.getOperand(1)] = ++valueNumber;
                            // errs() << valueNumber << "\n";
                        }
                        string temp = to_string(valTable[inst.getOperand(0)]) + op + to_string(valTable[inst.getOperand(1)]);

                        if (redTable.find(temp) != redTable.end()){
                             valTable[inst.getOperandList()->getUser()] = redTable[temp];
                        }
                        else if (valTable.find(inst.getOperandList()->getUser()) == valTable.end()){
                            valTable[inst.getOperandList()->getUser()] = ++valueNumber;
                            // errs() << valueNumber << "\n";
                        }


                    }
                    if(inst.getOpcode() == Instruction::UDiv){
                        // errs() << "Div ";
                        op = "Div";
                        if (valTable.find(inst.getOperand(0)) == valTable.end()){
                            valTable[inst.getOperand(0)] = ++valueNumber;
                            // errs() << valueNumber << "\n";
                        }
                        if (valTable.find(inst.getOperand(1)) == valTable.end()){
                            valTable[inst.getOperand(1)] = ++valueNumber;
                            // errs() << valueNumber << "\n";
                        }
                        string temp = to_string(valTable[inst.getOperand(0)]) + op + to_string(valTable[inst.getOperand(1)]);

                        if (redTable.find(temp) != redTable.end()){
                             valTable[inst.getOperandList()->getUser()] = redTable[temp];
                        }
                        else if (valTable.find(inst.getOperandList()->getUser()) == valTable.end()){
                            valTable[inst.getOperandList()->getUser()] = ++valueNumber;
                            // errs() << valueNumber << "\n";
                        }

                    }
                    
                    // see other classes, Instruction::Sub, Instruction::UDiv, Instruction::SDiv
                    auto* ptr = dyn_cast<User>(&inst);

                    int i = 0;
                    int temp1 = 0;
                    int temp2 = 0;

                    for (auto it = ptr->op_begin(); it != ptr->op_end(); ++it) {
                        
                        //check for redundant computation, by adding the value numbers as a string
                        //to a map
                        if (i == 0){
                            temp1 = valTable[(*it)];
                        }
                        else{
                            temp2 = valTable[(*it)];
                            string temp = to_string(temp1) + op + to_string(temp2);

                            if (redTable.find(temp) == redTable.end()){
                                redTable[temp] = valTable[inst.getOperandList()->getUser()];
                            }
                            else{
                                repeat = " (redundant) ";
                                valTable[inst.getOperandList()->getUser()] = redTable[temp];
                            }
                        }
                        i++;
                        // if ((*it)->hasName()) 
            			//     errs() << (*it)->getName() << "\n";                      
                    }
                    //output for binary OP w/ redundant computation output
                    errs() << formatv("{0,-40}", inst) << valTable[inst.getOperandList()->getUser()] << " = " << temp1 << " " + op + " " << temp2 << repeat << "\n";
                    repeat = "";
                } // end if
            } // end for inst
        } // end for block
        return false;
    } // end runOnFunction
}; // end of struct ValueNumbering
}  // end of anonymous namespace

char ValueNumbering::ID = 0;
static RegisterPass<ValueNumbering> X("ValueNumbering", "ValueNumbering Pass",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);
