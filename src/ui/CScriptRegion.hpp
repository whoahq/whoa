#ifndef UI_C_SCRIPT_REGION_HPP
#define UI_C_SCRIPT_REGION_HPP

#include "ui/CLayoutFrame.hpp"
#include "ui/CScriptObject.hpp"

class C2Vector;
class CSimpleAnimGroup;
class CSimpleFrame;
class XMLNode;
struct lua_State;

class CScriptRegion : public CScriptObject, public CLayoutFrame {
    public:
        // Static members
        static int32_t s_objectType;
        static const char* s_objectTypeName;

        // Static functions
        static void RegisterScriptMethods(lua_State*);
        static int32_t GetObjectType(void);

        // Member variables
        CSimpleFrame* m_parent = NULL; // TODO verify type

        // Virtual member functions
        virtual bool IsA(int32_t);
        virtual CScriptObject* GetScriptObjectParent(void);
        virtual bool IsA(const char*);
        virtual const char* GetObjectTypeName(void);
        virtual bool IsDragging(void);
        virtual void PreOnAnimUpdate(void) {};
        virtual void OnLayerUpdate(float);
        virtual void NotifyAnimBegin(CSimpleAnimGroup*);
        virtual void NotifyAnimEnd(CSimpleAnimGroup*);
        virtual void StopAnimating(void);
        virtual void AnimActivated(CSimpleAnimGroup*, int32_t, int32_t) {};
        virtual void AnimDeactivated(CSimpleAnimGroup*, int32_t, int32_t) {};
        virtual void AddAnimTranslation(CScriptRegion*, const C2Vector&) {};
        virtual void AddAnimRotation(CScriptRegion*, FRAMEPOINT, const C2Vector&, float) {};
        virtual void AddAnimScale(CScriptRegion*, FRAMEPOINT, const C2Vector&, const C2Vector&) {};
        virtual void AddAnimAlpha(CScriptRegion*, int16_t) {};
        virtual void LoadXML(XMLNode*, CStatus*);
        virtual CLayoutFrame* GetLayoutParent(void);
        virtual CLayoutFrame* GetLayoutFrameByName(const char*);

        // Member functions
        void LoadXML_Animations(XMLNode*, CStatus*);
        bool ProtectedFunctionsAllowed(void);
};

#endif
