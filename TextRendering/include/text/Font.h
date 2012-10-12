/*
 Copyright (c) 2010-2012, Paul Houx - All rights reserved.
 This code is intended for use with the Cinder C++ library: http://libcinder.org

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that
 the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and
	the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
	the following disclaimer in the documentation and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include "cinder/Cinder.h"
#include "cinder/ImageIo.h"
#include "cinder/Surface.h"
#include "cinder/Utilities.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/Texture.h"

#include <map>

namespace ph { namespace text {

typedef std::shared_ptr<class Font> FontRef;

class Font
{
public:
	// stores the font metrics for a single character
	struct Metrics {
		float x;	// x - of character in texture
		float y;	// y - of character in texture
		float w;	// width - of character in texture
		float h;	// height - of character in texture
		float dx;	// xoffset - adjusts character positioning
		float dy;	// yoffset - adjusts character positioning
		float d;	// xadvance - adjusts character positioning
	};

	typedef std::map<uint16_t, Metrics>	MetricsMap;
public:
	Font(void);
	~Font(void);

	//! creates a font from the two files generated by LoneSock's SDFont.exe
	void create( const ci::DataSourceRef png, const ci::DataSourceRef txt );
	//! reads a binary font file created using 'writeBinary'
	void read( const ci::DataSourceRef source );
	//! writes the font to a binary file
	void write( const ci::DataTargetRef target );

	//!
	std::string getFamily() const { return mFamily; }

	//!
	float		getAscent( float fontSize=12.0f ) const { return mAscent * (fontSize / mFontSize); }
	//!
	float		getDescent( float fontSize=12.0f ) const { return mDescent * (fontSize / mFontSize); }
	//!
	float		getLeading( float fontSize=12.0f ) const { return mLeading * (fontSize / mFontSize); }
	//!
	float		getSpaceWidth( float fontSize=12.0f ) const { return mSpaceWidth * (fontSize / mFontSize); }

	//!
	bool		contains(uint16_t charcode) const { return (mMetrics.find(charcode) != mMetrics.end()); }

	//!
	ci::Rectf	getBounds(uint16_t charcode, float fontSize=12.0f) const;
	//!
	ci::Rectf	getTexCoords(uint16_t charcode) const;
	//!
	float		getAdvance(uint16_t charcode, float fontSize=12.0f) const;

	//!
	void		enableAndBind() const { if(mTexture) mTexture.enableAndBind(); }
	//!
	void		bind(GLuint textureUnit=0) const { if(mTexture) mTexture.bind(textureUnit); }
	//!
	void		unbind(GLuint textureUnit=0) const  { if(mTexture) mTexture.unbind(textureUnit); }

	//!
	ci::Rectf	measure( const std::string &text, float fontSize=12.0f ) const { return measure( ci::toUtf16(text), fontSize ); }
	//!
	ci::Rectf	measure( const std::wstring &text, float fontSize=12.0f ) const;

	//!
	float		measureWidth( const std::string &text, float fontSize=12.0f, bool precise=true ) const { return measureWidth( ci::toUtf16(text), fontSize, precise ); }
	//!
	float		measureWidth( const std::wstring &text, float fontSize=12.0f, bool precise=true ) const;

	/*//!
	ci::Vec2f	render( ci::TriMesh2d &mesh, const std::string &text, float fontSize=12.0f, const ci::Vec2f &origin=ci::Vec2f::zero() ) {
		render( mesh, ci::toUtf16(text), fontSize, origin );
	}
	//!
	ci::Vec2f	render( ci::TriMesh2d &mesh, const std::wstring &text, float fontSize=12.0f, const ci::Vec2f &origin=ci::Vec2f::zero() );
	//*/
protected:
	bool				mInvalid;

	std::string			mFamily;

	//! calculated by the 'measure' function
	float				mFontSize;
	float				mLeading;
	float				mAscent;
	float				mDescent;
	float				mSpaceWidth;

	ci::Surface			mSurface;
	ci::gl::Texture		mTexture;

	MetricsMap			mMetrics;
};

class FontExc : public std::exception {
 public:
	virtual const char* what() const throw() { return "Font exception"; }
};

class FontInvalidSourceExc : public FontExc {
 public:
	virtual const char* what() const throw() { return "Font exception: could not load from the specified source"; }
};

class FontInvalidTargetExc : public FontExc {
 public:
	virtual const char* what() const throw() { return "Font exception: could not write to the specified target"; }
};

} } // namespace ph::text