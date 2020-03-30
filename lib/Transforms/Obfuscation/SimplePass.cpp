/*
    LLVM Simple Pass
    Copyright (C) 2017 Zhang(http://mayuyu.io)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "llvm/Transforms/Obfuscation/SimplePass.h"

#define DEBUG_TYPE "simpe"

using namespace llvm;
using namespace std;

namespace llvm {
    struct Simple : public ModulePass {
        static char ID;
        bool flag;
        Simple(bool flag): ModulePass(ID) {
            this->flag=flag;
        }
        Simple(): ModulePass(ID) {
        this->flag=true;
        }

        uint8_t get_random_uint8() {
            srand((uint8_t)time(NULL));
            uint8_t random_num = rand();
            return random_num;
        }

        bool runOnModule(Module& M) override {
            for(auto GV=M.global_begin(); GV!=M.global_end(); GV++){
                GlobalVariable *GVPtr = &*GV;
                //Filter out GVs that could potentially be referenced outside of current TU
                if(GVPtr->hasInitializer() && (GVPtr->hasPrivateLinkage() || GVPtr->hasInternalLinkage())) {
                    if(ConstantInt *CI = dyn_cast<ConstantInt>(GVPtr->getInitializer())) {
                        //Prepare Types and Keys
                        IntegerType* IT = cast<IntegerType>(CI->getType());
                        //注意这里的cryptoutils->get_uint8_t(); 是Obfuscator-LLVM提供的密码学安全的随机数生成器，您也可以使用其他方式来生成随机的XOR Key。
                        //uint8_t K = cryptoutils->get_uint8_t();
                        uint8_t K = this->get_random_uint8();
                        ConstantInt *XORKey = ConstantInt::get(IT, K);
                        //Encrypt Original GV
                        ConstantInt *newGVInit = ConstantInt::get(IT, CI->getZExtValue()^K);    // 计算加密后的值并创建新的初始化器
                        GVPtr->setInitializer(newGVInit);                                       // 将初始化器的值赋值给全局变量
                        for(User *U : GVPtr->users()){
                            if(LoadInst *LI = dyn_cast<LoadInst>(U)){
                            // This is dummy Instruction so we can use replaceAllUsesWith
                            // without having to hand-craft our own implementation
                            // We will replace LHS later
                            Instruction* XORInst = BinaryOperator::CreateXor(XORKey, XORKey);
                            XORInst->insertAfter(LI);
                            LI->replaceAllUsesWith(XORInst);    //将所有LI output的use转为对XORInst output的use
                            XORInst->setOperand(0, LI);         //将第0个operand的值设置为LI的output

                            } else if(StoreInst *SI = dyn_cast<StoreInst>(U)) {
                                Instruction* XORInst = BinaryOperator::CreateXor(SI->getValueOperand(), XORKey);
                                XORInst->insertBefore(SI);
                                SI->replaceUsesOfWith(SI->getValueOperand(), XORInst);
                            }
                        }
                    }
                }
            }
            return true;
        }
    };

    /*
    ModulePass * createSimplePass() {
        return new Simple();
        }

    ModulePass * createSimplePass(bool flag) {
        return new Simple(flag);
        }
    */
}

//char Simple::ID = 0;
//INITIALIZE_PASS(Simple, "constenc", "Enable ConstantInt GV Encryption.", true, true)

char Simple::ID = 0;
static RegisterPass<Simple> X("Simple", "Obfuscating Pass");
//INITIALIZE_PASS(Simple, "Simple", "Enable ConstantInt GV Encryption.", true, true)