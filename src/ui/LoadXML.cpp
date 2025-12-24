#include "ui/LoadXML.hpp"
#include "gx/Coordinate.hpp"
#include "ui/simple/CSimpleFontString.hpp"
#include "ui/simple/CSimpleTexture.hpp"
#include "util/CStatus.hpp"
#include <common/XML.hpp>
#include <storm/String.hpp>

int32_t LoadXML_Color(const XMLNode* node, CImVector& color) {
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
    float a = 1.0f;

    auto rAttr = node->GetAttributeByName("r");
    if (rAttr && *rAttr) {
        if (SStrToFloat(rAttr) <= 0.0f) {
            r = 0.0f;
        } else if (SStrToFloat(rAttr) >= 1.0f) {
            r = 1.0f;
        } else {
            r = SStrToFloat(rAttr);
        }
    }

    auto gAttr = node->GetAttributeByName("g");
    if (gAttr && *gAttr) {
        if (SStrToFloat(gAttr) <= 0.0f) {
            g = 0.0f;
        } else if (SStrToFloat(gAttr) >= 1.0f) {
            g = 1.0f;
        } else {
            g = SStrToFloat(gAttr);
        }
    }

    auto bAttr = node->GetAttributeByName("b");
    if (bAttr && *bAttr) {
        if (SStrToFloat(bAttr) <= 0.0f) {
            b = 0.0f;
        } else if (SStrToFloat(bAttr) >= 1.0f) {
            b = 1.0f;
        } else {
            b = SStrToFloat(bAttr);
        }
    }

    auto aAttr = node->GetAttributeByName("a");
    if (aAttr && *aAttr) {
        if (SStrToFloat(aAttr) <= 0.0f) {
            a = 0.0f;
        } else if (SStrToFloat(aAttr) >= 1.0f) {
            a = 1.0f;
        } else {
            a = SStrToFloat(aAttr);
        }
    }

    color.Set(a, r, g, b);

    return 1;
}

int32_t LoadXML_Dimensions(const XMLNode* node, float& x, float& y, CStatus* status) {
    const char* xAttr = node->GetAttributeByName("x");

    if (xAttr && *xAttr) {
        float v18 = SStrToFloat(xAttr);
        float v6 = v18 / (CoordinateGetAspectCompensation() * 1024.0f);
        x = NDCToDDCWidth(v6);
    }

    const char* yAttr = node->GetAttributeByName("y");

    if (yAttr && *yAttr) {
        float v19 = SStrToFloat(yAttr);
        float v9 = v19 / (CoordinateGetAspectCompensation() * 1024.0f);
        y = NDCToDDCWidth(v9);
    }

    XMLNode* childNode = node->m_child;

    if (!childNode) {
        return yAttr != nullptr;
    }

    if (!SStrCmpI(childNode->GetName(), "RelDimension", 0x7FFFFFFF)) {
        const char* xAttrRel = childNode->GetAttributeByName("x");

        if (xAttrRel && *xAttrRel) {
            x = SStrToFloat(xAttrRel);
        }

        const char* yAttrRel = childNode->GetAttributeByName("y");

        if (yAttrRel && *yAttrRel) {
            y = SStrToFloat(yAttrRel);
        }

        return 1;
    } else if (!SStrCmpI(childNode->GetName(), "AbsDimension", 0x7FFFFFFF)) {
        const char* xAttrAbs = childNode->GetAttributeByName("x");

        if (xAttrAbs && *xAttrAbs) {
            float v20 = SStrToFloat(xAttrAbs);
            float v13 = v20 / (CoordinateGetAspectCompensation() * 1024.0f);
            x = NDCToDDCWidth(v13);
        }

        const char* yAttrAbs = childNode->GetAttributeByName("y");

        if (yAttrAbs && *yAttrAbs) {
            float v21 = SStrToFloat(yAttrAbs);
            float v15 = v21 / (CoordinateGetAspectCompensation() * 1024.0f);
            y = NDCToDDCWidth(v15);
        }

        return 1;
    } else {
        status->Add(
            STATUS_WARNING,
            "Unknown child node in %s element: %s",
            node->GetName(),
            childNode->GetName()
        );

        return 0;
    }
}

