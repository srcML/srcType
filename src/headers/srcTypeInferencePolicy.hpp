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
    class srcTypeInferencePolicy : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener {
        public:
            std::list<CallStackFrame> callStack;
            std::list<srcTypeInferenceData> data;
            std::vector<DeclData> currentParameters;
            std::vector<std::string> operatorStack;
            std::string currentFunctionName, argumentexpr, currentAttr, currentAttrType;
            srcType * const dictionary;
            xmlBufferPtr archiveBuffer;

            srcTypeInferencePolicy(srcType* const data, std::initializer_list<srcSAXEventDispatch::PolicyListener*> listeners = {}) : srcSAXEventDispatch::PolicyDispatcher(listeners), dictionary(data), archiveBuffer(nullptr){
                InitializeEventHandlers();
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
                closeEventMap[ParserState::archive] = [this](srcSAXEventContext& ctx){
                    archiveBuffer = ctx.archiveBuffer;
                };
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
                            auto var = dictionary->FindIdentifier(argumentexpr, currentFunctionName, "","testsrcType.cpp");
                            currentParameters.push_back(var.at(0));
                            callStack.back().parameters.push_back(var.at(0).nameOfType);
                        }catch(std::runtime_error e){
                            std::cerr<<e.what();
                            if(currentAttrType == "type"){
                                std::cerr<<"Oh, it's a "<<currentAttr<<". Repairing and marking as a "<<currentAttr<<"."<<std::endl;
                                callStack.back().parameters.push_back(currentAttr);
                            }else{
                                callStack.back().parameters.push_back("unresolved");
                            }
                        }
                        argumentexpr.clear();
                    }
                };
                closeEventMap[ParserState::xmlattribute] = [this](srcSAXEventContext& ctx){
                    if(ctx.currentAttributeName == "type"){
                        currentAttr = ctx.currentAttributeValue;
                        currentAttrType = ctx.currentAttributeName;
                    }
                };
                closeEventMap[ParserState::call] = [this](srcSAXEventContext& ctx){
                    if(ctx.IsClosed(ParserState::genericargumentlist)){
                        const unsigned int ONLY_ONE_FUNCTION_IN_RESULT = 1;
                        const unsigned int NESTED_CALL_RESOLVED = 1;

                        auto filteredFunctionList = dictionary->FindFunction(callStack.back().callName, callStack.back().parameters);
                        for(auto function : filteredFunctionList){
                            data.push_back(srcTypeInferenceData(function.second.name, function.second.returnType));
                        }

                        if(ctx.NumCurrentlyOpen(ParserState::call) > 1){ //Have to be in some nested call
                            callStack.pop_back(); //pop current, resolved call
                            callStack.back().parameters.push_back(data.back().type); //Add resolved call's return type to param list
                        }
                    }

                    //write the return type into srcML archive at the call site
                    xmlTextWriterStartElementNS(ctx.writer, (const xmlChar*)"src",(const xmlChar*)"rtype", (const xmlChar*)"http://www.srcML.org/srcML/src");
                    ctx.write_content(data.back().type);
                    xmlTextWriterEndElement(ctx.writer);
                };
                /*Parsing calls have three cases. 
                  1. call(name*)
                  2. call(prefixCall()+)
                  3. call(infixCall())
                  The first case is the simplest and only requires a lookup of name. The second case requires prefixCall
                  and any call within its arguments to be handled before call is handled. In this case, we should push a
                  call frame onto the stack and begin a new frame to handle this call. Once the call is finished, pop the
                  stack and take the return type and use it in the previous call frame. The third case is the nastiest.
                  Infix calls are C++ operators and not very parse-friendly. For the time being, we will skip arguments
                  that use infix and instead take a guess at what they are based on surrounding data.
                */
                closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx){
                    if(ctx.And({ParserState::name, ParserState::function}) && ctx.Nor({ParserState::functionblock, ParserState::type, ParserState::parameterlist, ParserState::genericargumentlist})){
                        currentFunctionName = ctx.currentToken;
                    }
                    if(ctx.And({ParserState::name, ParserState::call}) && ctx.IsClosed(ParserState::genericargumentlist)){
                        if(ctx.IsGreaterThan(ParserState::call, ParserState::argumentlist)){
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
