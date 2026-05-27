
#ifndef INCLUDED_StringWrapper
#define INCLUDED_StringWrapper

// system
#include <string>

// ICU
#include <unicode/unistr.h>
#include <unicode/ustring.h>


// ----------------------------------------------------------------------------
// String Debuggging

// These macros set/check flags in debug mode to ensure that the string classes are
// being used correctly. Asserts indicate incorrect usage of the class.
#ifdef STRINGWRAPPER_ASSERT
# define STRINGWRAPPER_NULLSAFE_DEFINE      bool m_bIsTerminated
# define STRINGWRAPPER_NULLSAFE_SET(x)      m_bIsTerminated=(x)
# define STRINGWRAPPER_NULLSAFE_TRUE        m_bIsTerminated=true
# define STRINGWRAPPER_NULLSAFE_ASSERT      STRINGWRAPPER_ASSERT(m_bIsTerminated)
# define STRINGWRAPPER_OPENBUF_DEFINE       bool m_bBufferIsOpen
# define STRINGWRAPPER_OPENBUF_TRUE         m_bBufferIsOpen=true
# define STRINGWRAPPER_OPENBUF_FALSE        m_bBufferIsOpen=false
# define STRINGWRAPPER_OPENBUF_ASSERT       STRINGWRAPPER_ASSERT(!m_bBufferIsOpen)
# define STRINGWRAPPER_OPENBUF_ASSERT_(x)   STRINGWRAPPER_ASSERT(!x.m_bBufferIsOpen)
# define STRINGWRAPPER_OPENBUF_ASSERT_TRUE  STRINGWRAPPER_ASSERT(m_bBufferIsOpen)
#else
# define STRINGWRAPPER_ASSERT(x)
# define STRINGWRAPPER_NULLSAFE_DEFINE
# define STRINGWRAPPER_NULLSAFE_SET(x)
# define STRINGWRAPPER_NULLSAFE_TRUE
# define STRINGWRAPPER_NULLSAFE_ASSERT
# define STRINGWRAPPER_OPENBUF_DEFINE
# define STRINGWRAPPER_OPENBUF_TRUE
# define STRINGWRAPPER_OPENBUF_FALSE
# define STRINGWRAPPER_OPENBUF_ASSERT
# define STRINGWRAPPER_OPENBUF_ASSERT_(x)
# define STRINGWRAPPER_OPENBUF_ASSERT_TRUE
#endif

// ----------------------------------------------------------------------------
// API definitions

#ifndef STRINGWRAPPER_CLASS_API
# define STRINGWRAPPER_CLASS_API
#endif
#ifndef STRINGWRAPPER_API
# define STRINGWRAPPER_API
#endif
#ifdef STRINGWRAPPER_NAMESPACE
# define STRINGWRAPPER_NS STRINGWRAPPER_NAMESPACE
namespace STRINGWRAPPER_NS {
#else
# define STRINGWRAPPER_NS
#endif

// ----------------------------------------------------------------------------
class STRINGWRAPPER_CLASS_API NarrowString
{
    std::string mStr;
    STRINGWRAPPER_OPENBUF_DEFINE;

public:
    inline NarrowString()
    {
        STRINGWRAPPER_OPENBUF_FALSE;
    }

    inline NarrowString(const char * text)
        : mStr(text)
    {
        STRINGWRAPPER_OPENBUF_FALSE;
    }

    inline NarrowString(const char * text, int32_t len)
        : mStr(text, (len == -1) ? strlen(text) : len)
    {
        STRINGWRAPPER_OPENBUF_FALSE;
    }

    inline NarrowString(const NarrowString & text)
        : mStr(text.mStr)
    {
        STRINGWRAPPER_OPENBUF_FALSE;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
    }

    inline NarrowString(const NarrowString & text, int32_t start)
        : mStr(text.mStr, start)
    {
        STRINGWRAPPER_OPENBUF_FALSE;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
    }

