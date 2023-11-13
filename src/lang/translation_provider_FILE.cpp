#include "translation_provider_FILE.hpp"

extern "C" size_t strlcpy(char *dst, const char *src, size_t dsize);

// path cannot be longer than 16 characters
FILETranslationProvider fileProviderInternal("/internal/ts.mo");
FILETranslationProvider fileProviderUSB("/usb/lang/ts.mo");

FILETranslationProvider::FILETranslationProvider(const char *path) {
    strlcpy(m_Path, path, sizeof(m_Path));
}

string_view_utf8 FILETranslationProvider::GetText(const char *key) const {
    // check if file is valid, if not try to open it again
    if (!EnsureFile()) {
        return string_view_utf8::MakeCPUFLASH((const uint8_t *)key);
    }

    // find translation for key, if not found return the original string
    int32_t offset = 0;
    switch (offset = m_HashTable.GetOffset(key)) {
    case gettext_hash_table::FileErrorOccurred:
        fclose(m_File);
        m_File = nullptr;
        [[fallthrough]];
    case gettext_hash_table::TranslationNotFound:
        return string_view_utf8::MakeCPUFLASH((const uint8_t *)key);
    default:
        return string_view_utf8::MakeFILE(m_File, offset);
    }
}

bool FILETranslationProvider::EnsureFile() const {
    // check if there is open file, if yes it must have been open with this function and is valid
    if (m_File != nullptr) {
        return true;
    }
    m_File = fopen(m_Path, "rb"); // now we know that the FILE* is valid
    if (m_File == nullptr) {
        return false;
    }

    // set file buffer to 64B -> most reads are very short, so it will be more efficient to have relatively small buffer.
    setvbuf(m_File, nullptr, _IOFBF, 64);

    if (!m_HashTable.Init(m_File)) {
        fclose(m_File);
        m_File = nullptr;
        return false;
    }

    return true;
}

namespace {
#include "option/enable_translation_cs.h"
#if ENABLE_TRANSLATION_CS()
static const FILETranslationProvider cs("/internal/res/lang/cs.mo");
ProviderRegistrator csReg("cs", &cs);
#endif

#include "option/enable_translation_de.h"
#if ENABLE_TRANSLATION_DE()
static const FILETranslationProvider de("/internal/res/lang/de.mo");
ProviderRegistrator deReg("de", &de);
#endif

#include "option/enable_translation_es.h"
#if ENABLE_TRANSLATION_ES()
static const FILETranslationProvider es("/internal/res/lang/es.mo");
ProviderRegistrator esReg("es", &es);
#endif

#include "option/enable_translation_fr.h"
#if ENABLE_TRANSLATION_FR()
static const FILETranslationProvider fr("/internal/res/lang/fr.mo");
ProviderRegistrator frReg("fr", &fr);
#endif

#include "option/enable_translation_it.h"
#if ENABLE_TRANSLATION_IT()
static const FILETranslationProvider it("/internal/res/lang/it.mo");
ProviderRegistrator itReg("it", &it);
#endif

#include "option/enable_translation_pl.h"
#if ENABLE_TRANSLATION_PL()
static const FILETranslationProvider pl("/internal/res/lang/pl.mo");
ProviderRegistrator plReg("pl", &pl);
#endif
} // namespace
