#ifndef UI_LOAD_XML_HPP
#define UI_LOAD_XML_HPP

#include "ui/Types.hpp"
#include <cstdint>

class CImVector;
class CSimpleFontString;
class CSimpleFrame;
class CSimpleTexture;
class CStatus;
class XMLNode;

int32_t LoadXML_Color(const XMLNode* node, CImVector& color);

int32_t LoadXML_Dimensions(const XMLNode* node, float& x, float& y, CStatus* status);

int32_t LoadXML_Gradient(const XMLNode* node, ORIENTATION& orientation, CImVector& minColor, CImVector& maxColor, CStatus* status);

int32_t LoadXML_Insets(const XMLNode* node, float& left, float& right, float& top, float& bottom, CStatus* status);

CSimpleFontString* LoadXML_String(const XMLNode* node, CSimpleFrame* frame, CStatus* status);

CSimpleTexture* LoadXML_Texture(const XMLNode* node, CSimpleFrame* frame, CStatus* status);

int32_t LoadXML_Value(const XMLNode* node, float& value, CStatus* status);

#endif
