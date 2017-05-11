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
#include <srcSAXEventDispatcher.hpp>
#include <FunctionSignaturePolicy.hpp>
namespace srcTypeNS{
    class srcTypePolicy : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener 
    {
        public:
            struct srcTypeData{
                std::unordered_map<std::string, std::vector<ParamTypePolicy::ParamData>> paramMap;
                std::unordered_map<std::string, std::vector<DeclTypePolicy::DeclTypeData>> variableMap;
                std::unordered_map<std::string, std::vector<FunctionSignaturePolicy::SignatureData>> functionMap;
            };
            srcTypePolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener*> listeners = {}) : srcSAXEventDispatch::PolicyDispatcher(listeners)
            {
                InitializeEventHandlers();
                declpolicy.AddListener(this);
                functionpolicy.AddListener(this);
            }
    
            void Notify(const PolicyDispatcher *policy, const srcSAXEventDispatch::srcSAXEventContext &ctx) override {
                using namespace srcSAXEventDispatch;
                if(ctx.IsOpen(ParserState::declstmt)){
                    //Grab data
                    decldata = *policy->Data<DeclTypePolicy::DeclTypeData>();

                    //If we have seen it before, add it to currently existing entry. Otherwise, make a new one.
                    auto declCheck = srctypedata.variableMap.find(ctx.currentFilePath + functionsigdata.name + decldata.nameofidentifier);
                    if(declCheck == srctypedata.variableMap.end()){
                        std::vector<DeclTypePolicy::DeclTypeData> decldatavec = {decldata};
                        srctypedata.variableMap.insert(std::make_pair(ctx.currentFilePath + functionsigdata.name + decldata.nameofidentifier, decldatavec));
                    }else{
                        declCheck->second.push_back(decldata);
                    }
                }else if(ctx.IsClosed(ParserState::declstmt)){
                    //Grab data
                    functionsigdata = *policy->Data<FunctionSignaturePolicy::SignatureData>();
                    std::string paramhash;
                    
                    for(auto param : functionsigdata.parameters){
                        //If we have seen it before, add it to currently existing entry. Otherwise, make a new one.
                        auto paramCheck = srctypedata.paramMap.find(functionsigdata.name + param.nameoftype);
                        if(paramCheck == srctypedata.paramMap.end()){
                            std::vector<ParamTypePolicy::ParamData> paramdatavec = {param};
                            srctypedata.paramMap.insert(std::make_pair(functionsigdata.name + param.nameoftype, paramdatavec));
                        }else{
                            paramCheck->second.push_back(param);
                        }
                        paramhash += param.nameoftype;
                    }
                    
                    //Generate a string to be hashed for the function
                    std::string fullhash = functionsigdata.name + paramhash + std::to_string(functionsigdata.isConst);
                    
                    //If we have seen it before, add it to currently existing entry. Otherwise, make a new one.
                    auto functionCheck = srctypedata.functionMap.find(fullhash);
                    if(functionCheck == srctypedata.functionMap.end()){
                        std::vector<FunctionSignaturePolicy::SignatureData> functionsigdatavec = {functionsigdata};
                        srctypedata.functionMap.insert(std::make_pair(fullhash, functionsigdatavec));
                    }else{
                        functionCheck->second.push_back(functionsigdata);
                    }
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

            DeclTypePolicy declpolicy;
            DeclTypePolicy::DeclTypeData decldata;

            FunctionSignaturePolicy functionpolicy;
            FunctionSignaturePolicy::SignatureData functionsigdata;
            
            void InitializeEventHandlers(){
                using namespace srcSAXEventDispatch;
               // openEventMap[ParserState::classn] = [this](srcSAXEventContext& ctx){};
                openEventMap[ParserState::function] = [this](srcSAXEventContext& ctx){
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
                //closeEventMap[ParserState::classn] = [this](srcSAXEventContext& ctx){};
            }
    };
};
#endif
