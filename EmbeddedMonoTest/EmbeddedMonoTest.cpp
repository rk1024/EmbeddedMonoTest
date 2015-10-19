// EmbeddedMonoTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "monoImports.h" //This includes stuff needed for loading the DLL at runtime

MonoDomain *domain;

//Basic struct type to test marshalling
struct MyStruct {
public:
  int x;
};

//Testing internal calls
MONOEXPORT MonoString* EmbeddedApp_Test() {
  return _mono_string_new(domain, "Hello!");
}

//Testing struct marshalling from native to managed code
MONOEXPORT MyStruct EmbeddedApp_GetMyStruct() {
  MyStruct s = { 5 };

  return s;
}

//Testing marshalling and passing structs
MONOEXPORT MyStruct EmbeddedApp_IncrementMyStruct(MyStruct myStruct) {
  myStruct.x++;

  return myStruct;
}

int main(int argc, char *argv[]) {
  HINSTANCE hInstMono = LoadLibrary(_T("C:\\Program Files (x86)\\Mono\\bin\\mono-2.0.dll")); //Load Mono's DLL dynamically

  if (hInstMono == NULL) { //Did it fail?
    std::cout << "Mono not found." << std::endl;

    return 1;
  }

#define MONO_HINST hInstMono
  DeclMonoProcs();  //Get pointers to the methods we need within the DLL.

  domain = _mono_jit_init("EmbeddedApp");

  auto assem = _mono_domain_assembly_open(domain, "EmbeddedApp.exe");

  if (!assem) {
    std::cout << "Couldn't open assembly." << std::endl;

    return 1;
  }

  //Register internal calls within the C# app
  _mono_add_internal_call("EmbeddedApp.EmbeddedApp::Test", EmbeddedApp_Test);
  _mono_add_internal_call("EmbeddedApp.EmbeddedApp::GetMyStruct", EmbeddedApp_GetMyStruct);
  _mono_add_internal_call("EmbeddedApp.EmbeddedApp::IncrementMyStruct", EmbeddedApp_IncrementMyStruct);

  _mono_jit_exec(domain, assem, argc, argv); //Launch the C# app from its entry point

  _mono_jit_cleanup(domain);

  FreeLibrary(hInstMono); //Release Mono's DLL

  return 0;
}