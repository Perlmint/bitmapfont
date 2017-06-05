#pragma once
#if NODE

#include <node.h>
#include <node_object_wrap.h>
#include <nan.h>

#define DECL_CLASS(NAME) class NAME : public node::ObjectWrap
#define DECL_SETTER(NAME) \
static void Set##NAME(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info)
#define DECL_GETTER(NAME) \
static void Get##NAME(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info)
#define DECL_PROPERTY(NAME) \
DECL_SETTER(NAME);\
DECL_GETTER(NAME)
#define DECL_METHOD(NAME) \
static void NAME(const Nan::FunctionCallbackInfo<v8::Value>& info)

#else

#define DECL_CLASS(NAME) class NAME

#endif