/* -------------------------------------------------------------------------------
 *
 * The original of this file has been previously released to the public domain by
 * Nigel Atkinson.
 *
 * See the wiki on www.ogre3d.org
 *
 * -------------------------------------------------------------------------------
 */


#include "editstring.h"

CodePoints EditString::UTF8ToCodePoints( const std::string& utf8 )
{
    CodePoints codes;
    ALLEGRO_USTR* str = al_ustr_new( utf8.c_str() );
    int pos = 0;
    int32_t code = al_ustr_get_next( str, &pos );

    while( code != -1 )
    {
        codes.push_back( code );
        code = al_ustr_get_next( str, &pos );
    }

    al_ustr_free( str );

    return codes;
}

std::string EditString::CodePointsToUTF8( const CodePoints& codes )
{
    ALLEGRO_USTR* str = al_ustr_new("");
    for( const auto& code : codes )
    {
        al_ustr_append_chr( str, code );
    }

    std::string ret( al_cstr( str ) );

    al_ustr_free( str );

    return ret;
}

// Process a key press.  Return true if it was used.

bool EditString::injectKeyPress( const ALLEGRO_EVENT& event  )
{
    bool keyUsed = true;

    switch( event.keyboard.keycode )
    {
        case ALLEGRO_KEY_BACKSPACE:
            if( mPosition != mText.begin() )
            {
                mPosition = mText.erase( --mPosition );
                --mCaret;
            }
            break;

        case ALLEGRO_KEY_INSERT:
            mInsert = ! mInsert;
            break;

        case ALLEGRO_KEY_HOME:
            mPosition = mText.begin();
            mCaret = 0;
            break;

        case ALLEGRO_KEY_END:
            mPosition = mText.end();
            mCaret = (int)mText.size();
            break;

        case ALLEGRO_KEY_LEFT:
            if( mPosition != mText.begin() )
            {
                mPosition--;
                mCaret--;
            }
            break;

        case ALLEGRO_KEY_RIGHT:
            if( mPosition != mText.end() )
            {
                mPosition++;
                mCaret++;
            }
            break;

        case ALLEGRO_KEY_DELETE:
            if( mPosition != mText.end() )
                mPosition = mText.erase( mPosition );
            break;

        default:
            keyUsed = false;
            break;
    }

    if( keyUsed == false )
    {
        int32_t codepoint = event.keyboard.unichar;
        if( isgraph( (wchar_t)codepoint, mLocale_utf8 ) || isspace( (wchar_t)codepoint ) )
        {
            if( mInsert || mPosition == mText.end() )
            {
                mPosition = mText.insert( mPosition, codepoint );
            }
            else
            {
                *mPosition = codepoint;
            }
            mPosition++;
            mCaret++;

            keyUsed = true;
        }
    }

    return keyUsed;
}

