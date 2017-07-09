/**
 * @file srcTypeHandler.hpp
 *
 * @copyright Copyright (C) 2013-2014  SDML (www.srcML.org)
 *
 * The srcML Toolkit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML Toolkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef SRCTYPEINFERENCEINCLUDE
#define SRCTYPEINFERENCEINCLUDE
#include <exception>
#include <unordered_map>
#include <srcType.hpp>
#include <srcSAXEventDispatcher.hpp>
#include <FunctionCallPolicy.hpp>
namespace srcTypeNS{
    struct srcTypeInferenceData{
        srcTypeInferenceData(std::string nme, std::string rtype){
            name = nme;
            type = rtype;
        }
        std::string name;
        std::string type;
    };
    struct CallStackFrame{
        CallStackFrame(std::string name){
            callName = name;
        }
        std::string callName;
        std::vector<std::string> parameters;
    };
    class srcTypeInferencePolicy : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener 
    {
        public:
            std::list<CallStackFrame> callStack;
            std::list<srcTypeInferenceData> data;
            std::vector<DeclData> currentParameters;
            std::vector<std::string> operatorStack;
            std::string currentFunctionCall, currentFunctionName, argumentexpr;
            srcType * const dictionary;
            srcTypeInferencePolicy(srcType* const data, std::initializer_list<srcSAXEventDispatch::PolicyListener*> listeners = {}) : srcSAXEventDispatch::PolicyDispatcher(listeners), dictionary(data)
            {
                
                InitializeEventHandlers();
                //declpolicy.AddListener(this);
                //functionpolicy.AddListener(this);
            }
    
            void Notify(const PolicyDispatcher *policy, const srcSAXEventDispatch::srcSAXEventContext &ctx) override {
                using namespace srcSAXEventDispatch;
              
            }
        protected:
            void *DataInner() const override {
                return new std::list<srcTypeInferenceData>(data);
            }
            
        private:
            void InitializeEventHandlers(){
                using namespace srcSAXEventDispatch;
                
                closeEventMap[ParserState::argumentlist] = [this](srcSAXEventContext& ctx){
                    argumentexpr.clear();
                };

                openEventMap[ParserState::argument] = [this](srcSAXEventContext& ctx){
                    argumentexpr.clear();
                };
                /*End of argument. If no infix operator or parent function call (i.e., foo(bar())) was seen, then just look up
                the name.  If parent function call was seen */
                closeEventMap[ParserState::argument] = [this](srcSAXEventContext& ctx){
                    if(ctx.IsEqualTo(ParserState::call,ParserState::argumentlist) && ctx.IsClosed(ParserState::genericargumentlist) && !argumentexpr.empty()){
                        try{
                            std::cerr<<"Lookup id: "<<argumentexpr<<std::endl;
                            auto var = dictionary->FindIdentifier(argumentexpr, currentFunctionName, "testsrcType.cpp");
                            currentParameters.push_back(var.at(0));
                            callStack.back().parameters.push_back(var.at(0).nameoftype);
                        }catch(std::runtime_error e){
                            std::cerr<<e.what();
                        }
                        argumentexpr.clear();
                    }
                };

                openEventMap[ParserState::call] = [this](srcSAXEventContext& ctx){
                    std::cerr<<"PUSHING"<<std::endl;
                    
                };
                closeEventMap[ParserState::call] = [this](srcSAXEventContext& ctx){
                    std::cerr<<"END CALL"<<std::endl;
                    if(ctx.IsClosed(ParserState::genericargumentlist)){
                        const unsigned int ONLY_ONE_FUNCTION_IN_RESULT = 1;
                        const unsigned int NESTED_CALL_RESOLVED = 1;
                        std::cerr<<"Lookup: "<<callStack.size()<<callStack.back().callName<<std::endl;
                        auto filteredFunctionList = dictionary->FindFunction(callStack.back().callName, callStack.back().parameters);
                        for(auto function : filteredFunctionList){
                            data.push_back(srcTypeInferenceData(function.name, function.returnType));
                        }
                        if(data.size() == ONLY_ONE_FUNCTION_IN_RESULT && callStack.size() > NESTED_CALL_RESOLVED){
                            //FIX: Todo, pop call stack and record the return type of the last call in the current position
                            //of parameter list
                            std::cerr<<"POPPING: "<<data.back().type<<std::endl;
                            callStack.pop_back();
                            callStack.back().parameters.push_back(data.back().type);
                        }
                        
                        std::cerr<<filteredFunctionList.size();
                    }

                };
                closeEventMap[ParserState::op] = [this](srcSAXEventContext& ctx){
                    /*Parsing calls have three cases. 
                    1. call(name*)
                    2. call(prefixCall()+)
                    3. call(infixCall())
                    The first case is the simplest and only requires a lookup of name. The second case requires prefixCall
                    and any call within its arguments to be handled before call is handled. In this case, we should push a
                    call frame onto the stack and begin a new frame to handle this call. Once the call is finished, pop the
                    stack and take the return type and use it in the previous call frame. The third case is the nastiest.
                    Infix calls are C++ operators and not very parse-friendly. For the time being, we will skip arguments
                    that use infix and instead take a guess at what they are based on surrounding data.*/

                };
                closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx){
                    //std::cerr<<ctx.And({ParserState::name, ParserState::call})<<std::endl;
                    if(ctx.And({ParserState::name, ParserState::function}) && ctx.Nor({ParserState::functionblock, ParserState::type, ParserState::parameterlist, ParserState::genericargumentlist})){
                        std::cerr<<ctx.currentToken<<std::endl;
                        currentFunctionName = ctx.currentToken;
                    }
                    if(ctx.And({ParserState::name, ParserState::call}) && ctx.IsClosed(ParserState::genericargumentlist)){
                        if(ctx.IsGreaterThan(ParserState::call, ParserState::argumentlist)){
                            std::cerr<<"Assign: "<<ctx.currentToken<<std::endl;
                            currentFunctionCall = ctx.currentToken;
                            callStack.push_back(CallStackFrame(ctx.currentToken));
                        }
                    }
                    if(ctx.IsOpen(ParserState::argument) && ctx.IsClosed(ParserState::genericargumentlist)){
                        argumentexpr += ctx.currentToken;
                    }
                };
            }
    };
};
#endif