    inline NarrowString(const NarrowString & text, int32_t start, int32_t len)
        : mStr(text.mStr, start, (len == -1) ? text.mStr.size() - start : len)
    {
        STRINGWRAPPER_OPENBUF_FALSE; STRINGWRAPPER_OPENBUF_ASSERT_(text);
    }

    inline ~NarrowString() { }

    inline bool isEmpty() const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        return mStr.empty();
    }

    inline int32_t length() const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        return int32_t(mStr.size());
    }

    inline NarrowString & operator=(const NarrowString & text) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
        mStr.assign(text.getBuffer(), text.length());
        return *this;
    }

    inline NarrowString & operator=(const char * text) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        mStr.assign(text, strlen(text));
        return *this;
    }

    inline NarrowString & operator=(char ch) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        mStr.assign(&ch, 1);
        return *this;
    }

    inline NarrowString & setTo(const NarrowString & text) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
        mStr.assign(text.getBuffer(), text.length());
        return *this;
    }

    inline NarrowString & setTo(const NarrowString & text, int32_t start) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
        mStr.assign(text.getBuffer() + start, text.length() - start);
        return *this;
    }

    inline NarrowString & setTo(const NarrowString & text, int32_t start, int32_t len) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
        if (len == -1) len = text.length() - start;
        mStr.assign(text.getBuffer() + start, len);
        return *this;
    }

    inline NarrowString & setTo(const char * text) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        mStr.assign(text, strlen(text));
        return *this;
    }

    inline NarrowString & setTo(const char * text, int32_t len) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        if (len == -1) len = (int32_t) strlen(text);
        mStr.assign(text, len);
        return *this;
    }

    inline NarrowString & setTo(char ch) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        mStr.assign(&ch, 1);
        return *this;
    }

    inline NarrowString & operator+=(const NarrowString & text) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
        mStr.append(text.getBuffer(), text.length());
        return *this;
    }

    inline NarrowString & operator+=(const char * text) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        mStr.append(text, strlen(text));
        return *this;
    }

    inline NarrowString & operator+=(char ch) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        mStr.append(&ch, 1);
        return *this;
    }

    inline NarrowString & append(const NarrowString & text) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
        mStr.append(text.getBuffer(), text.length());
        return *this;
    }

    inline NarrowString & append(const NarrowString & text, int32_t start, int32_t len) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
        if (len == -1) len = text.length() - start;
        mStr.append(text.getBuffer() + start, len);
        return *this;
    }

    inline NarrowString & append(const char * text, int32_t len) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        if (len == -1) len = (int32_t) strlen(text);
        mStr.append(text, len);
        return *this;
    }

    inline NarrowString & append(const char * text, int32_t start, int32_t len) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        if (len == -1) len = (int32_t) strlen(text) - start;
        mStr.append(text + start, len);
        return *this;
    }

    inline NarrowString & append(char ch) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        mStr.append(&ch, 1);
        return *this;
    }

    inline bool operator==(const NarrowString & text) const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
        return compare(text) == 0;
    }

    inline bool operator!=(const NarrowString & text) const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
        return compare(text) != 0;
    }

    inline bool operator> (const NarrowString & text) const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
        return compare(text) >  0;
    }

    inline bool operator>=(const NarrowString & text) const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
        return compare(text) >= 0;
    }

    inline bool operator< (const NarrowString & text) const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
        return compare(text) <  0;
    }

    inline bool operator<=(const NarrowString & text) const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
        return compare(text) <= 0;
    }

    inline int8_t compare(const NarrowString & text) const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
        return (int8_t) strcmp(getBuffer(), text.getBuffer());
    }

    inline int8_t compare(const NarrowString & text, int32_t len) const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
        return (int8_t) strncmp(getBuffer(), text.getBuffer(), len);
    }

    inline int8_t compare(const char * text) const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        return (int8_t) strcmp(getBuffer(), text);
    }

    inline int8_t compare(const char * text, int32_t len) const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        return (int8_t) strncmp(getBuffer(), text, len);
    }

    inline int8_t caseCompare(const NarrowString & text) const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
        return caseCompare(text.getBuffer());
    }

    int8_t caseCompare(const char * chars) const;

    inline char first() const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_ASSERT(!isEmpty());
        return mStr[0];
    }

    inline char last() const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_ASSERT(!isEmpty());
        return mStr[mStr.length()-1];
    }

    inline char operator[](int32_t offset) const { 
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_ASSERT(offset >= 0 && offset < length());
        return mStr[offset];
    }
    
    inline char getCharAt(int32_t offset) const { return operator[](offset); }

    inline NarrowString & setCharAt(int32_t offset, char ch) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_ASSERT(offset >= 0 && offset < length());
        mStr[offset] = ch;
        return *this;
    }

    inline const char * getBuffer() const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_ASSERT(mStr.c_str()[mStr.length()] == 0);
        return mStr.c_str();
    }

    inline char * getBuffer(int32_t minCapacity) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        if (minCapacity > (int32_t) mStr.size()) mStr.resize(minCapacity);
        STRINGWRAPPER_OPENBUF_TRUE;
        return const_cast<char*>(mStr.c_str());
    }

    inline void releaseBuffer(int32_t newLength = -1) {
        STRINGWRAPPER_OPENBUF_ASSERT_TRUE;
        if (newLength < 0) newLength = (int32_t) strlen(mStr.c_str());
        mStr.erase(newLength);
        STRINGWRAPPER_OPENBUF_FALSE;
    }

    inline int32_t getCapacity() const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        return int32_t(mStr.capacity());
    }

    inline void setCapacity(int32_t n) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        return mStr.reserve(n);
    }

    inline NarrowString & remove() {
        STRINGWRAPPER_OPENBUF_ASSERT;
        mStr.clear();
        return *this;
    }

    inline NarrowString & remove(int32_t start, int32_t length = INT32_MAX) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        mStr.erase(mStr.begin() + start, mStr.begin() + start + length);
        return *this;
    }

    inline bool truncate(int32_t targetLength) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        mStr.erase(mStr.begin() + targetLength, mStr.end());
        return true;
    }

    inline NarrowString & insert(int32_t offset, const NarrowString & srcText) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_OPENBUF_ASSERT_(srcText);
        return insert(offset, srcText.getBuffer());
    }

    inline NarrowString & insert(int32_t offset, const char * srcText) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        mStr.insert(offset, srcText);
        return *this;
    }

    inline NarrowString & insert(int32_t offset, char ch) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        mStr.insert(offset, &ch, 1);
        return *this;
    }

    NarrowString & toUpper();

    NarrowString & toLower();

    NarrowString & trim();

    inline int32_t indexOf(char c, int32_t start = 0) const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        std::string::size_type n = mStr.find(c, start);
        return (n == std::string::npos) ? -1 : int32_t(n);
    }

    inline int32_t indexOf(const NarrowString & text, int32_t start = 0) const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
        std::string::size_type n = mStr.find(text.mStr, start);
        return (n == std::string::npos) ? -1 : int32_t(n);
    }

    inline int32_t indexOf(const char * text, int32_t start = 0) const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        std::string::size_type n = mStr.find(text, start);
        return (n == std::string::npos) ? -1 : int32_t(n);
    }

    inline int32_t lastIndexOf(char c, int32_t start = 0) const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        std::string::size_type n = mStr.rfind(c, start);
        return (n == std::string::npos) ? -1 : int32_t(n);
    }

    inline int32_t lastIndexOf(const NarrowString & text, int32_t start = 0) const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
        std::string::size_type n = mStr.rfind(text.mStr, start);
        return (n == std::string::npos) ? -1 : int32_t(n);
    }

    inline int32_t lastIndexOf(const char * text, int32_t start = 0) const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        std::string::size_type n = mStr.rfind(text, start);
        return (n == std::string::npos) ? -1 : int32_t(n);
    }

    inline NarrowString & replace(int32_t start, int32_t length, char srcChar) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        return replace(start, length, &srcChar, 1);
    }

    inline NarrowString & replace(int32_t start, int32_t length, const NarrowString & srcText) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_OPENBUF_ASSERT_(srcText);
        return replace(start, length, srcText.getBuffer(), srcText.length());
    }

    inline NarrowString & replace(int32_t start, int32_t length,
        const char * srcText, int32_t srcLength = -1)
    {
        STRINGWRAPPER_OPENBUF_ASSERT;
        if (srcLength == -1) srcLength = (int32_t) strlen(srcText);
        mStr.replace(start, length, srcText, srcLength);
        return *this;
    }

    inline NarrowString & findAndReplace(char chOld, char chNew) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        std::string::size_type n = mStr.find(chOld);
        while (n != std::string::npos) {
            mStr[n] = chNew;
            n = mStr.find(chOld, n+1);
        }
        return *this;
    }

    inline NarrowString & findAndReplace(
        const NarrowString & oldText, const NarrowString & newText)
    {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_OPENBUF_ASSERT_(newText);
        std::string::size_type n = mStr.find(oldText.mStr);
        while (n != std::string::npos) {
            mStr.replace(n, oldText.mStr.size(), newText.mStr);
            n = mStr.find(oldText.mStr, n + newText.mStr.size());
        }
        return *this;
    }

    void format(const char * a_pszFormat, ...);
    void formatV(const char * a_pszFormat, va_list ap);

    struct LessNoCase {
        bool operator()(const NarrowString & lhs, const NarrowString & rhs) const {
            return lhs.caseCompare(rhs) < 0;
        }
    };
};