int32_t LoadXML_Gradient(const XMLNode* node, ORIENTATION& orientation, CImVector& minColor, CImVector& maxColor, CStatus* status) {
    orientation = ORIENTATION_HORIZONTAL;

    // Orientation
    auto orientationAttr = node->GetAttributeByName("orientation");
    if (orientationAttr && *orientationAttr) {
        if (!StringToOrientation(orientationAttr, orientation)) {
            status->Add(STATUS_WARNING, "Unknown orientation %s in element %s", orientationAttr, node->GetName());
        }
    }

    for (auto child = node->m_child; child; child = child->m_next) {
        if (!SStrCmpI(child->GetName(), "MinColor")) {
            LoadXML_Color(child, minColor);
        } else if (!SStrCmpI(child->GetName(), "MaxColor")) {
            LoadXML_Color(child, maxColor);
        }
    }

    return 1;
}

int32_t LoadXML_Insets(const XMLNode* node, float& left, float& right, float& top, float& bottom, CStatus* status) {
    left = 0.0f;
    right = 0.0f;
    top = 0.0f;
    bottom = 0.0f;

    const char* leftAttr = node->GetAttributeByName("left");
    if (leftAttr && *leftAttr) {
        float l = SStrToFloat(leftAttr);
        float ndcl = l / (CoordinateGetAspectCompensation() * 1024.0f);
        left = NDCToDDCWidth(ndcl);
    }

    const char* rightAttr = node->GetAttributeByName("right");
    if (rightAttr && *rightAttr) {
        float r = SStrToFloat(rightAttr);
        float ndcr = r / (CoordinateGetAspectCompensation() * 1024.0f);
        right = NDCToDDCWidth(ndcr);
    }

    const char* topAttr = node->GetAttributeByName("top");
    if (topAttr && *topAttr) {
        float t = SStrToFloat(topAttr);
        float ndct = t / (CoordinateGetAspectCompensation() * 1024.0f);
        top = NDCToDDCWidth(ndct);
    }

    const char* bottomAttr = node->GetAttributeByName("bottom");
    if (bottomAttr && *bottomAttr) {
        float b = SStrToFloat(bottomAttr);
        float ndcb = b / (CoordinateGetAspectCompensation() * 1024.0f);
        bottom = NDCToDDCWidth(ndcb);
    }

    auto child = node->m_child;

    if (!child) {
        if (!bottomAttr) {
            status->Add(
                STATUS_WARNING,
                "No \"right\", \"left\", \"top\", or \"bottom\" attributes in element: %s",
                node->GetName()
            );

            return 0;
        }

        return 1;
    }

    if (!SStrCmpI(child->GetName(), "AbsInset", STORM_MAX_STR)) {
        const char* leftAttr = child->GetAttributeByName("left");
        if (leftAttr && *leftAttr) {
            float l = SStrToFloat(leftAttr);
            float ndcl = l / (CoordinateGetAspectCompensation() * 1024.0f);
            left = NDCToDDCWidth(ndcl);
        }

        const char* rightAttr = child->GetAttributeByName("right");
        if (rightAttr && *rightAttr) {
            float r = SStrToFloat(rightAttr);
            float ndcr = r / (CoordinateGetAspectCompensation() * 1024.0f);
            right = NDCToDDCWidth(ndcr);
        }

        const char* topAttr = child->GetAttributeByName("top");
        if (topAttr && *topAttr) {
            float t = SStrToFloat(topAttr);
            float ndct = t / (CoordinateGetAspectCompensation() * 1024.0f);
            top = NDCToDDCWidth(ndct);
        }

        const char* bottomAttr = child->GetAttributeByName("bottom");
        if (bottomAttr && *bottomAttr) {
            float b = SStrToFloat(bottomAttr);
            float ndcb = b / (CoordinateGetAspectCompensation() * 1024.0f);
            bottom = NDCToDDCWidth(ndcb);
        }

        return 1;
    }

    if (!SStrCmpI(child->GetName(), "RelInset", STORM_MAX_STR)) {
        const char* leftAttr = child->GetAttributeByName("left");
        if (leftAttr && *leftAttr) {
            left = SStrToFloat(leftAttr);
        }

        const char* rightAttr = child->GetAttributeByName("right");
        if (rightAttr && *rightAttr) {
            right = SStrToFloat(rightAttr);
        }

        const char* topAttr = child->GetAttributeByName("top");
        if (topAttr && *topAttr) {
            top = SStrToFloat(topAttr);
        }

        const char* bottomAttr = child->GetAttributeByName("bottom");
        if (bottomAttr && *bottomAttr) {
            bottom = SStrToFloat(bottomAttr);
        }

        return 1;
    }

    status->Add(
        STATUS_WARNING,
        "Unknown child node in %s element: %s",
        child->GetName()
    );

    return 0;
}

