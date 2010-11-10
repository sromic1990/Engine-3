//----------------------------------------------------------------------------------------------------------------------
// String.h
//
// Copyright (C) 2010 WhiteMoon Dreams, Inc.
// All Rights Reserved
//----------------------------------------------------------------------------------------------------------------------

#pragma once
#ifndef LUNAR_CORE_STRING_H
#define LUNAR_CORE_STRING_H

#include "Core/Core.h"

#include "Platform/Types.h"
#include "Platform/DynArray.h"
#include "Core/HashFunctions.h"

namespace Lunar
{
    /// Base string class.
    template< typename CharType, typename Allocator = DefaultAllocator >
    class StringBase
    {
    public:
        /// @name Construction/Destruction
        //@{
        StringBase();
        explicit StringBase( const CharType* pString );
        StringBase( const CharType* pString, size_t size );
        //@}

        /// @name String Operations
        //@{
        size_t GetSize() const;
        bool IsEmpty() const;

        size_t GetCapacity() const;
        void Reserve( size_t capacity );
        void Trim();

        const CharType* GetData() const;

        void Clear();

        CharType& GetElement( size_t index );
        const CharType& GetElement( size_t index ) const;

        void Add( CharType character, size_t count = 1 );
        void Add( const CharType* pString );
        void Insert( size_t index, CharType character, size_t count = 1 );
        void Insert( size_t index, const CharType* pString );
        void Remove( size_t index, size_t count = 1 );

        CharType& GetFirst();
        const CharType& GetFirst() const;
        CharType& GetLast();
        const CharType& GetLast() const;
        size_t Push( CharType character );
        void Pop();

        void Format( const CharType* pFormatString, ... );
        //@}

        /// @name Parsing
        //@{
        size_t Find( CharType character, size_t startIndex = 0 ) const;
        size_t FindReverse( CharType character, size_t startIndex = Invalid< size_t >() ) const;

        size_t FindAny(
            const CharType* pCharacters, size_t startIndex = 0, size_t characterCount = Invalid< size_t >() ) const;
        size_t FindAnyReverse(
            const CharType* pCharacters, size_t startIndex = Invalid< size_t >(),
            size_t characterCount = Invalid< size_t >() ) const;

        bool Contains( CharType character ) const;
        template< typename OtherAllocator > bool Contains(
            const StringBase< CharType, OtherAllocator >& rString ) const;
        bool Contains( const CharType* pString ) const;

        template< typename OtherAllocator > bool StartsWith(
            const StringBase< CharType, OtherAllocator >& rString ) const;
        bool StartsWith( const CharType* pString ) const;

        template< typename OtherAllocator > bool EndsWith(
            const StringBase< CharType, OtherAllocator >& rString ) const;
        bool EndsWith( const CharType* pString ) const;

        template< typename ArrayType, typename ArrayAllocator > void Split(
            DynArray< ArrayType, ArrayAllocator >& rStringResults, CharType separator,
            bool bCombineAdjacentSeparators = false ) const;
        template< typename ArrayType, typename ArrayAllocator > void Split(
            DynArray< ArrayType, ArrayAllocator >& rStringResults, const CharType* pSeparators,
            size_t separatorCount = Invalid< size_t >(), bool bCombineAdjacentSeparators = false ) const;
        template< typename ArrayType, typename ArrayAllocator, typename StringAllocator > void Split(
            DynArray< ArrayType, ArrayAllocator >& rStringResults,
            const StringBase< CharType, StringAllocator >& rSeparators, bool bCombineAdjacentSeparators = false ) const;
        //@}

        /// @name Overloaded Operators
        //@{
        const CharType* operator*() const;

        CharType& operator[]( ptrdiff_t index );
        const CharType& operator[]( ptrdiff_t index ) const;

        StringBase& operator=( CharType character );
        StringBase& operator=( const CharType* pString );
        template< typename OtherAllocator > StringBase& operator=(
            const StringBase< CharType, OtherAllocator >& rSource );

        bool operator==( const CharType* pString ) const;
        template< typename OtherAllocator > bool operator==(
            const StringBase< CharType, OtherAllocator >& rString ) const;

        bool operator!=( const CharType* pString ) const;
        template< typename OtherAllocator > bool operator!=(
            const StringBase< CharType, OtherAllocator >& rString ) const;
        //@}

    protected:
        /// Allocated string buffer.
        DynArray< CharType, Allocator > m_buffer;