// ----------------------------------------------------------------------------
class STRINGWRAPPER_CLASS_API WideString
{
    UnicodeString mStr;
    STRINGWRAPPER_NULLSAFE_DEFINE;
    STRINGWRAPPER_OPENBUF_DEFINE;

public:
    inline WideString() {
        STRINGWRAPPER_OPENBUF_FALSE;
        STRINGWRAPPER_NULLSAFE_TRUE;
        mStr.getTerminatedBuffer();
    }

    inline WideString(const UChar * text)
        : mStr(text)
    {
        STRINGWRAPPER_OPENBUF_FALSE;
        STRINGWRAPPER_NULLSAFE_TRUE;
        mStr.getTerminatedBuffer();
    }

    inline WideString(const UChar * text, int32_t len)
        : mStr(text, len)
    {
        STRINGWRAPPER_OPENBUF_FALSE;
        STRINGWRAPPER_NULLSAFE_TRUE;
        mStr.getTerminatedBuffer();
    }

    inline WideString(const WideString & text)
        : mStr(text.mStr)
    {
        STRINGWRAPPER_OPENBUF_FALSE;
        STRINGWRAPPER_NULLSAFE_TRUE;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
        mStr.getTerminatedBuffer();
    }

    inline WideString(const WideString & text, int32_t start)
        : mStr(text.mStr, start)
    {
        STRINGWRAPPER_OPENBUF_FALSE;
        STRINGWRAPPER_NULLSAFE_TRUE;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
        mStr.getTerminatedBuffer();
    }

