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

int32_t LoadXML_Color(XMLNode* node, CImVector& color);

int32_t LoadXML_Dimensions(XMLNode* node, float& x, float& y, CStatus* status);

int32_t LoadXML_Gradient(XMLNode* node, ORIENTATION& orientation, CImVector& minColor, CImVector& maxColor, CStatus* status);

int32_t LoadXML_Insets(XMLNode* node, float& left, float& right, float& top, float& bottom, CStatus* status);

CSimpleFontString* LoadXML_String(XMLNode* node, CSimpleFrame* frame, CStatus* status);

CSimpleTexture* LoadXML_Texture(XMLNode* node, CSimpleFrame* frame, CStatus* status);

int32_t LoadXML_Value(XMLNode* node, float& value, CStatus* status);

#endif
