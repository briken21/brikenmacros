// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__DataStruct
#define R__NO_DEPRECATION

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

// The generated code does not explicitly qualifies STL entities
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "/home/maxpallas/data/briken2021/TestScript/online1_he3/include/libDataStruct.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *BrikenTreeData_Dictionary();
   static void BrikenTreeData_TClassManip(TClass*);
   static void *new_BrikenTreeData(void *p = 0);
   static void *newArray_BrikenTreeData(Long_t size, void *p);
   static void delete_BrikenTreeData(void *p);
   static void deleteArray_BrikenTreeData(void *p);
   static void destruct_BrikenTreeData(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::BrikenTreeData*)
   {
      ::BrikenTreeData *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::BrikenTreeData));
      static ::ROOT::TGenericClassInfo 
         instance("BrikenTreeData", "libDataStruct.h", 28,
                  typeid(::BrikenTreeData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &BrikenTreeData_Dictionary, isa_proxy, 4,
                  sizeof(::BrikenTreeData) );
      instance.SetNew(&new_BrikenTreeData);
      instance.SetNewArray(&newArray_BrikenTreeData);
      instance.SetDelete(&delete_BrikenTreeData);
      instance.SetDeleteArray(&deleteArray_BrikenTreeData);
      instance.SetDestructor(&destruct_BrikenTreeData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::BrikenTreeData*)
   {
      return GenerateInitInstanceLocal((::BrikenTreeData*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::BrikenTreeData*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *BrikenTreeData_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::BrikenTreeData*)0x0)->GetClass();
      BrikenTreeData_TClassManip(theClass);
   return theClass;
   }

   static void BrikenTreeData_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_NIGIRIHit(void *p = 0);
   static void *newArray_NIGIRIHit(Long_t size, void *p);
   static void delete_NIGIRIHit(void *p);
   static void deleteArray_NIGIRIHit(void *p);
   static void destruct_NIGIRIHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NIGIRIHit*)
   {
      ::NIGIRIHit *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NIGIRIHit >(0);
      static ::ROOT::TGenericClassInfo 
         instance("NIGIRIHit", ::NIGIRIHit::Class_Version(), "libDataStruct.h", 53,
                  typeid(::NIGIRIHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NIGIRIHit::Dictionary, isa_proxy, 4,
                  sizeof(::NIGIRIHit) );
      instance.SetNew(&new_NIGIRIHit);
      instance.SetNewArray(&newArray_NIGIRIHit);
      instance.SetDelete(&delete_NIGIRIHit);
      instance.SetDeleteArray(&deleteArray_NIGIRIHit);
      instance.SetDestructor(&destruct_NIGIRIHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NIGIRIHit*)
   {
      return GenerateInitInstanceLocal((::NIGIRIHit*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NIGIRIHit*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_NIGIRI(void *p = 0);
   static void *newArray_NIGIRI(Long_t size, void *p);
   static void delete_NIGIRI(void *p);
   static void deleteArray_NIGIRI(void *p);
   static void destruct_NIGIRI(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NIGIRI*)
   {
      ::NIGIRI *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NIGIRI >(0);
      static ::ROOT::TGenericClassInfo 
         instance("NIGIRI", ::NIGIRI::Class_Version(), "libDataStruct.h", 161,
                  typeid(::NIGIRI), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NIGIRI::Dictionary, isa_proxy, 4,
                  sizeof(::NIGIRI) );
      instance.SetNew(&new_NIGIRI);
      instance.SetNewArray(&newArray_NIGIRI);
      instance.SetDelete(&delete_NIGIRI);
      instance.SetDeleteArray(&deleteArray_NIGIRI);
      instance.SetDestructor(&destruct_NIGIRI);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NIGIRI*)
   {
      return GenerateInitInstanceLocal((::NIGIRI*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::NIGIRI*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr NIGIRIHit::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *NIGIRIHit::Class_Name()
{
   return "NIGIRIHit";
}

//______________________________________________________________________________
const char *NIGIRIHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NIGIRIHit*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int NIGIRIHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NIGIRIHit*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NIGIRIHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NIGIRIHit*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NIGIRIHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NIGIRIHit*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NIGIRI::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *NIGIRI::Class_Name()
{
   return "NIGIRI";
}

//______________________________________________________________________________
const char *NIGIRI::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NIGIRI*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int NIGIRI::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NIGIRI*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NIGIRI::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NIGIRI*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NIGIRI::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NIGIRI*)0x0)->GetClass(); }
   return fgIsA;
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_BrikenTreeData(void *p) {
      return  p ? new(p) ::BrikenTreeData : new ::BrikenTreeData;
   }
   static void *newArray_BrikenTreeData(Long_t nElements, void *p) {
      return p ? new(p) ::BrikenTreeData[nElements] : new ::BrikenTreeData[nElements];
   }
   // Wrapper around operator delete
   static void delete_BrikenTreeData(void *p) {
      delete ((::BrikenTreeData*)p);
   }
   static void deleteArray_BrikenTreeData(void *p) {
      delete [] ((::BrikenTreeData*)p);
   }
   static void destruct_BrikenTreeData(void *p) {
      typedef ::BrikenTreeData current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::BrikenTreeData

//______________________________________________________________________________
void NIGIRIHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class NIGIRIHit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NIGIRIHit::Class(),this);
   } else {
      R__b.WriteClassBuffer(NIGIRIHit::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NIGIRIHit(void *p) {
      return  p ? new(p) ::NIGIRIHit : new ::NIGIRIHit;
   }
   static void *newArray_NIGIRIHit(Long_t nElements, void *p) {
      return p ? new(p) ::NIGIRIHit[nElements] : new ::NIGIRIHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_NIGIRIHit(void *p) {
      delete ((::NIGIRIHit*)p);
   }
   static void deleteArray_NIGIRIHit(void *p) {
      delete [] ((::NIGIRIHit*)p);
   }
   static void destruct_NIGIRIHit(void *p) {
      typedef ::NIGIRIHit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NIGIRIHit

//______________________________________________________________________________
void NIGIRI::Streamer(TBuffer &R__b)
{
   // Stream an object of class NIGIRI.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(NIGIRI::Class(),this);
   } else {
      R__b.WriteClassBuffer(NIGIRI::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_NIGIRI(void *p) {
      return  p ? new(p) ::NIGIRI : new ::NIGIRI;
   }
   static void *newArray_NIGIRI(Long_t nElements, void *p) {
      return p ? new(p) ::NIGIRI[nElements] : new ::NIGIRI[nElements];
   }
   // Wrapper around operator delete
   static void delete_NIGIRI(void *p) {
      delete ((::NIGIRI*)p);
   }
   static void deleteArray_NIGIRI(void *p) {
      delete [] ((::NIGIRI*)p);
   }
   static void destruct_NIGIRI(void *p) {
      typedef ::NIGIRI current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NIGIRI

namespace ROOT {
   static TClass *vectorlEunsignedsPshortgR_Dictionary();
   static void vectorlEunsignedsPshortgR_TClassManip(TClass*);
   static void *new_vectorlEunsignedsPshortgR(void *p = 0);
   static void *newArray_vectorlEunsignedsPshortgR(Long_t size, void *p);
   static void delete_vectorlEunsignedsPshortgR(void *p);
   static void deleteArray_vectorlEunsignedsPshortgR(void *p);
   static void destruct_vectorlEunsignedsPshortgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<unsigned short>*)
   {
      vector<unsigned short> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<unsigned short>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<unsigned short>", -2, "vector", 210,
                  typeid(vector<unsigned short>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEunsignedsPshortgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<unsigned short>) );
      instance.SetNew(&new_vectorlEunsignedsPshortgR);
      instance.SetNewArray(&newArray_vectorlEunsignedsPshortgR);
      instance.SetDelete(&delete_vectorlEunsignedsPshortgR);
      instance.SetDeleteArray(&deleteArray_vectorlEunsignedsPshortgR);
      instance.SetDestructor(&destruct_vectorlEunsignedsPshortgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<unsigned short> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<unsigned short>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEunsignedsPshortgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<unsigned short>*)0x0)->GetClass();
      vectorlEunsignedsPshortgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEunsignedsPshortgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEunsignedsPshortgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<unsigned short> : new vector<unsigned short>;
   }
   static void *newArray_vectorlEunsignedsPshortgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<unsigned short>[nElements] : new vector<unsigned short>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEunsignedsPshortgR(void *p) {
      delete ((vector<unsigned short>*)p);
   }
   static void deleteArray_vectorlEunsignedsPshortgR(void *p) {
      delete [] ((vector<unsigned short>*)p);
   }
   static void destruct_vectorlEunsignedsPshortgR(void *p) {
      typedef vector<unsigned short> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<unsigned short>

namespace ROOT {
   static TClass *vectorlENIGIRIHitmUgR_Dictionary();
   static void vectorlENIGIRIHitmUgR_TClassManip(TClass*);
   static void *new_vectorlENIGIRIHitmUgR(void *p = 0);
   static void *newArray_vectorlENIGIRIHitmUgR(Long_t size, void *p);
   static void delete_vectorlENIGIRIHitmUgR(void *p);
   static void deleteArray_vectorlENIGIRIHitmUgR(void *p);
   static void destruct_vectorlENIGIRIHitmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<NIGIRIHit*>*)
   {
      vector<NIGIRIHit*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<NIGIRIHit*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<NIGIRIHit*>", -2, "vector", 210,
                  typeid(vector<NIGIRIHit*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlENIGIRIHitmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<NIGIRIHit*>) );
      instance.SetNew(&new_vectorlENIGIRIHitmUgR);
      instance.SetNewArray(&newArray_vectorlENIGIRIHitmUgR);
      instance.SetDelete(&delete_vectorlENIGIRIHitmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlENIGIRIHitmUgR);
      instance.SetDestructor(&destruct_vectorlENIGIRIHitmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<NIGIRIHit*> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<NIGIRIHit*>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlENIGIRIHitmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<NIGIRIHit*>*)0x0)->GetClass();
      vectorlENIGIRIHitmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlENIGIRIHitmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlENIGIRIHitmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<NIGIRIHit*> : new vector<NIGIRIHit*>;
   }
   static void *newArray_vectorlENIGIRIHitmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<NIGIRIHit*>[nElements] : new vector<NIGIRIHit*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlENIGIRIHitmUgR(void *p) {
      delete ((vector<NIGIRIHit*>*)p);
   }
   static void deleteArray_vectorlENIGIRIHitmUgR(void *p) {
      delete [] ((vector<NIGIRIHit*>*)p);
   }
   static void destruct_vectorlENIGIRIHitmUgR(void *p) {
      typedef vector<NIGIRIHit*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<NIGIRIHit*>

namespace {
  void TriggerDictionaryInitialization_libDataStruct_Impl() {
    static const char* headers[] = {
"/home/maxpallas/data/briken2021/TestScript/online1_he3/include/libDataStruct.h",
0
    };
    static const char* includePaths[] = {
"/opt/root/root_v6.20.00_gsl/root/include",
"/home/maxpallas/data/briken2021/TestScript/online1_he3",
"/home/maxpallas/data/briken2021/OnlineLibraries/include",
"/home/maxpallas/data/briken2021/TestScript/online1_he3/include",
"/opt/root/root_v6.20.00_gsl/root/include/",
"/data/briken2021/TestScript/online1_he3/build/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libDataStruct dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$/home/maxpallas/data/briken2021/TestScript/online1_he3/include/libDataStruct.h")))  BrikenTreeData;
class __attribute__((annotate("$clingAutoload$/home/maxpallas/data/briken2021/TestScript/online1_he3/include/libDataStruct.h")))  NIGIRIHit;
class __attribute__((annotate("$clingAutoload$/home/maxpallas/data/briken2021/TestScript/online1_he3/include/libDataStruct.h")))  NIGIRI;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libDataStruct dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "/home/maxpallas/data/briken2021/TestScript/online1_he3/include/libDataStruct.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"BrikenTreeData", payloadCode, "@",
"NIGIRI", payloadCode, "@",
"NIGIRIHit", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libDataStruct",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libDataStruct_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libDataStruct_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libDataStruct() {
  TriggerDictionaryInitialization_libDataStruct_Impl();
}
