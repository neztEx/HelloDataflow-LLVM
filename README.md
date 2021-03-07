# HelloDataflow-LLVM
Getting started to write Dataflow analysis in LLVM. (Without standard templates)

## Code Guidelines for LLVM APIs
1. See how to access functions, basic blocks, and instructions in [HelloDataflow-LLVM](https://github.com/ufarooq/HelloPass-LLVM/). 
2. The following LLVM and C++ APIs might be helpful for implementing a dataflow analysis. 
- Given a basic block, to find out its successor(s), use the following snippet to iterate over its successors.
```cpp
//#include "llvm/IR/CFG.h"

for (BasicBlock *Succ : successors(&basic_block)) {
	...
}
```
- Similarly, to find out the predecessor(s) of a basic block, use the following snippet to iterate over its predecessors.
```cpp
//#include "llvm/IR/CFG.h"

for (BasicBlock *Pred : predecessors(&basic_block)) {
	...
}
```
- Use standard C++ APIs to perform basic set operations such as union of two sets or vectors. 
```cpp
// #include <set>
std::vector<int> v1 = {1, 2, 3, 4, 5}; 
std::vector<int> v2 = {3, 4, 5, 6, 7}; 
std::vector<int> dest1; // create a third vector to write results

std::set_union(v1.begin(), v1.end(), v2.begin(), v2.end(),std::back_inserter(dest1)); // writes Union (v1+v2) to dest1
```
- In a similar way, intersection and difference operations can be performed. 
```cpp
std::set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(),std::back_inserter(dest1)); // writes difference (v1-v2) to dest1

std::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(),std::back_inserter(dest1)); // writes Common elements of both sets to dest1

``` 

# HelloPass-LLVM
Getting started for LLVM pass writing

## Setup and Run
1. Clone this repo, see the file details below:
- [Pass](https://github.com/ufarooq/HelloPass-LLVM/tree/master/Pass "Pass") - root directory for build and source code
   - [Transforms](https://github.com/ufarooq/HelloPass-LLVM/tree/master/Pass/Transforms "Transforms") is top level directory for project.
   - [CMakeLists.txt](https://github.com/ufarooq/HelloPass-LLVM/blob/master/Pass/Transforms/CMakeLists.txt "CMakeLists.txt")  file is CMakeLists for the Project.
     - [ValueNumbering](https://github.com/ufarooq/HelloPass-LLVM/tree/master/Pass/Transforms/ValueNumbering "ValueNumbering") is top level directory for the Pass
	     - [ValueNumbering.cpp](https://github.com/ufarooq/HelloPass-LLVM/blob/master/Pass/Transforms/ValueNumbering/ValueNumbering.cpp "ValueNumbering.cpp") contains code for the Pass
	     - [CMakeLists.txt](https://github.com/ufarooq/HelloPass-LLVM/blob/master/Pass/Transforms/ValueNumbering/CMakeLists.txt "CMakeLists.txt") is CMakeLists for this Pass
   - [build](https://github.com/ufarooq/HelloPass-LLVM/tree/master/Pass/build "build") will build Pass in this directory
- [test](https://github.com/ufarooq/HelloPass-LLVM/tree/master/test "test") contains Test code
2. For Mac OSX users, uncomment the following line in Pass/Transforms/ValueNumbering/CMakeLists.txt
```bash
SET(CMAKE_MODULE_LINKER_FLAGS "-undefined dynamic_lookup")
```
3. Move to [Pass](https://github.com/ufarooq/HelloPass-LLVM/tree/master/Pass)/**build**/ directory using cd command on your local system. Next, execute the following command. If it executes successfully, proceed to next step.
```bash
cmake -DCMAKE_BUILD_TYPE=Release ../Transforms/ValueNumbering
```
4. Next execute make and it will generate *.so files under build directory. 
```bash
make -j4
```
5. Move to **test**/ directory and generate ***test.ll*** file for test.c using following command. 
```c++
clang -S -emit-llvm test.c -o test.ll
```
6. After generating test.ll, run the following command to test the LLVM Pass. 
```bash
opt -load ../Pass/build/libLLVMValueNumberingPass.so  -ValueNumbering test.ll
```
## Code Explanation 
- The implemented Pass extends from ``FunctionPass`` class and overrides ``runOnFunction(Function &F)`` function.
- ``runOnFunction(Function &F)`` function gets called for each function in the test code. Name of the function being analyzed is accessible using the following code snippet. 
```c++
bool runOnFunction(Function &F) override {
	F.getName();
}
```
- To print out to the screen, you need to redirect strings to ``errs()``, as in:
```c++
bool runOnFunction(Function &F) override {
	errs() << "function name: " << F.getName() << "\n";
}
```
- We can iterate over basic blocks of the given function as:
```c++
bool runOnFunction(Function &F) override {
	for (auto& basic_block : F)
	{
		...
	}
}
```
- Next, we can iterate over the instructions in a basic block (BB). **Note:** instructions are in LLVM IR.
```c++
bool runOnFunction(Function &F) override {
    for (auto& basic_block : F)
    {
        for (auto& inst : basic_block)
        {
            ...
        }
    }
}
```
- Once we get an instruction, then we can cast it as ``User`` and iterate over operands of that instruction. 
```c++
auto* ptr = dyn_cast<User>(&inst);
for (auto it = ptr->op_begin(); it != ptr->op_end(); ++it) 
{
    ...
}
```
- You can also access the operands using ``getOperand(operand_index)`` as in:
```c++
for (auto& inst : basic_block)
{
    ...
    errs() << "operand: " << inst.getOperand(0) << "\n";
    ...
}
```
- You can check whether instruction is a binary operation (like ``a = b + c``) with ``isBinaryOp()``
```c++
if (inst.isBinaryOp())
{
    ...
}
```
- You can find operator types with ``getOpcode()`` and predefined opcodes
```c++
if (inst.isBinaryOp())
{
    inst.getOpcodeName(); //prints OpCode by name such as add, mul etc.
    if(inst.getOpcode() == Instruction::Add)
    {
        errs() << "This is Addition"<<"\n";
    }
    if(inst.getOpcode() == Instruction::Mul)
    {
        errs() << "This is Multiplication"<<"\n";
    }
    // See Other classes Instruction::Sub, Instruction::UDiv, Instruction::SDiv
}
```
- A sample implementation of ``runOnFunction(Function &F)``:  
```c++
string func_name = "test";
bool runOnFunction(Function &F) override {

    errs() << "ValueNumbering: ";
    errs() << F.getName() << "\n";
    if (F.getName() != func_name) 
        return false;
	
    for (auto& basic_block : F)
    {
        for (auto& inst : basic_block)
        {
            errs() << inst << "\n";
            auto* ptr = dyn_cast<User>(&inst);
            for (auto it = ptr->op_begin(); it != ptr->op_end(); ++it) 
            {
                errs() << "\t" << *(*it) << "\n";
            }
        }
    }
    return false;
}
```
