#ifndef UI_LOAD_XML_HPP
#define UI_LOAD_XML_HPP

#include <cstdint>

class CSimpleFontString;
class CSimpleFrame;
class CSimpleTexture;
class CStatus;
class XMLNode;
class CImVector;

int32_t LoadXML_Color(XMLNode*, CImVector&);

int32_t LoadXML_Dimensions(XMLNode*, float&, float&, CStatus*);

int32_t LoadXML_Insets(XMLNode*, float&, float&, float&, float&, CStatus*);

CSimpleFontString* LoadXML_String(XMLNode*, CSimpleFrame*, CStatus*);

CSimpleTexture* LoadXML_Texture(XMLNode*, CSimpleFrame*, CStatus*);

int32_t LoadXML_Value(XMLNode*, float&, CStatus*);

#endif
