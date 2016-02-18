#include "UtilityString.h"

#include <assert.h>
#include <string.h>

UtilityString::UtilityString()
	: m_pData (NULL),
	  m_nLength(0)
{
}

UtilityString::UtilityString( const char * str )
{
	if ( str == NULL ) {
		m_pData = new char[1];
		m_pData[0] = '\0';
		m_nLength = 0;
	}
	else {
		m_nLength = strlen( str );
		m_pData = new char[ m_nLength + 1 ];
		strcpy( m_pData, str );
		m_pData[ m_nLength ] = '\0';
	}
}

UtilityString::UtilityString( const UtilityString & str )
{
	m_nLength = str.m_nLength;
	m_pData = new char[ m_nLength + 1 ];
	strcpy( m_pData, str.m_pData );
}

UtilityString::~UtilityString()
{
	if ( m_pData ) {
		delete [] m_pData;
		m_pData = NULL;

		m_nLength = 0;
	}
}

istream & operator >> ( istream & in, UtilityString & str )
{
	char temp[100]; // 临时字符串数组
	if ( in >> temp )
	{
		delete [] str.m_pData;
		str.m_nLength = strlen( temp );
		str.m_pData = new char[ str.m_nLength + 1 ];
		strcpy( str.m_pData, temp );
	}

	return in;
}

ostream & operator << ( ostream & out, UtilityString & str )
{
	if ( str.m_pData != NULL )
		out << str.m_pData;
	return out;
}

UtilityString operator + ( const UtilityString & s1, const UtilityString & s2 )
{
	UtilityString retStr;
    retStr.m_nLength = s1.m_nLength + s2.m_nLength;
    retStr.m_pData = new char[ retStr.m_nLength + 1 ];
    strcpy( retStr.m_pData, s1.m_pData );
    strcat( retStr.m_pData, s2.m_pData);
    return retStr;
}

bool operator == ( const UtilityString & s1, const UtilityString & s2 )
{
	return ( strcmp( s1.m_pData, s2.m_pData ) == 0 );
}

bool operator != ( const UtilityString & s1, const UtilityString & s2 )
{
	return ( strcmp( s1.m_pData, s2.m_pData ) != 0 );
}

bool operator < ( const UtilityString & s1, const UtilityString & s2 )
{
	return ( strcmp( s1.m_pData, s2.m_pData ) < 0 );
}

bool operator <= ( const UtilityString & s1, const UtilityString & s2 )
{
	return ( strcmp( s1.m_pData, s2.m_pData ) <= 0 );
}

bool operator > ( const UtilityString & s1, const UtilityString & s2 )
{
	return ( strcmp( s1.m_pData, s2.m_pData ) > 0 );
}

bool operator >= ( const UtilityString & s1, const UtilityString & s2 )
{
	return ( strcmp( s1.m_pData, s2.m_pData ) >= 0 );
}

//
char & UtilityString::operator [] ( const int index )
{
	assert( index >= 0 && index <= m_nLength );
	return m_pData[ index ];
}

const char & UtilityString::operator [] ( const int index ) const
{
	assert( index >= 0 && index <= m_nLength );
	return m_pData[ index ];
}

UtilityString & UtilityString::operator = ( const UtilityString & str )
{
	// 检查自赋值
	if ( this == &str ) {
		return *this;
	}
	delete [] m_pData;
	m_nLength = strlen( str.m_pData );
	m_pData = new char[ m_nLength + 1 ];
	strcpy( m_pData, str.m_pData );
	return *this;
}

UtilityString & UtilityString::operator += ( const UtilityString & str )
{
	if ( this == &str )
	{
		UtilityString copy( str );
		return ( *this += copy);
	}
	m_nLength += str.m_nLength;

	char * pOld = m_pData;
	m_pData = new char[ m_nLength + 1 ];
	strcpy( m_pData, pOld );
	delete [] pOld;
	strcat( m_pData, str.m_pData );

	return *this;
}

////////////////////
bool UtilityString::empty()
{
	return ( m_nLength == 0 ) ? true : false;
}

int UtilityString::length()
{
	return m_nLength;
}

const char * UtilityString::c_str()
{
	return m_pData;
}

UtilityString UtilityString::substr( int pos, int n  )
{
	assert( pos + n < m_nLength );

	UtilityString retStr;
	retStr.m_nLength = n;
	retStr.m_pData = new char[ retStr.m_nLength + 1 ];
	for( int i=0; i<n; ++i )
	{
		retStr[i] = this->m_pData[ pos + i ];
	}
	retStr[n] = '\0';

	return retStr;
}

UtilityString & UtilityString::append( const UtilityString & str )
{
	*this += str;
	return *this;
}

