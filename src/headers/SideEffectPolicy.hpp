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
            currentSig = nullptr;
            numParams = 0;
            InitializeEventHandlers();
        }
        int numParams;
        void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {} //doesn't use other parsers
    protected:
        void * DataInner() const override {return (void*)0;}
    private:
        srcTypeNS::srcType* dictionary;
        std::string currentExprName, currentDeclName;
        std::vector<DeclData>* currentDecl;
        std::vector<SignatureData>* currentSig;
        void InitializeEventHandlers(){
            using namespace srcSAXEventDispatch;
            openEventMap[ParserState::init] = [this](srcSAXEventContext& ctx){
                for(std::vector<DeclData>::iterator it = currentDecl->begin(); it!= currentDecl->end(); ++it){
                    it->hasSideEffect = true;
                }
            };
            closeEventMap[ParserState::op] = [this](srcSAXEventContext& ctx){
                //can't have side effect on class members
                if(ctx.currentToken == "="){
                    for(std::vector<DeclData>::iterator it = currentDecl->begin(); it!= currentDecl->end(); ++it){
                        if(!dictionary->IsPrimitive(it->nameOfType)){
                            it->hasSideEffect = true;
                            currentSig->at(0).hasSideEffect = true;
                        }
                        if(it->isClassMember){
                            currentSig->at(0).hasSideEffect = true;
                        }
                    }
                }
            };

            closeEventMap[ParserState::name] = [this](srcSAXEventContext& ctx){
                if(ctx.Or({ParserState::exprstmt, ParserState::declstmt}) && ctx.Nor({ParserState::type, ParserState::call})){
                    std::string currentName = currentExprName.empty() ? currentDeclName : currentExprName;
                    currentDecl = dictionary->FindIdentifierWrite(currentName, ctx.currentFunctionName, "", ctx.currentFilePath);
                    if(!currentDecl || currentDecl->empty()){
                        currentDecl = dictionary->FindIdentifierWrite(currentName, "", ctx.currentClassName, ctx.currentFilePath);
                    }
                    currentExprName.clear();
                    currentDeclName.clear();
                }

                //Can't return something with side effect
                if(ctx.IsOpen(ParserState::returnstmt)){
                    currentDecl = dictionary->FindIdentifierWrite(ctx.currentToken, ctx.currentFunctionName, "", ctx.currentFilePath);
                    if(!currentDecl || currentDecl->empty()){
                        currentDecl = dictionary->FindIdentifierWrite(ctx.currentToken, "", ctx.currentClassName, ctx.currentFilePath);
                    }
                    for(auto declit = currentDecl->begin(); declit != currentDecl->end(); ++declit){
                        if(declit->hasSideEffect){
                            currentSig->at(0).hasSideEffect = true;
                        }
                    }
                }
            };

            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx){
                //TODO: possibly, this if-statement is suppressing more than just unmarked whitespace. Investigate.
                if(!(ctx.currentToken.empty() || ctx.currentToken == " ")){
                    if(ctx.And({ParserState::name, ParserState::expr, ParserState::exprstmt}) && ctx.Nor({ParserState::specifier, ParserState::modifier, ParserState::op})){
                        currentExprName = ctx.currentToken;
                    }
                    if(ctx.And({ParserState::name, ParserState::decl, ParserState::declstmt}) &&
                       ctx.Nor({ParserState::type, ParserState::index/*skip array portion*/, ParserState::argumentlist/*skip init list portion*/, ParserState::init, ParserState::specifier, ParserState::modifier})){
                        currentDeclName = ctx.currentToken;
                    }
                    if(ctx.And({ParserState::name, ParserState::decl, ParserState::parameter}) && 
                        ctx.Nor({ParserState::type, ParserState::index/*skip array portion*/, ParserState::argumentlist/*skip init list portion*/, 
                        ParserState::init, ParserState::specifier, ParserState::modifier, ParserState::genericargumentlist})){
                        currentDeclName = ctx.currentToken;
                    }                
                }
            };
            closeEventMap[ParserState::parameter] = [this](srcSAXEventContext& ctx){
                ++numParams;
                currentDecl = dictionary->FindIdentifierWrite(currentDeclName, ctx.currentFunctionName, "", ctx.currentFilePath);
            };
            closeEventMap[ParserState::parameterlist] = [this](srcSAXEventContext& ctx){
                currentSig = dictionary->FindFunctionWrite(ctx.currentFunctionName, numParams);
                numParams = 0;
            };
            closeEventMap[ParserState::exprstmt] = [this](srcSAXEventContext& ctx){
                currentDecl = nullptr;
            };
            closeEventMap[ParserState::declstmt] = [this](srcSAXEventContext& ctx){
                currentDecl = nullptr;
            };
            closeEventMap[ParserState::function] = [this](srcSAXEventContext& ctx){
                currentSig = nullptr;
            };

        }
};
#endif