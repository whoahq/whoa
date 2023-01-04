#ifndef UI_C_BACKDROP_GENERATOR_HPP
#define UI_C_BACKDROP_GENERATOR_HPP

#include "gx/Types.hpp"
#include <common/String.hpp>
#include <tempest/Vector.hpp>

class CRect;
class CSimpleFrame;
class CSimpleTexture;
class CStatus;
class XMLNode;

class CBackdropGenerator {
    public:
        // Member variables
        CSimpleTexture* m_backgroundTexture = nullptr;
        CSimpleTexture* m_leftTexture = nullptr;
        CSimpleTexture* m_rightTexture = nullptr;
        CSimpleTexture* m_topTexture = nullptr;
        CSimpleTexture* m_bottomTexture = nullptr;
        CSimpleTexture* m_topLeftTexture = nullptr;
        CSimpleTexture* m_topRightTexture = nullptr;
        CSimpleTexture* m_bottomLeftTexture = nullptr;
        CSimpleTexture* m_bottomRightTexture = nullptr;
        RCString m_background;
        RCString m_border;
        uint32_t m_pieces = 0;
        int32_t m_tileBackground = 0;
        int32_t int44;
        float m_cornerSize = 0.025f;
        float m_backgroundSize = 0.0f;
        float m_topInset = 0.0f;
        float m_bottomInset = 0.0f;
        float m_leftInset = 0.0f;
        float m_rightInset = 0.0f;
        CImVector m_color = { 0x00 };
        CImVector m_borderColor = { 0x00 };
        EGxBlend m_blend;

        // Member functions
        CBackdropGenerator();
        void Generate(const CRect* rect);
        void LoadXML(XMLNode* node, CStatus* status);
        void SetBorderVertexColor(const CImVector& borderColor);
        void SetOutput(CSimpleFrame* frame);
};

#endif
