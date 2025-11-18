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
        static void RegisterScriptMethods(lua_State* L);
        static int32_t GetObjectType();

        // Member variables
        CSimpleFrame* m_parent = nullptr; // TODO verify type

        // Virtual member functions
        virtual bool IsA(int32_t type);
        virtual CScriptObject* GetScriptObjectParent();
        virtual bool IsA(const char* typeName);
        virtual const char* GetObjectTypeName();
        virtual bool IsDragging();
        virtual void PreOnAnimUpdate() {};
        virtual void OnLayerUpdate(float elapsedSec);
        virtual void NotifyAnimBegin(CSimpleAnimGroup* animGroup);
        virtual void NotifyAnimEnd(CSimpleAnimGroup* animGroup);
        virtual void StopAnimating();
        virtual void AnimActivated(CSimpleAnimGroup* animGroup, int32_t, int32_t) {};
        virtual void AnimDeactivated(CSimpleAnimGroup* animGroup, int32_t, int32_t) {};
        virtual void AddAnimTranslation(CScriptRegion*, const C2Vector&) {};
        virtual void AddAnimRotation(CScriptRegion*, FRAMEPOINT, const C2Vector&, float) {};
        virtual void AddAnimScale(CScriptRegion*, FRAMEPOINT, const C2Vector&, const C2Vector&) {};
        virtual void AddAnimAlpha(CScriptRegion*, int16_t) {};
        virtual void LoadXML(XMLNode* node, CStatus* status);
        virtual CLayoutFrame* GetLayoutParent();
        virtual CLayoutFrame* GetLayoutFrameByName(const char* name);

        // Member functions
        void LoadXML_Animations(XMLNode* node, CStatus* status);
        bool ProtectedFunctionsAllowed();
        void SetParent(CSimpleFrame* parent);
};

#endif
