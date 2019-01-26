#include <iostream>
#include <vector>
using namespace std;

class CHashTable {
public:
    CHashTable( int n );
    ~CHashTable();
    string Find( string value );
    string Insert( string value );
    string Erase( string value );

private:
    int h1( string s );
    int h2( string s );
    void reSize();
    int realSize;
    int tableSize;
    vector <string> v;
    vector <bool> deleted;
    const int Base1 = 29;
    const int Base2 = 31;
};

CHashTable::CHashTable( int n )
{
    realSize = 0;
    tableSize = n;
    vector <string> vNew( n );
    vector <bool> deletedNew( n );
    v = vNew;
    deleted = deletedNew;
    for( int i = 0; i < n; i++ ) {
        v[i] = "#";
        deleted[i] = true;
    }
}

CHashTable::~CHashTable()
{
    v.clear();
    deleted.clear();
}

int CHashTable::h1( string s )
{
    int result = 0;
    for( int i = 0; i < s.length(); i++ ) {
        result = ( result * Base1 + s[i] ) % tableSize;
    }
    return result;
}

int CHashTable::h2( string s )
{
    int result = 0;
    for( int i = 0; i < s.length(); i++ ) {
        result = ( result * Base2 + s[i] ) % tableSize;
    }
    return ( result * 2 + 1 ) % tableSize;
}

void CHashTable::reSize()
{
    // создаем копии векторов v и deleted
    vector <string> tmpV( tableSize );
    vector <bool> tmpDeleted( tableSize );
    for( int i = 0; i < tableSize; i++ ) {
        tmpV[i] = v[i];
        tmpDeleted[i] = deleted[i];
    }

    tableSize = tableSize * 2;
    realSize = 0;
    v.resize( tableSize );
    deleted.resize( tableSize );
    for( int i = 0; i < tableSize; i++ ) {
        v[i] = "#";
        deleted[i] = true;
    }

    for( int i = 0; i < tmpV.size(); i++ ) {
        if( tmpDeleted[i] == false ) {
            string tmp = Insert( tmpV[i] );
        }
    }
}

string CHashTable::Find( string value )
{
    int x = h1( value );
    int y = h2( value );

    for( int i = 0; i < tableSize; i++ ) {
        if( v[x] != "#" ) {
            if( v[x] == value && !deleted[x] ) {
                return "OK";
            }
        } else {
            return "FAIL";
        }
        x = ( x + y ) % tableSize;
    }

    return "FAIL";
}

string CHashTable::Insert( string value )
{
    if( Find( value ) == "OK" ) {
        return "FAIL";
    }

    int x = h1( value );
    int y = h2( value );
    for( int i = 0; i < tableSize; i++ ) {
        if( deleted[x] || v[x] == "#" ) {
            v[x] = value;
            deleted[x] = false;
            realSize++;
            // если коэф. заполнения >= 3/4, то делаем перехеширование
            if( realSize * 4 >= tableSize * 3 ) {
                reSize();
            }
            return "OK";
        }
        x = ( x + y ) % tableSize;
    }
}

string CHashTable::Erase( string value )
{
    if( Find( value ) == "FAIL" ) {
        return "FAIL";
    }

    int x = h1( value );
    int y = h2( value );
    for( int i = 0; i < tableSize; i++ ) {
        if( v[x] != "#" ) {
            if( v[x] == value ) {
                deleted[x] = true;
                realSize--;
            }
        }
        x = ( x + y ) % tableSize;
    }

    return "OK";
}

int main()
{
    CHashTable table( 8 );
    string operation, word;

    while( cin >> operation >> word ) {
        if( operation == "+" ) {
            cout << table.Insert( word ) << endl;
        }
        if( operation == "-" ) {
            cout << table.Erase( word ) << endl;
        }
        if( operation == "?" ) {
            cout << table.Find( word ) << endl;
        }
    }
}