        /// @name Protected String Operations
        //@{
        template< typename OtherAllocator > void Add( const StringBase< CharType, OtherAllocator >& rString );
        template< typename OtherAllocator > void Insert(
            size_t index, const StringBase< CharType, OtherAllocator >& rString );
        //@}

        /// @name Protected Overloaded Operators
        //@{
        //@}
    };

    /// 8-bit character string class.
    class LUNAR_CORE_API CharString : public StringBase< char, DefaultAllocator >
    {
    public:
        /// @name Construction/Destruction
        //@{
        CharString();
        explicit CharString( const char* pString );
        CharString( const char* pString, size_t size );
        CharString( const CharString& rSource );
        //@}

        /// @name String Operations
        //@{
        void Add( char character, size_t count = 1 );
        void Add( const char* pString );
        void Add( const CharString& rString );

        void Insert( size_t index, char character, size_t count = 1 );
        void Insert( size_t index, const char* pString );
        void Insert( size_t index, const CharString& rString );
        //@}

        /// @name Parsing
        //@{
        size_t FindAny(
            const char* pCharacters, size_t startIndex = 0, size_t characterCount = Invalid< size_t >() ) const;
        size_t FindAny( const CharString& rCharacters, size_t startIndex = Invalid< size_t >() ) const;

        size_t FindAnyReverse(
            const char* pCharacters, size_t startIndex = Invalid< size_t >(),
            size_t characterCount = Invalid< size_t >() ) const;
        size_t FindAnyReverse( const CharString& rCharacters, size_t startIndex = Invalid< size_t >() ) const;
        //@}

        /// @name Overloaded Operators
        //@{
        CharString& operator=( char character );
        CharString& operator=( const char* pString );
        CharString& operator=( const CharString& rSource );

        CharString& operator+=( char character );
        CharString& operator+=( const char* pString );
        CharString& operator+=( const CharString& rString );

        bool operator==( const char* pString ) const;
        bool operator==( const CharString& rString ) const;
        bool operator!=( const char* pString ) const;
        bool operator!=( const CharString& rString ) const;
        //@}
    };

    /// Wide character string class.
    class LUNAR_CORE_API WideString : public StringBase< wchar_t, DefaultAllocator >
    {
    public:
        /// @name Construction/Destruction
        //@{
        WideString();
        explicit WideString( const wchar_t* pString );
        WideString( const wchar_t* pString, size_t size );
        WideString( const WideString& rSource );
        //@}

        /// @name String Operations
        //@{
        void Add( wchar_t character, size_t count = 1 );
        void Add( const wchar_t* pString );
        void Add( const WideString& rString );

        void Insert( size_t index, wchar_t character, size_t count = 1 );
        void Insert( size_t index, const wchar_t* pString );
        void Insert( size_t index, const WideString& rString );
        //@}

        /// @name Parsing
        //@{
        size_t FindAny(
            const wchar_t* pCharacters, size_t startIndex = 0, size_t characterCount = Invalid< size_t >() ) const;
        size_t FindAny( const WideString& rCharacters, size_t startIndex = Invalid< size_t >() ) const;

        size_t FindAnyReverse(
            const wchar_t* pCharacters, size_t startIndex = Invalid< size_t >(),
            size_t characterCount = Invalid< size_t >() ) const;
        size_t FindAnyReverse( const WideString& rCharacters, size_t startIndex = Invalid< size_t >() ) const;
        //@}

        /// @name Overloaded Operators
        //@{
        WideString& operator=( wchar_t character );
        WideString& operator=( const wchar_t* pString );
        WideString& operator=( const WideString& rSource );

        WideString& operator+=( wchar_t character );
        WideString& operator+=( const wchar_t* pString );
        WideString& operator+=( const WideString& rString );

        bool operator==( const wchar_t* pString ) const;
        bool operator==( const WideString& rString ) const;
        bool operator!=( const wchar_t* pString ) const;
        bool operator!=( const WideString& rString ) const;
        //@}
    };

    /// Default CharString hash.
    template<>
    class LUNAR_CORE_API Hash< CharString >
    {
    public:
        size_t operator()( const CharString& rKey ) const;
    };

    /// Default WideString hash.
    template<>
    class LUNAR_CORE_API Hash< WideString >
    {
    public:
        size_t operator()( const WideString& rKey ) const;
    };

#if HELIUM_UNICODE
    /// Default string class.
    typedef WideString String;
#else  // HELIUM_UNICODE
    /// Default string class.
    typedef CharString String;
#endif  // HELIUM_UNICODE
}

#include "Core/String.inl"

#endif  // LUNAR_CORE_STRING_H
