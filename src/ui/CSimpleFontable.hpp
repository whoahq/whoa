#ifndef UI_C_SIMPLE_FONTABLE_HPP
#define UI_C_SIMPLE_FONTABLE_HPP

#include "ui/Types.hpp"
#include <storm/List.hpp>

class CSimpleFont;
class CSimpleFontStringAttributes;

class CSimpleFontable {
    public:
        // Member variables
        CSimpleFont* m_fontObject = nullptr;
        uint32_t m_fontableFlags = FLAG_COMPLETE_UPDATE;
        TSLink<CSimpleFontable> m_fontableLink;

        // Virtual member functions
        virtual ~CSimpleFontable();
        virtual void FontObjectUpdated(CSimpleFontStringAttributes&) = 0;

        // Member functions
        CSimpleFont* GetFontObject(void);
        void SetFontObject(CSimpleFont*);
};

#endif
