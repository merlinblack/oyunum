/* -------------------------------------------------------------------------------
 *
 * The original of this file has been previously released to the public domain by
 * Nigel Atkinson.
 *
 * See the wiki on www.ogre3d.org
 *
 * -------------------------------------------------------------------------------
 */
#ifndef EDITSTRING_H
#define EDITSTRING_H

#include <locale>
#include <string>
#include <vector>
#include <allegro5/allegro.h>

using CodePoints = std::vector<int32_t>;

class EditString
{
public:
    EditString(void) : mInsert(true), mPosition(mText.begin()), mCaret(0), mLocale_utf8( "" )
    {}

    EditString( std::string newText )
    {
        setText( newText );
    }

    ~EditString(void)
    {}


protected:

    std::locale mLocale_utf8;

    // The text for editing
    // This is a vector of 32 bit code points.
    CodePoints mText;

    // Overwrite or insert
    bool mInsert;

    // Position for insert / overwrite
    CodePoints::iterator mPosition;

    // Caret Position - for positioning the cursor.
    int mCaret;

public:
    void setText( const std::string& newText )
    {
        mText.clear();
        mText = UTF8ToCodePoints( newText );
        mPosition = mText.end();
        mCaret = (int)mText.size();
    }

    void clear()
    {
        mText.clear();
        mPosition = mText.end();
        mCaret = 0;
    }

    std::string getText() {
        return CodePointsToUTF8( mText );
    }

    bool isOverwriting() { return !mInsert; }

    bool injectKeyPress( const ALLEGRO_EVENT& event );

    // gets the current position in the text for cursor placement
    int getPosition(void) { return mCaret; }
    
    // UTF8 encode / decode
    CodePoints UTF8ToCodePoints( const std::string& utf8 );
    std::string CodePointsToUTF8( const CodePoints& codes );
};

#endif // EDITSTRING_H
