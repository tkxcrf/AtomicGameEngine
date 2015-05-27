
#pragma once

#include <Atomic/Core/Object.h>

#include "JSBHeader.h"
#include "JSBModule.h"

using namespace Atomic;

namespace ToolCore
{

class JSBPackage;
class JSBFunction;
class JSBType;

// chosen function overrides
class JSBFunctionOverride
{

public:

    JSBFunctionOverride(const String& name, const Vector<String>& sig);

    String name_;
    Vector<String> sig_;
    Vector<JSBType*> types_;

    void Parse() ;

    bool parsed_;

};

class JSBProperty
{
public:
    String name_;
    JSBFunction* getter_;
    JSBFunction* setter_;

    JSBProperty() : getter_(0), setter_(0)
    {

    }

};


class JSBClass : public Object
{
    friend class JSBClassWriter;

    OBJECT(JSBClass)

public:

    JSBClass(Context* context, JSBModule* module, const String& name, const String& nativeName);
    virtual ~JSBClass();

    const String& GetName() { return name_; }
    const String& GetNativeName() { return nativeName_; }
    JSBClass* GetBaseClass();
    PODVector<JSBClass*>& GetBaseClasses() {return baseClasses_; }
    PODVector<JSBFunction*>& GetFunctions() { return functions_; }

    bool IsAbstract() { return isAbstract_; }

    /// Note that if we at some point want to generate bindings for JSBClass
    /// this override will need to be addressed, as we'll need to know that JSBClass is
    /// itself an object
    bool IsObject() { return isObject_; }

    bool HasProperties() { return hasProperties_; }
    void GetPropertyNames(Vector<String>& names) { names = properties_.Keys(); }
    JSBProperty* GetProperty(const String& name)
    {
        if (!properties_.Contains(name))
            return 0;

        return properties_[name];
    }


    JSBHeader* GetHeader() { return header_; }
    JSBModule* GetModule() { return module_; }
    JSBPackage* GetPackage() { return module_->GetPackage(); }

    bool IsNumberArray() { return numberArrayElements_ != 0; }
    int  GetNumberArrayElements() { return numberArrayElements_;}
    const String& GetArrayElementType() const { return arrayElementType_; }

    JSBFunction* GetConstructor();

    void SetAbstract(bool value = true) { isAbstract_ = value; }
    void SetObject(bool value = true) { isObject_ = value; }
    void SetHeader(JSBHeader* header) { header_ = header; }
    void SetBaseClass(JSBClass* baseClass);

    void SetSkipFunction(const String& name, bool skip = true);
    void AddFunction(JSBFunction* function);
    void AddFunctionOverride(JSBFunctionOverride* override) { overrides_.Push(override); }
    void AddPropertyFunction(JSBFunction* function);

    void Preprocess();
    void Process();
    void PostProcess();

    void Dump();

private:

    void RecursiveAddBaseClass(PODVector<JSBClass *> &baseClasses);

    String name_;
    String nativeName_;

    SharedPtr<JSBHeader> header_;
    SharedPtr<JSBModule> module_;

    PODVector<JSBFunction*> functions_;
    PODVector<JSBClass*> baseClasses_;

    PODVector<JSBFunctionOverride*> overrides_;

    bool isAbstract_;
    bool isObject_;

    // Vector3, Color, etc are marshalled via arrays
    int numberArrayElements_;
    String arrayElementType_;

    bool hasProperties_;
    HashMap<String, JSBProperty*> properties_;

};


}
