/***************************************************************************
Text in a box
Rectangle of width w, height h

given:
int getWidth(int fontSize, char c);
int getHeight(int fontSize);

write:
int getMaxFontSize(int w, int h, string s);

"This is my input string"

Example where font size is too big:
        w
 ------------------
 |                |
h| This is my inpu|
 |                |
 ------------------

Example where font size could be bigger:

-----------------------------------------------
|                                             |
| This is my input string                     |
|                                             |
|                                             |
|                                             |
-----------------------------------------------

Assume valid font sizes are in [1, N]

Part II:
Now allow line wrap at any character:

------------
|This is m |
|y input st|
|ring      |
------------

*/

#include <string>

using namespace std;

//N is max font size
#define N 100
#define getMidFont(left, right) (((left) + (right)) >> 1)

//FWD declarations
int getWidth(int fontSize, char c);
int getHeight(int fontSize);
bool canFit(int w, int h, const string s, int fontSize);
bool canFitMultiline(int w, int h, const string s, int fontSize);

/*
 * Take the mid point of font size
 *     a. See if that font is with in the "h" that is passed, i.e midfont <= h
 *     b. Calculate the width for input string with this font (midfont)
 */
int getMaxFontSize(int w, int h, const string s)
{
    int left = 1;
    int right = N;
    int midFont = 0;
    
    //validate the rect
    if (w > 0 && h > 0 && s.length() != 0) {
        do {
            midFont = getMidFont(left, right);
            if (canFit(w, h, s, midFont)) {
                left = midFont;
            } else {
                right = midFont;
            }
        } while (left < right);
   }
   
   return left;
}
 
/*
 * For given rectangle, see if that can fit the given string
 */
bool canFit(int w, int h, const string s, int fontSize)
{
    bool fits = false;
    int  totalWidth = 0;
    int  i = 0;
    
    if ( h <= getHeight(fontSize) ) {
        for (i = 0; i < s.size(); i ++) {
            totalWidth += getWidth(fontSize, s[i]);
        }
        if (totalWidth <= w) {
            fits = true;
        }
    }
    return fits;
}   

bool canFitMultiline(int w, int h, const string s, int fontSize)
{
    bool fits = false;
    int  totalWidth = 0;
    int  width  = 0;
    int  line = 0;
    int  i = 0;
    
    if ( h <= getHeight(fontSize) ) {
        for (i = 0; i < s.size(); i ++) {
            width = getWidth(fontSize, s[i]);
            totalWidth += width;
            if (totalWidth > w) {
                totalWidth = width;
                line++;
            }
        }
        if (h >= line * getHeight(fontSize)) {
            fits = true;
        }
    }

    return fits;
}

int main()
{
    return 0;
}