CSimpleFontString* LoadXML_String(const XMLNode* node, CSimpleFrame* frame, CStatus* status) {
    // TODO
    // auto m = CDataAllocator::GetData(CSimpleFontString::s_allocator, 0, a__avcsimplefon, -2);

    auto m = SMemAlloc(sizeof(CSimpleFontString), __FILE__, __LINE__, 0x0);
    auto fontString = new (m) CSimpleFontString(frame, 2, 1);

    fontString->PreLoadXML(node, status);
    fontString->LoadXML(node, status);
    fontString->PostLoadXML(node, status);

    return fontString;
}

CSimpleTexture* LoadXML_Texture(const XMLNode* node, CSimpleFrame* frame, CStatus* status) {
    // TODO
    // auto m = (CSimpleTexture *)CDataAllocator::GetData((int)CSimpleTexture::s_allocator, 0, a__avcsimpletex, -2);

    auto m = SMemAlloc(sizeof(CSimpleTexture), __FILE__, __LINE__, 0x0);
    auto texture = new (m) CSimpleTexture(frame, 2, 1);

    texture->PreLoadXML(node, status);
    texture->LoadXML(node, status);
    texture->PostLoadXML(node, status);

    return texture;
}

int32_t LoadXML_Value(const XMLNode* node, float& value, CStatus* status) {
    value = 0.0;

    const char* v4 = node->GetAttributeByName("val");

    if (v4 && *v4) {
        float v = SStrToFloat(v4);
        float ndc = v / (CoordinateGetAspectCompensation() * 1024.0);
        value = NDCToDDCWidth(ndc);
    }

    XMLNode* child = node->m_child;

    if (!child) {
        if (v4) {
            return 1;
        } else {
            status->Add(STATUS_WARNING, "No \"val\" attribute in element: %s", node->GetName());

            return 0;
        }
    }

    if (!SStrCmpI(child->GetName(), "AbsValue", 0x7FFFFFFF)) {
        const char* v9 = child->GetAttributeByName("val");

        if (v9 && *v9) {
            float v = SStrToFloat(v9);
            float ndc = v / (CoordinateGetAspectCompensation() * 1024.0);
            value = NDCToDDCWidth(ndc);
        }

        return 1;
    } else if (!SStrCmpI(child->GetName(), "RelValue", 0x7FFFFFFF)) {
        const char* v11 = child->GetAttributeByName("val");

        if (v11 && *v11) {
            value = SStrToFloat(v11);
        }

        return 1;
    } else {
        status->Add(STATUS_WARNING, "Unknown child node in %s element: %s", child->GetName(), node->GetName());

        return 0;
    }
}
