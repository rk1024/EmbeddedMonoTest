#pragma once

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-config.h>

#define MONOEXPORT __declspec(dllexport)

//The name used for dynamically-loaded Mono procedures
#define MonoProcName(name) _##name

//The method pointer type used for dyamically-loaded Mono procedures
#define MonoProcType(name) name##_t

//Defines a method pointer type for and global pointer to a Mono procedure
#define DefMonoProc(name, ret, ...) typedef ret (__cdecl *MonoProcType(name))(##__VA_ARGS__); MonoProcType(name) MonoProcName(name)

//The pointer to Mono's DLL
#define MONO_HINST

//Assigns a value to a previously-defined Mono procedure
#define GetMonoProc(name) MonoProcName(name) = (MonoProcType(name)) GetProcAddress(MONO_HINST, #name)

//Definitions of Mono procedures
DefMonoProc(mono_add_internal_call, void, const char *name, const void* method);
DefMonoProc(mono_domain_assembly_open, MonoAssembly*, MonoDomain *domain, const char *name);
DefMonoProc(mono_jit_cleanup, void, MonoDomain *domain);
DefMonoProc(mono_jit_exec, int, MonoDomain *domain, MonoAssembly *assembly, int argc, char *argv[]);
DefMonoProc(mono_jit_init, MonoDomain*, const char *file);
DefMonoProc(mono_string_new, MonoString*, MonoDomain *domain, const char *text);

//Assigns values to all Mono procedures defined above
#define DeclMonoProcs()\
GetMonoProc(mono_add_internal_call); \
GetMonoProc(mono_domain_assembly_open); \
GetMonoProc(mono_jit_cleanup); \
GetMonoProc(mono_jit_exec); \
GetMonoProc(mono_jit_init); \
GetMonoProc(mono_string_new)