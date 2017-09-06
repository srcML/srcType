/**
 * @file SideEffectPolicy.hpp
 *
 * @copyright Copyright (C) 2013-2014 SDML (www.srcML.org)
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
#include <srcSAXEventDispatcher.hpp>
#include <srcSAXHandler.hpp>
#include <exception>
#include <set>
#include <vector>
#include <srcType.hpp>
#ifndef SIDEEFFECTPOLICY
#define SIDEEFFECTPOLICY
class SideEffectPolicy : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener {
    public:
        ~SideEffectPolicy(){}
        SideEffectPolicy(srcTypeNS::srcType* dict, std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners = {}): srcSAXEventDispatch::PolicyDispatcher(listeners){
            dictionary = dict;
            seenAssignment = false;
            InitializeEventHandlers();
        }
        void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {} //doesn't use other parsers
    protected:
        void * DataInner() const override {return (void*)0;}
    private:
        srcTypeNS::srcType* dictionary;
        std::string currentFunctionName, currentExprName, currentModifier, currentSpecifier;
        bool seenAssignment;
        void InitializeEventHandlers(){
            using namespace srcSAXEventDispatch;
            closeEventMap[ParserState::op] = [this](srcSAXEventContext& ctx){
                if(ctx.currentToken == "="){
                    seenAssignment = true;
                }
            };

            closeEventMap[ParserState::name] = [this](srcSAXEventContext& ctx){
                if(ctx.IsOpen(ParserState::exprstmt)){
                   std::cerr<<currentExprName<<" "<<currentFunctionName<<" "<<ctx.currentFilePath<<std::endl;
                   auto test =dictionary->FindIdentifier(currentExprName, currentFunctionName, ctx.currentFilePath);
                   std::cerr<<"EXPRNAME: "<<currentExprName<<std::endl;
                }
            };

            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx){
                //TODO: possibly, this if-statement is suppressing more than just unmarked whitespace. Investigate.
                if(!(ctx.currentToken.empty() || ctx.currentToken == " ")){
                    if(ctx.And({ParserState::name, ParserState::expr, ParserState::exprstmt}) && ctx.Nor({ParserState::specifier, ParserState::modifier, ParserState::op})){
                        currentExprName = ctx.currentToken;
                    }
                }
                if(ctx.And({ParserState::name, ParserState::function}) && ctx.Nor({ParserState::functionblock, ParserState::type, ParserState::parameterlist, ParserState::genericargumentlist})){
                    currentFunctionName = ctx.currentToken;
                }
            };
            closeEventMap[ParserState::function] = [this](srcSAXEventContext& ctx){
                currentFunctionName.clear();
            };
            closeEventMap[ParserState::exprstmt] = [this](srcSAXEventContext& ctx){
                NotifyAll(ctx);
                seenAssignment = false;
            };

        }
};
#endif