// 在下标位 pos 的元素之前插入 UtilityString 对象 str 的副本
UtilityString & UtilityString::insert( int pos, const UtilityString & str )
{
	assert( pos < m_nLength );

	char * pOld = m_pData;

	m_nLength += str.m_nLength;
	m_pData = new char [ m_nLength + 1 ];
	for( int i=0; i != pos; ++i )
	{
		*( m_pData + i ) = *( pOld + i );
	}
	for( int i=pos; i != str.m_nLength + pos; ++i )
	{
		*( m_pData + i ) = *( str.m_pData + i - pos );
	}
	for( int i=str.m_nLength + pos; i != m_nLength; ++i )
	{
		*( m_pData + i ) = *( pOld + i - str.m_nLength );
	}
	*( m_pData + m_nLength ) = '\0';
	delete [] pOld;

	return *this;
}

// 替换
// std::string base="The quick brown fox jumps over a lazy dog.";
// str.assign(base,10,9); --> "brown fox"
UtilityString & UtilityString::assign( const UtilityString & str, int pos, int len )
{
	if ( m_nLength < len )
	{
		m_nLength = len;
		delete [] m_pData;
		m_pData = new char[ m_nLength + 1 ];
	}
	for( int i=0; i != len; ++i )
	{
		*( m_pData + i ) = str[ pos + i ];
	}
	*( m_pData + m_nLength ) = '\0';
	return *this;
}

// 删除从 pos 开始的 len 个字符
UtilityString & UtilityString::erase( int pos, int len )
{
	assert( pos + len <= m_nLength );
	int index = pos + len;
	while ( *( m_pData + index ) != '\0' )
	{
		*( m_pData + index - len ) = *( m_pData + index );
		++index;
	}
	*( m_pData + index - len ) = '\0';

	return *this;
}

int UtilityString::CompareNoCase( const char * s1 )
{
	char *ptr = m_pData;

	int c1, c2;
	do {
		c1 = tolower( *ptr++ );
		c2 = tolower( *s1++ );
	} while ( c1 == c2 && c1 != '\0' ); //

	return c1 - c2;
}


#define MAX_BUFF 32
int UtilityString::GetByteArray(BYTE * buff, int uiSize, char div)
{
	char szBuff[MAX_BUFF] = {0};
	char *obj = szBuff;
    char *src = m_pData;
    BYTE *bys = buff;
    int  sub  = 0;

    while ( *src != '\0' )
    {
        if ( *src == div )
        {
            if ( sub < uiSize )
            {
                if ( (obj - szBuff) < MAX_BUFF && (obj - szBuff) != 0 )
                {
                    *bys = (BYTE) atoi(szBuff);
                    ++bys;
                    ++sub;
                }
            }

            memset( szBuff, 0x0, sizeof(szBuff) );
            obj = szBuff;
        }
        else if ( (obj - szBuff) < MAX_BUFF )
        {
            *obj = *src;
            ++obj;
        }
        ++src;
    }
    if ( sub < uiSize )
    {
        if ( (obj - szBuff) < MAX_BUFF && (obj - szBuff) != 0)
        {
            *bys = (BYTE) atoi(szBuff);
            ++sub;
        }
    }
    return sub;
}

int UtilityString::GetIntArray(int * buff, int uiSize, char div)
{
	char szBuff[MAX_BUFF] = {0};
	char *obj = szBuff;
    char *src = m_pData;
    int  *uis = buff;
    int  sub  = 0;

    while ( *src != '\0' )
    {
        if ( *src == div )
        {
            if ( sub < uiSize )
            {
                if ( (obj - szBuff) < MAX_BUFF && (obj - szBuff) != 0 )
                {
                    *uis = (int) atoi(szBuff);
                    ++uis;
                    ++sub;
                }
            }

            memset( szBuff, 0x0, sizeof(szBuff) );
            obj = szBuff;
        }
        else if ( (obj - szBuff) < MAX_BUFF )
        {
            *obj = *src;
            ++obj;
        }
        ++src;
    }
    if ( sub < uiSize )
    {
        if ( (obj - szBuff) < MAX_BUFF && (obj - szBuff) != 0)
        {
            *uis = (int) atoi(szBuff);
            ++sub;
        }
    }
    return sub;
}


int UtilityString::SetByteArray(BYTE * buff, int uiSize, char div)
{
    char szBuff[MAX_BUFF] = {0};

    for (int i=0; i<uiSize; i++)
    {
        memset( szBuff, 0x0, sizeof(szBuff) );

        sprintf(szBuff, "%d%c", buff[i], div);

        this->append(szBuff);
    }

    int szLen = this->length() - 1;

    *( m_pData + szLen ) = '\0';

    return szLen;
}
