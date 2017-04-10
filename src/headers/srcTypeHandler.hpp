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
                std::unordered_map<std::string, ParamTypePolicy::ParamData> paramMap;
                std::unordered_map<std::string, DeclTypePolicy::DeclTypeData> variableMap;
                std::unordered_map<std::string, FunctionSignaturePolicy::SignatureData> functionMap;
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
                    decldata = *policy->Data<DeclTypePolicy::DeclTypeData>();
                    //May want to get rid of line number and store right side of map as a set or something
                    std::cerr<<"Hashing on: "<<ctx.currentFilePath + functionsigdata.name + decldata.nameofidentifier + std::to_string(decldata.linenumber);
                    srctypedata.variableMap.insert(std::make_pair(ctx.currentFilePath + functionsigdata.name + decldata.nameofidentifier + std::to_string(decldata.linenumber), decldata));
                }else if(ctx.IsClosed(ParserState::declstmt)){
                    functionsigdata = *policy->Data<FunctionSignaturePolicy::SignatureData>();
                    std::string paramhash;
                    for(auto param : functionsigdata.parameters){
                        srctypedata.paramMap.insert(std::make_pair(ctx.currentFilePath + functionsigdata.name + param.nameofidentifier, param));
                        paramhash += param.nameoftype;
                    }
                    std::string fullhash = functionsigdata.name + paramhash + std::to_string(functionsigdata.isConst);
                    std::cerr<<"Hashing on: "<<fullhash<<std::endl;
                    srctypedata.functionMap.insert(std::make_pair(fullhash, functionsigdata));
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
                openEventMap[ParserState::classn] = [this](srcSAXEventContext& ctx){
                    std::cerr<<"beg class"<<std::endl;
                };
                openEventMap[ParserState::function] = [this](srcSAXEventContext& ctx){
                    std::cerr<<"add func"<<std::endl;
                    ctx.dispatcher->AddListenerDispatch(&functionpolicy);
                };
                openEventMap[ParserState::declstmt] = [this](srcSAXEventContext& ctx){
                    std::cerr<<"add decl"<<std::endl;
                    ctx.dispatcher->AddListenerDispatch(&declpolicy);
                };
                closeEventMap[ParserState::declstmt] = [this](srcSAXEventContext& ctx){
                    std::cerr<<"Rem decl"<<std::endl;
                    ctx.dispatcher->RemoveListenerDispatch(&declpolicy);
                };
                openEventMap[ParserState::functionblock] = [this](srcSAXEventContext& ctx){
                    std::cerr<<"Reem func"<<std::endl;
                    ctx.dispatcher->RemoveListenerDispatch(&functionpolicy);
                };
                closeEventMap[ParserState::classn] = [this](srcSAXEventContext& ctx){
                    std::cerr<<"end class"<<std::endl;
                };
            }
    };
};
#endif
