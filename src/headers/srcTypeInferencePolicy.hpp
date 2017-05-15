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
            };
            srcTypeInferenceData data;
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
                closeEventMap[ParserState::call] = [this](srcSAXEventContext& ctx){
                    std::cerr<<"enter call"<<std::endl;
                };
                closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx){
                    if(ctx.IsOpen(ParserState::name) && ctx.IsGreaterThan(ParserState::call,ParserState::argumentlist) && ctx.IsClosed(ParserState::genericargumentlist)){
                        data.name = ctx.currentToken;
                        auto foo = dictionary->FindFunction(data.name);
                        std::cerr<<foo.front().returnType<<std::endl;
                    }
                };
            }
    };
};
#endif
