// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__AutoNRF

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "DataReader.h"
#include "Functions.h"
#include "Efficiency.h"
#include "Flux.h"
#include "Output.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *Functions_Dictionary();
   static void Functions_TClassManip(TClass*);
   static void *new_Functions(void *p = 0);
   static void *newArray_Functions(Long_t size, void *p);
   static void delete_Functions(void *p);
   static void deleteArray_Functions(void *p);
   static void destruct_Functions(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Functions*)
   {
      ::Functions *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::Functions));
      static ::ROOT::TGenericClassInfo 
         instance("Functions", "Functions.h", 28,
                  typeid(::Functions), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &Functions_Dictionary, isa_proxy, 4,
                  sizeof(::Functions) );
      instance.SetNew(&new_Functions);
      instance.SetNewArray(&newArray_Functions);
      instance.SetDelete(&delete_Functions);
      instance.SetDeleteArray(&deleteArray_Functions);
      instance.SetDestructor(&destruct_Functions);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Functions*)
   {
      return GenerateInitInstanceLocal((::Functions*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::Functions*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *Functions_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::Functions*)0x0)->GetClass();
      Functions_TClassManip(theClass);
   return theClass;
   }

   static void Functions_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *FluxFitFunc_Dictionary();
   static void FluxFitFunc_TClassManip(TClass*);
   static void *new_FluxFitFunc(void *p = 0);
   static void *newArray_FluxFitFunc(Long_t size, void *p);
   static void delete_FluxFitFunc(void *p);
   static void deleteArray_FluxFitFunc(void *p);
   static void destruct_FluxFitFunc(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::FluxFitFunc*)
   {
      ::FluxFitFunc *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::FluxFitFunc));
      static ::ROOT::TGenericClassInfo 
         instance("FluxFitFunc", "Functions.h", 57,
                  typeid(::FluxFitFunc), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &FluxFitFunc_Dictionary, isa_proxy, 4,
                  sizeof(::FluxFitFunc) );
      instance.SetNew(&new_FluxFitFunc);
      instance.SetNewArray(&newArray_FluxFitFunc);
      instance.SetDelete(&delete_FluxFitFunc);
      instance.SetDeleteArray(&deleteArray_FluxFitFunc);
      instance.SetDestructor(&destruct_FluxFitFunc);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::FluxFitFunc*)
   {
      return GenerateInitInstanceLocal((::FluxFitFunc*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::FluxFitFunc*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *FluxFitFunc_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::FluxFitFunc*)0x0)->GetClass();
      FluxFitFunc_TClassManip(theClass);
   return theClass;
   }

   static void FluxFitFunc_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *Output_Dictionary();
   static void Output_TClassManip(TClass*);
   static void *new_Output(void *p = 0);
   static void *newArray_Output(Long_t size, void *p);
   static void delete_Output(void *p);
   static void deleteArray_Output(void *p);
   static void destruct_Output(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Output*)
   {
      ::Output *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::Output));
      static ::ROOT::TGenericClassInfo 
         instance("Output", "Output.h", 29,
                  typeid(::Output), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &Output_Dictionary, isa_proxy, 4,
                  sizeof(::Output) );
      instance.SetNew(&new_Output);
      instance.SetNewArray(&newArray_Output);
      instance.SetDelete(&delete_Output);
      instance.SetDeleteArray(&deleteArray_Output);
      instance.SetDestructor(&destruct_Output);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Output*)
   {
      return GenerateInitInstanceLocal((::Output*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::Output*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *Output_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::Output*)0x0)->GetClass();
      Output_TClassManip(theClass);
   return theClass;
   }

   static void Output_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *Flux_Dictionary();
   static void Flux_TClassManip(TClass*);
   static void *new_Flux(void *p = 0);
   static void *newArray_Flux(Long_t size, void *p);
   static void delete_Flux(void *p);
   static void deleteArray_Flux(void *p);
   static void destruct_Flux(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Flux*)
   {
      ::Flux *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::Flux));
      static ::ROOT::TGenericClassInfo 
         instance("Flux", "Flux.h", 91,
                  typeid(::Flux), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &Flux_Dictionary, isa_proxy, 4,
                  sizeof(::Flux) );
      instance.SetNew(&new_Flux);
      instance.SetNewArray(&newArray_Flux);
      instance.SetDelete(&delete_Flux);
      instance.SetDeleteArray(&deleteArray_Flux);
      instance.SetDestructor(&destruct_Flux);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Flux*)
   {
      return GenerateInitInstanceLocal((::Flux*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::Flux*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *Flux_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::Flux*)0x0)->GetClass();
      Flux_TClassManip(theClass);
   return theClass;
   }

   static void Flux_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_Functions(void *p) {
      return  p ? new(p) ::Functions : new ::Functions;
   }
   static void *newArray_Functions(Long_t nElements, void *p) {
      return p ? new(p) ::Functions[nElements] : new ::Functions[nElements];
   }
   // Wrapper around operator delete
   static void delete_Functions(void *p) {
      delete ((::Functions*)p);
   }
   static void deleteArray_Functions(void *p) {
      delete [] ((::Functions*)p);
   }
   static void destruct_Functions(void *p) {
      typedef ::Functions current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Functions

namespace ROOT {
   // Wrappers around operator new
   static void *new_FluxFitFunc(void *p) {
      return  p ? new(p) ::FluxFitFunc : new ::FluxFitFunc;
   }
   static void *newArray_FluxFitFunc(Long_t nElements, void *p) {
      return p ? new(p) ::FluxFitFunc[nElements] : new ::FluxFitFunc[nElements];
   }
   // Wrapper around operator delete
   static void delete_FluxFitFunc(void *p) {
      delete ((::FluxFitFunc*)p);
   }
   static void deleteArray_FluxFitFunc(void *p) {
      delete [] ((::FluxFitFunc*)p);
   }
   static void destruct_FluxFitFunc(void *p) {
      typedef ::FluxFitFunc current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::FluxFitFunc

namespace ROOT {
   // Wrappers around operator new
   static void *new_Output(void *p) {
      return  p ? new(p) ::Output : new ::Output;
   }
   static void *newArray_Output(Long_t nElements, void *p) {
      return p ? new(p) ::Output[nElements] : new ::Output[nElements];
   }
   // Wrapper around operator delete
   static void delete_Output(void *p) {
      delete ((::Output*)p);
   }
   static void deleteArray_Output(void *p) {
      delete [] ((::Output*)p);
   }
   static void destruct_Output(void *p) {
      typedef ::Output current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Output

namespace ROOT {
   // Wrappers around operator new
   static void *new_Flux(void *p) {
      return  p ? new(p) ::Flux : new ::Flux;
   }
   static void *newArray_Flux(Long_t nElements, void *p) {
      return p ? new(p) ::Flux[nElements] : new ::Flux[nElements];
   }
   // Wrapper around operator delete
   static void delete_Flux(void *p) {
      delete ((::Flux*)p);
   }
   static void deleteArray_Flux(void *p) {
      delete [] ((::Flux*)p);
   }
   static void destruct_Flux(void *p) {
      typedef ::Flux current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Flux

namespace ROOT {
   static TClass *vectorlEvectorlEvectorlEdoublegRsPgRsPgR_Dictionary();
   static void vectorlEvectorlEvectorlEdoublegRsPgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEvectorlEdoublegRsPgRsPgR(void *p = 0);
   static void *newArray_vectorlEvectorlEvectorlEdoublegRsPgRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEvectorlEdoublegRsPgRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEvectorlEdoublegRsPgRsPgR(void *p);
   static void destruct_vectorlEvectorlEvectorlEdoublegRsPgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<vector<double> > >*)
   {
      vector<vector<vector<double> > > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<vector<double> > >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<vector<double> > >", -2, "vector", 214,
                  typeid(vector<vector<vector<double> > >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEvectorlEdoublegRsPgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<vector<double> > >) );
      instance.SetNew(&new_vectorlEvectorlEvectorlEdoublegRsPgRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEvectorlEdoublegRsPgRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEvectorlEdoublegRsPgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEvectorlEdoublegRsPgRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEvectorlEdoublegRsPgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<vector<double> > > >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<vector<double> > >*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEvectorlEdoublegRsPgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<vector<double> > >*)0x0)->GetClass();
      vectorlEvectorlEvectorlEdoublegRsPgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEvectorlEdoublegRsPgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEvectorlEdoublegRsPgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<double> > > : new vector<vector<vector<double> > >;
   }
   static void *newArray_vectorlEvectorlEvectorlEdoublegRsPgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<vector<double> > >[nElements] : new vector<vector<vector<double> > >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEvectorlEdoublegRsPgRsPgR(void *p) {
      delete ((vector<vector<vector<double> > >*)p);
   }
   static void deleteArray_vectorlEvectorlEvectorlEdoublegRsPgRsPgR(void *p) {
      delete [] ((vector<vector<vector<double> > >*)p);
   }
   static void destruct_vectorlEvectorlEvectorlEdoublegRsPgRsPgR(void *p) {
      typedef vector<vector<vector<double> > > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<vector<double> > >

namespace ROOT {
   static TClass *vectorlEvectorlEdoublegRsPgR_Dictionary();
   static void vectorlEvectorlEdoublegRsPgR_TClassManip(TClass*);
   static void *new_vectorlEvectorlEdoublegRsPgR(void *p = 0);
   static void *newArray_vectorlEvectorlEdoublegRsPgR(Long_t size, void *p);
   static void delete_vectorlEvectorlEdoublegRsPgR(void *p);
   static void deleteArray_vectorlEvectorlEdoublegRsPgR(void *p);
   static void destruct_vectorlEvectorlEdoublegRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<vector<double> >*)
   {
      vector<vector<double> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<vector<double> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<vector<double> >", -2, "vector", 214,
                  typeid(vector<vector<double> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEvectorlEdoublegRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<vector<double> >) );
      instance.SetNew(&new_vectorlEvectorlEdoublegRsPgR);
      instance.SetNewArray(&newArray_vectorlEvectorlEdoublegRsPgR);
      instance.SetDelete(&delete_vectorlEvectorlEdoublegRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEvectorlEdoublegRsPgR);
      instance.SetDestructor(&destruct_vectorlEvectorlEdoublegRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<vector<double> > >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<vector<double> >*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEvectorlEdoublegRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<vector<double> >*)0x0)->GetClass();
      vectorlEvectorlEdoublegRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEvectorlEdoublegRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEvectorlEdoublegRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<double> > : new vector<vector<double> >;
   }
   static void *newArray_vectorlEvectorlEdoublegRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<vector<double> >[nElements] : new vector<vector<double> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEvectorlEdoublegRsPgR(void *p) {
      delete ((vector<vector<double> >*)p);
   }
   static void deleteArray_vectorlEvectorlEdoublegRsPgR(void *p) {
      delete [] ((vector<vector<double> >*)p);
   }
   static void destruct_vectorlEvectorlEdoublegRsPgR(void *p) {
      typedef vector<vector<double> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<vector<double> >

namespace ROOT {
   static TClass *vectorlEdoublegR_Dictionary();
   static void vectorlEdoublegR_TClassManip(TClass*);
   static void *new_vectorlEdoublegR(void *p = 0);
   static void *newArray_vectorlEdoublegR(Long_t size, void *p);
   static void delete_vectorlEdoublegR(void *p);
   static void deleteArray_vectorlEdoublegR(void *p);
   static void destruct_vectorlEdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<double>*)
   {
      vector<double> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<double>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<double>", -2, "vector", 214,
                  typeid(vector<double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEdoublegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<double>) );
      instance.SetNew(&new_vectorlEdoublegR);
      instance.SetNewArray(&newArray_vectorlEdoublegR);
      instance.SetDelete(&delete_vectorlEdoublegR);
      instance.SetDeleteArray(&deleteArray_vectorlEdoublegR);
      instance.SetDestructor(&destruct_vectorlEdoublegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<double> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<double>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<double>*)0x0)->GetClass();
      vectorlEdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEdoublegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double> : new vector<double>;
   }
   static void *newArray_vectorlEdoublegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double>[nElements] : new vector<double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEdoublegR(void *p) {
      delete ((vector<double>*)p);
   }
   static void deleteArray_vectorlEdoublegR(void *p) {
      delete [] ((vector<double>*)p);
   }
   static void destruct_vectorlEdoublegR(void *p) {
      typedef vector<double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<double>

namespace ROOT {
   static TClass *vectorlEboolgR_Dictionary();
   static void vectorlEboolgR_TClassManip(TClass*);
   static void *new_vectorlEboolgR(void *p = 0);
   static void *newArray_vectorlEboolgR(Long_t size, void *p);
   static void delete_vectorlEboolgR(void *p);
   static void deleteArray_vectorlEboolgR(void *p);
   static void destruct_vectorlEboolgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<bool>*)
   {
      vector<bool> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<bool>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<bool>", -2, "vector", 541,
                  typeid(vector<bool>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEboolgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<bool>) );
      instance.SetNew(&new_vectorlEboolgR);
      instance.SetNewArray(&newArray_vectorlEboolgR);
      instance.SetDelete(&delete_vectorlEboolgR);
      instance.SetDeleteArray(&deleteArray_vectorlEboolgR);
      instance.SetDestructor(&destruct_vectorlEboolgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<bool> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<bool>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEboolgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<bool>*)0x0)->GetClass();
      vectorlEboolgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEboolgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEboolgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<bool> : new vector<bool>;
   }
   static void *newArray_vectorlEboolgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<bool>[nElements] : new vector<bool>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEboolgR(void *p) {
      delete ((vector<bool>*)p);
   }
   static void deleteArray_vectorlEboolgR(void *p) {
      delete [] ((vector<bool>*)p);
   }
   static void destruct_vectorlEboolgR(void *p) {
      typedef vector<bool> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<bool>

namespace ROOT {
   static TClass *vectorlETThreadmUgR_Dictionary();
   static void vectorlETThreadmUgR_TClassManip(TClass*);
   static void *new_vectorlETThreadmUgR(void *p = 0);
   static void *newArray_vectorlETThreadmUgR(Long_t size, void *p);
   static void delete_vectorlETThreadmUgR(void *p);
   static void deleteArray_vectorlETThreadmUgR(void *p);
   static void destruct_vectorlETThreadmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TThread*>*)
   {
      vector<TThread*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TThread*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TThread*>", -2, "vector", 214,
                  typeid(vector<TThread*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETThreadmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TThread*>) );
      instance.SetNew(&new_vectorlETThreadmUgR);
      instance.SetNewArray(&newArray_vectorlETThreadmUgR);
      instance.SetDelete(&delete_vectorlETThreadmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlETThreadmUgR);
      instance.SetDestructor(&destruct_vectorlETThreadmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TThread*> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TThread*>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETThreadmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TThread*>*)0x0)->GetClass();
      vectorlETThreadmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETThreadmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETThreadmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TThread*> : new vector<TThread*>;
   }
   static void *newArray_vectorlETThreadmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TThread*>[nElements] : new vector<TThread*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETThreadmUgR(void *p) {
      delete ((vector<TThread*>*)p);
   }
   static void deleteArray_vectorlETThreadmUgR(void *p) {
      delete [] ((vector<TThread*>*)p);
   }
   static void destruct_vectorlETThreadmUgR(void *p) {
      typedef vector<TThread*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TThread*>

namespace {
  void TriggerDictionaryInitialization_G__AutoNRF_Impl() {
    static const char* headers[] = {
"DataReader.h",
"Functions.h",
"Efficiency.h",
"Flux.h",
"Output.h",
0
    };
    static const char* includePaths[] = {
"/usr/include",
"/usr/local/include",
"/home/mschilling/Schreibtisch/PHD/NRF_cpp/AutoNRF/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "G__AutoNRF dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$Functions.h")))  __attribute__((annotate("$clingAutoload$DataReader.h")))  Functions;
class __attribute__((annotate("$clingAutoload$Functions.h")))  __attribute__((annotate("$clingAutoload$DataReader.h")))  FluxFitFunc;
class __attribute__((annotate("$clingAutoload$Output.h")))  __attribute__((annotate("$clingAutoload$Efficiency.h")))  Output;
class __attribute__((annotate("$clingAutoload$Flux.h")))  Flux;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "G__AutoNRF dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "DataReader.h"
#include "Functions.h"
#include "Efficiency.h"
#include "Flux.h"
#include "Output.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"Flux", payloadCode, "@",
"FluxFitFunc", payloadCode, "@",
"Functions", payloadCode, "@",
"Output", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("G__AutoNRF",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_G__AutoNRF_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_G__AutoNRF_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_G__AutoNRF() {
  TriggerDictionaryInitialization_G__AutoNRF_Impl();
}