    inline WideString(const WideString & text, int32_t start, int32_t len)
        : mStr(text.mStr, start, len)
    {
        STRINGWRAPPER_OPENBUF_FALSE;
        STRINGWRAPPER_NULLSAFE_TRUE;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
        mStr.getTerminatedBuffer();
    }

    inline WideString(const char * text, int32_t len, UnicodeString::EInvariant)
        : mStr(text, len, US_INV)
    {
        STRINGWRAPPER_OPENBUF_FALSE;
        STRINGWRAPPER_NULLSAFE_TRUE;
        mStr.getTerminatedBuffer();
    }

    inline ~WideString() { }

    inline bool isEmpty() const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        return mStr.isEmpty() == TRUE;
    }

    inline int32_t length() const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        return mStr.length();
    }

    inline WideString & operator=(const WideString & text) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_NULLSAFE_TRUE;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
        return setTo(text.mStr.getBuffer(), text.mStr.length());
    }

    inline WideString & operator=(const UnicodeString & text) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_NULLSAFE_TRUE;
        return setTo(text.getBuffer(), text.length());
    }

    inline WideString & operator=(const UChar * text) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_NULLSAFE_TRUE;
        return setTo(text, u_strlen(text));
    }

    inline WideString & operator=(UChar ch) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_NULLSAFE_TRUE;
        return setTo(&ch, 1);
    }

    inline WideString & setTo(const WideString & text) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_NULLSAFE_TRUE;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
        return setTo(text.mStr.getBuffer(), text.mStr.length());
    }

    inline WideString & setTo(const UnicodeString & text) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_NULLSAFE_TRUE;
        return setTo(text.getBuffer(), text.length());
    }

    inline WideString & setTo(const WideString & text, int32_t start) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_NULLSAFE_TRUE;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
        return setTo(text.mStr.getBuffer() + start, text.mStr.length() - start);
    }

    inline WideString & setTo(const WideString & text, int32_t start, int32_t len) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_NULLSAFE_TRUE;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
        if (len == -1) len = text.mStr.length() - start;
        return setTo(text.mStr.getBuffer() + start, len);
    }

    inline WideString & setTo(const UChar * text) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_NULLSAFE_TRUE;
        return setTo(text, u_strlen(text));
    }

    inline WideString & setTo(const UChar * text, int32_t len) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_NULLSAFE_TRUE;
        if (len == -1) len = u_strlen(text);
        // to avoid reallocations we manually get the buffer including space for a NULL
        UChar * pBuf = mStr.getBuffer(len+1);
        memcpy(pBuf, text, len*sizeof(UChar));
        pBuf[len] = 0;
        mStr.releaseBuffer(len);
        mStr.getTerminatedBuffer();
        return *this;
    }

    inline WideString & setTo(const char * text, int32_t len, UnicodeString::EInvariant) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_NULLSAFE_TRUE;
        mStr.setTo(text, len, US_INV);
        mStr.getTerminatedBuffer();
        return *this;
    }

    inline WideString & setTo(UChar ch) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_NULLSAFE_TRUE;
        return setTo(&ch, 1);
    }

    inline WideString & operator+=(const WideString & text) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_NULLSAFE_TRUE;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
        mStr.append(text.mStr.getBuffer(), text.mStr.length());
        mStr.getTerminatedBuffer();
        return *this;
    }

    inline WideString & operator+=(const UChar * text) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_NULLSAFE_TRUE;
        mStr.append(text, u_strlen(text));
        mStr.getTerminatedBuffer();
        return *this;
    }

    inline WideString & operator+=(UChar ch) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_NULLSAFE_TRUE;
        mStr.append(&ch, 1);
        mStr.getTerminatedBuffer();
        return *this;
    }

    inline WideString & append(const WideString & text) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_NULLSAFE_TRUE;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
        mStr.append(text.mStr.getBuffer(), text.mStr.length());
        mStr.getTerminatedBuffer();
        return *this;
    }

    inline WideString & append(const WideString & text, int32_t start, int32_t len) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_NULLSAFE_TRUE;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
        if (len == -1) len = text.mStr.length() - start;
        mStr.append(text.mStr.getBuffer() + start, len);
        mStr.getTerminatedBuffer();
        return *this;
    }

    inline WideString & append(const UChar * text, int32_t len) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_NULLSAFE_TRUE;
        if (len == -1) len = u_strlen(text);
        mStr.append(text, len);
        mStr.getTerminatedBuffer();
        return *this;
    }

    inline WideString & append(const UChar * text, int32_t start, int32_t len) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_NULLSAFE_TRUE;
        if (len == -1) len = u_strlen(text) - start;
        mStr.append(text + start, len);
        mStr.getTerminatedBuffer();
        return *this;
    }

    inline WideString & append(UChar ch) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_NULLSAFE_TRUE;
        mStr.append(&ch, 1);
        mStr.getTerminatedBuffer();
        return *this;
    }

    inline bool operator==(const WideString & text) const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
        return compare(text) == 0;
    }

    inline bool operator!=(const WideString & text) const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
        return compare(text) != 0;
    }

    inline bool operator> (const WideString & text) const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
        return compare(text) >  0;
    }

    inline bool operator>=(const WideString & text) const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
        return compare(text) >= 0;
    }

    inline bool operator< (const WideString & text) const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
        return compare(text) <  0;
    }

    inline bool operator<=(const WideString & text) const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
        return compare(text) <= 0;
    }

    inline int8_t compare(const WideString & text) const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
        return mStr.compare(text.mStr);
    }

    inline int8_t compare(const WideString & text, int32_t len) const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
        return mStr.compare(text.mStr.getBuffer(), len);
    }

    inline int8_t compare(const UChar * text) const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        return mStr.compare(text);
    }

    inline int8_t compare(const UChar * text, int32_t len) const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        return mStr.compare(text, len);
    }

    inline int8_t caseCompare(const WideString & text, int32_t options) const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
        return mStr.caseCompare(text.mStr, options);
    }

    inline int8_t caseCompare(const UChar * text, int32_t options) const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        return mStr.caseCompare(text, options);
    }

    inline UChar first() const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_ASSERT(!isEmpty());
        return mStr[0];
    }

    inline UChar last() const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_ASSERT(!isEmpty());
        return mStr[mStr.length()-1];
    }

    inline UChar operator[](int32_t offset) const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_ASSERT(offset >= 0 && offset < length());
        return mStr[offset];
    }

    inline UChar getCharAt(int32_t offset) const { return operator[](offset); }

    inline WideString & setCharAt(int32_t offset, UChar ch) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_NULLSAFE_TRUE;
        STRINGWRAPPER_ASSERT(offset >= 0 && offset < length());
        mStr.setCharAt(offset, ch);
        mStr.getTerminatedBuffer();
        return *this;
    }

    inline const UChar * getBuffer() const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_NULLSAFE_ASSERT;
        STRINGWRAPPER_ASSERT(mStr.getBuffer()[mStr.length()] == 0);
        return mStr.getBuffer();
    }

    inline UChar * getBuffer(int32_t minCapacity) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_OPENBUF_TRUE;
        return mStr.getBuffer(minCapacity+1);
    }

    inline void releaseBuffer(int32_t newLength = -1) {
        STRINGWRAPPER_OPENBUF_ASSERT_TRUE;
        mStr.releaseBuffer(newLength);
        mStr.getTerminatedBuffer();
        STRINGWRAPPER_OPENBUF_FALSE;
        STRINGWRAPPER_NULLSAFE_TRUE;
    }

    inline int32_t getCapacity() const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        return mStr.getCapacity();
    }

    inline void setCapacity(int32_t n) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        if (n+1 > mStr.getCapacity()) {
            int32_t nLen = mStr.length();
            mStr.getBuffer(n);
            mStr.releaseBuffer(nLen);
        }
    }

    inline WideString & remove() {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_NULLSAFE_TRUE;
        mStr.remove();
        mStr.getTerminatedBuffer();
        return *this;
    }

    inline WideString & remove(int32_t start, int32_t length = INT32_MAX) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_NULLSAFE_TRUE;
        mStr.remove(start, length);
        mStr.getTerminatedBuffer();
        return *this;
    }

    inline bool truncate(int32_t targetLength) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        if (!mStr.truncate(targetLength)) return false;
        STRINGWRAPPER_NULLSAFE_TRUE;
        mStr.getTerminatedBuffer();
        return true;
    }

    inline WideString & insert(int32_t offset, const WideString & srcText, int32_t srcLen = -1) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_NULLSAFE_TRUE;
        STRINGWRAPPER_OPENBUF_ASSERT_(srcText);
        return insert(offset, srcText.mStr.getBuffer(),
            srcLen == -1 ? srcText.mStr.length() : srcLen);
    }

    inline WideString & insert(int32_t offset, const UChar * srcText, int32_t srcLen = -1) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_NULLSAFE_TRUE;
        mStr.insert(offset, srcText, srcLen);
        mStr.getTerminatedBuffer();
        return *this;
    }

    inline WideString & insert(int32_t offset, UChar ch) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_NULLSAFE_TRUE;
        mStr.insert(offset, ch);
        mStr.getTerminatedBuffer();
        return *this;
    }

    inline WideString & toUpper() {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_NULLSAFE_TRUE;
        mStr.toUpper();
        mStr.getTerminatedBuffer();
        return *this;
    }

    inline WideString & toLower() {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_NULLSAFE_TRUE;
        mStr.toLower();
        mStr.getTerminatedBuffer();
        return *this;
    }

    inline WideString & trim() {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_NULLSAFE_TRUE;
        mStr.trim();
        mStr.getTerminatedBuffer();
        return *this;
    }

    inline int32_t indexOf(UChar c, int32_t start = 0) const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        return mStr.indexOf(c, start);
    }

    inline int32_t indexOf(const WideString & text, int32_t start = 0) const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
        return mStr.indexOf(text.mStr, start);
    }

    inline int32_t indexOf(const UChar * text, int32_t start = 0) const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        return mStr.indexOf(text, start);
    }

    inline int32_t indexOf(const char * text, int32_t start = 0) const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        return mStr.indexOf(text, start);
    }

    inline int32_t lastIndexOf(UChar c, int32_t start = 0) const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        return mStr.lastIndexOf(c, start);
    }

    inline int32_t lastIndexOf(const WideString & text, int32_t start = 0) const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_OPENBUF_ASSERT_(text);
        return mStr.lastIndexOf(text.mStr, start);
    }

    inline int32_t lastIndexOf(const UChar * text, int32_t start = 0) const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        return mStr.lastIndexOf(text, start);
    }

    inline int32_t lastIndexOf(const char * text, int32_t start = 0) const {
        STRINGWRAPPER_OPENBUF_ASSERT;
        return mStr.lastIndexOf(text, start);
    }

    inline WideString & replace(int32_t start, int32_t length, UChar srcChar) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_NULLSAFE_TRUE;
        return replace(start, length, &srcChar, 1);
    }

    inline WideString & replace(int32_t start, int32_t length, const WideString & srcText) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_NULLSAFE_TRUE;
        STRINGWRAPPER_OPENBUF_ASSERT_(srcText);
        return replace(start, length, srcText.mStr.getBuffer(), srcText.mStr.length());
    }

    inline WideString & replace(int32_t start, int32_t length,
        const UChar * srcText, int32_t srcLength = -1)
    {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_NULLSAFE_TRUE;
        if (srcLength == -1) srcLength = u_strlen(srcText);
        mStr.replace(start, length, srcText, srcLength);
        mStr.getTerminatedBuffer();
        return *this;
    }

    inline WideString & findAndReplace(UChar chOld, UChar chNew) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_NULLSAFE_TRUE;
        mStr.findAndReplace(chOld, chNew);
        mStr.getTerminatedBuffer();
        return *this;
    }

    inline WideString & findAndReplace(
        const WideString & oldText, const WideString & newText)
    {
        STRINGWRAPPER_OPENBUF_ASSERT;
        STRINGWRAPPER_NULLSAFE_TRUE;
        STRINGWRAPPER_OPENBUF_ASSERT_(newText);
        mStr.findAndReplace(oldText.mStr, newText.mStr);
        mStr.getTerminatedBuffer();
        return *this;
    }

    void format(const char * a_pszFormat, ...);
    void formatV(const char * a_pszFormat, va_list ap);

    struct LessNoCase {
        int32_t mOptions;
        LessNoCase(int32_t aOptions = 0) : mOptions(aOptions) { }
        bool operator()(const WideString & lhs, const WideString & rhs) const {
            return lhs.caseCompare(rhs, mOptions) < 0;
        }
    };

    enum ALIAS { USE_ALIAS };
    inline WideString(ALIAS, const UChar * text, int32_t len, bool isNullTerminated)
        : mStr(isNullTerminated, text, len)
    {
        STRINGWRAPPER_ASSERT(!isNullTerminated || mStr.getBuffer()[mStr.length()] == 0);
        STRINGWRAPPER_NULLSAFE_SET(isNullTerminated);
        STRINGWRAPPER_OPENBUF_FALSE;
    }

    inline WideString & setTo(ALIAS, const UChar * text, int32_t len, bool isNullTerminated) {
        STRINGWRAPPER_OPENBUF_ASSERT;
        mStr.setTo(isNullTerminated, text, len);
        STRINGWRAPPER_ASSERT(!isNullTerminated || mStr.getBuffer()[mStr.length()] == 0);
        STRINGWRAPPER_NULLSAFE_SET(isNullTerminated);
        return *this;
    }

    inline const UnicodeString & getUnicodeString() const { return mStr; }
};

