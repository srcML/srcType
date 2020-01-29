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
#ifndef SRCTYPEINCLUDE
#define SRCTYPEINCLUDE
#include <exception>
#include <unordered_map>
#include <srcSAXHandler.hpp>
#include <srcTypeProfile.hpp>
#include <DeclTypePolicy.hpp>
#include <srcSAXSingleEventDispatcher.hpp>
#include <FunctionSignaturePolicy.hpp>
#include <typeinfo>
namespace srcTypeNS{
    struct srcTypeData{
        std::unordered_map<std::string, std::vector<DeclData>> paramMap;
        std::unordered_map<std::string, std::vector<DeclData>> variableMap;
        std::unordered_map<std::string, std::vector<SignatureData>> functionMap;
    };
    class srcTypePolicy : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener 
    {
        public:
            srcTypePolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener*> listeners = {}) : srcSAXEventDispatch::PolicyDispatcher(listeners)
            {
                InitializeEventHandlers();
                declpolicy.AddListener(this);
                functionpolicy.AddListener(this);
                funcSigIt = srctypedata.functionMap.end();
            }
            void NotifyWrite(const PolicyDispatcher *policy, srcSAXEventDispatch::srcSAXEventContext &ctx) override {}
            void Notify(const PolicyDispatcher *policy, const srcSAXEventDispatch::srcSAXEventContext &ctx) override {
                using namespace srcSAXEventDispatch;
                if(typeid(DeclTypePolicy) == typeid(*policy)){
                    //Grab data
                    decldata = *policy->Data<DeclData>();
                    //If we have seen it before, add it to currently existing entry. Otherwise, make a new one.
                    auto declCheck = srctypedata.variableMap.find(decldata.nameOfIdentifier + std::to_string(decldata.lineNumber));
                    if(declCheck == srctypedata.variableMap.end()){
                        if(funcSigIt != srctypedata.functionMap.end()){
                            decldata.numOfContainingFunctionParams = funcSigIt->second.at(0).parameters.size();
                        }
                        std::vector<DeclData> decldatavec = {decldata};
                        srctypedata.variableMap.insert(std::make_pair(decldata.nameOfIdentifier+std::to_string(decldata.lineNumber), decldatavec));
                    }else{
                        declCheck->second.push_back(decldata);
                    }
                }else if(typeid(FunctionSignaturePolicy) == typeid(*policy)){
                    //Grab data
                    functionsigdata = *policy->Data<SignatureData>();

                    //If we have seen it before, add it to currently existing entry. Otherwise, make a new one.
                    auto functionCheck = srctypedata.functionMap.find(functionsigdata.name);
                    if(functionCheck == srctypedata.functionMap.end()){
                        if(!functionsigdata.name.empty()){
                            std::vector<SignatureData> functionsigdatavec = {functionsigdata};
                            funcSigIt = srctypedata.functionMap.insert(std::make_pair(functionsigdata.name+std::to_string(functionsigdata.lineNumber), functionsigdatavec)).first;
                        }
                    }else{
                        funcSigIt = functionCheck;
                        functionCheck->second.push_back(functionsigdata);
                    }

                    std::string paramhash;
                    
                    for(auto param : functionsigdata.parameters){
                        //If we have seen it before, add it to currently existing entry. Otherwise, make a new one.
                        auto paramCheck = srctypedata.paramMap.find(param.nameOfIdentifier + std::to_string(param.lineNumber));
                        if(paramCheck == srctypedata.paramMap.end()){
                            if(funcSigIt != srctypedata.functionMap.end()){
                                param.numOfContainingFunctionParams = funcSigIt->second.at(0).parameters.size();
                            }
                            std::vector<DeclData> paramdatavec = {param};
                            srctypedata.paramMap.insert(std::make_pair(param.nameOfIdentifier + std::to_string(param.lineNumber), paramdatavec));
                        }else{
                            paramCheck->second.push_back(param);
                        }
                        paramhash += param.nameOfIdentifier;
                    }
                }else{
                    std::cerr<<"error"<<std::endl;
                }
            }
            srcTypeData GetDictionary()const {
                return srctypedata;
            }
        protected:
            void *DataInner() const override {
                return new srcTypeData(srctypedata);
            }
            
        private:
            int currentscopelevel;
            bool inClass;

            srcTypeData srctypedata;
            
            std::unordered_map<std::string, std::vector<SignatureData>>::iterator funcSigIt;

            DeclTypePolicy declpolicy;
            DeclData decldata;

            FunctionSignaturePolicy functionpolicy;
            SignatureData functionsigdata;
            
            void InitializeEventHandlers(){
                using namespace srcSAXEventDispatch;
               // openEventMap[ParserState::classn] = [this](srcSAXEventContext& ctx){};
                openEventMap[ParserState::function] = [this](srcSAXEventContext& ctx){
                    ctx.dispatcher->AddListenerDispatch(&functionpolicy);
                };
                openEventMap[ParserState::constructor] = [this](srcSAXEventContext& ctx){
                    ctx.dispatcher->AddListenerDispatch(&functionpolicy);
                };
                openEventMap[ParserState::destructor] = [this](srcSAXEventContext& ctx){
                    ctx.dispatcher->AddListenerDispatch(&functionpolicy);
                };
                openEventMap[ParserState::declstmt] = [this](srcSAXEventContext& ctx){
                    ctx.dispatcher->AddListenerDispatch(&declpolicy);
                };
                closeEventMap[ParserState::declstmt] = [this](srcSAXEventContext& ctx){
                    ctx.dispatcher->RemoveListenerDispatch(&declpolicy);
                };
                openEventMap[ParserState::functionblock] = [this](srcSAXEventContext& ctx){
                    ctx.dispatcher->RemoveListenerDispatch(&functionpolicy);
                };
                closeEventMap[ParserState::function] = [this](srcSAXEventContext& ctx){
                    funcSigIt = srctypedata.functionMap.end();
                };
                closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx){};
                //closeEventMap[ParserState::classn] = [this](srcSAXEventContext& ctx){};
            }
    };
};
#endif
