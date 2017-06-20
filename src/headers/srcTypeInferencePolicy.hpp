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
    class srcTypeInferencePolicy : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener 
    {
        public:
            struct srcTypeInferenceData{
                std::string name;
                std::vector<std::string> argumentTypes;
            };
            srcTypeInferenceData data;
            std::vector<ParamTypePolicy::ParamData> currentParameters;
            std::string currentFunction;
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
                //return new srcTypeData(srctypedata);
            }
            
        private:

            
            void InitializeEventHandlers(){
                using namespace srcSAXEventDispatch;
                closeEventMap[ParserState::argument] = [this](srcSAXEventContext& ctx){
                    if(ctx.IsEqualTo(ParserState::call,ParserState::argumentlist) && ctx.IsClosed(ParserState::genericargumentlist)){
                    try{
                        auto var = dictionary->FindParam(data.name, currentFunction);
                        currentParameters.push_back(var.at(0));

                    }catch(std::runtime_error e){
                        std::cerr<<e.what();
                    }
                }
                };
                closeEventMap[ParserState::call] = [this](srcSAXEventContext& ctx){
                    if(ctx.IsGreaterThan(ParserState::call,ParserState::argumentlist) && ctx.IsClosed(ParserState::genericargumentlist)){
                        std::string hash;
                        for(auto param : currentParameters){
                            hash += param.nameoftype;
                        }
                    }
                };
                closeEventMap[ParserState::call] = [this](srcSAXEventContext& ctx){
                    if(ctx.IsClosed(ParserState::genericargumentlist)){
                        std::cerr<<"Found: "<<currentFunction<<" "<<ctx.currentToken<<std::endl;
                        auto func = dictionary->FindFunction(currentFunction, currentParameters.size());
                        std::cerr<<func.size();
                    }
                };
                closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx){
                    //std::cerr<<ctx.And({ParserState::name, ParserState::call})<<std::endl;
                    if(ctx.And({ParserState::name, ParserState::call}) && ctx.Nand({ParserState::genericargumentlist, ParserState::argumentlist})){
                        std::cerr<<"New func call" << ctx.currentToken<<std::endl;
                        currentFunction = ctx.currentToken;
                    }
                    if(ctx.And({ParserState::name, ParserState::argument}) && ctx.IsClosed(ParserState::genericargumentlist)){
                        std::cerr<<"data: "<<data.name<<std::endl;
                        data.name = ctx.currentToken;
                    }
                };
            }
    };
};
#endif