#define LITERAL_WIDE_STRING(str)                STRINGWRAPPER_NS::WideString(str, sizeof(str)-1, US_INV)
#define NAMED_LITERAL_WIDE_STRING(name, str)    const STRINGWRAPPER_NS::WideString name(str, sizeof(str)-1, US_INV)


// ------------------------------------------------------------------------
STRINGWRAPPER_API WideString & ConvertCPtoUTF16(
    const char * src, int32_t srcLen, const char * codepage, WideString & dest);

inline WideString & ConvertCPtoUTF16(const NarrowString & src, const char * codepage, WideString & dest) {
    return ConvertCPtoUTF16(src.getBuffer(), src.length(), codepage, dest);
}

inline WideString & ConvertASCIItoUTF16(const char * src, int32_t srcLen, WideString & dest) {
    return ConvertCPtoUTF16(src, srcLen, "", dest);
}

inline WideString & ConvertASCIItoUTF16(const NarrowString & src, WideString & dest) {
    return ConvertCPtoUTF16(src.getBuffer(), src.length(), "", dest);
}

inline WideString & ConvertUTF8toUTF16(const char * src, int32_t srcLen, WideString & dest) {
    return ConvertCPtoUTF16(src, srcLen, "UTF-8", dest);
}

inline WideString & ConvertUTF8toUTF16(const NarrowString & src, WideString & dest) {
    return ConvertCPtoUTF16(src.getBuffer(), src.length(), "UTF-8", dest);
}

