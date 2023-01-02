#ifndef UI_C_SIMPLE_RENDER_HPP
#define UI_C_SIMPLE_RENDER_HPP

class CGxShader;
class CRenderBatch;

class CSimpleRender {
    public:
        // Static variables
        static CGxShader* s_vertexShader[];

        // Static functions
        static void DrawBatch(CRenderBatch*);
        static void Init(void);
};

#endif
