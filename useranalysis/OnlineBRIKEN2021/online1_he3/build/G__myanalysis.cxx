// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__myanalysis
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
#include "/home/maxpallas/data/briken2021/TestScript/online1_he3/include/myanalysis.h"

// Header files passed via #pragma extra_include

namespace {
  void TriggerDictionaryInitialization_libmyanalysis_Impl() {
    static const char* headers[] = {
"/home/maxpallas/data/briken2021/TestScript/online1_he3/include/myanalysis.h",
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
#line 1 "libmyanalysis dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libmyanalysis dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "/home/maxpallas/data/briken2021/TestScript/online1_he3/include/myanalysis.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libmyanalysis",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libmyanalysis_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libmyanalysis_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libmyanalysis() {
  TriggerDictionaryInitialization_libmyanalysis_Impl();
}