inline WideString & ConvertNativeToUTF16(const char * src, int32_t srcLen, WideString & dest) {
    return ConvertCPtoUTF16(src, srcLen, NULL, dest);
}

inline WideString & ConvertNativeToUTF16(const NarrowString & src, WideString & dest) {
    return ConvertCPtoUTF16(src.getBuffer(), src.length(), NULL, dest);
}

// ------------------------------------------------------------------------
STRINGWRAPPER_API bool ConvertUTF16toCP(
    const UChar * src, int32_t srcLen, const char * codepage, NarrowString & dest);

inline bool ConvertUTF16toCP(const WideString & src, const char * codepage, NarrowString & dest) {
    return ConvertUTF16toCP(src.getBuffer(), src.length(), codepage, dest);
}

inline bool ConvertUTF16toASCII(const UChar * src, int32_t srcLen, NarrowString & dest) {
    return ConvertUTF16toCP(src, srcLen, "", dest);
}

inline bool ConvertUTF16toASCII(const WideString & src, NarrowString & dest) {
    return ConvertUTF16toCP(src, "", dest);
}

inline NarrowString & ConvertUTF16toUTF8(const UChar * src, int32_t srcLen, NarrowString & dest) {
    ConvertUTF16toCP(src, srcLen, "UTF-8", dest);
    return dest;
}

inline NarrowString & ConvertUTF16toUTF8(const WideString & src, NarrowString & dest) {
    ConvertUTF16toCP(src, "UTF-8", dest);
    return dest;
}

inline bool ConvertUTF16toNative(const WideString & src, NarrowString & dest) {
    return ConvertUTF16toCP(src, NULL, dest);
}

inline bool ConvertUTF16toNative(const UChar * src, int32_t srcLen, NarrowString & dest) {
    return ConvertUTF16toCP(src, srcLen, NULL, dest);
}

#ifdef STRINGWRAPPER_NAMESPACE
}
#endif

#endif // INCLUDED_StringWrapper
