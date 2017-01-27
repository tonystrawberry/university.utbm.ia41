/*
 * $Id$
 *
 * Ipseity is an open-source platform dedicated to the development and
 * validation of Artificial Intelligence techniques in Multiagent Systems.
 * It is developed in C++ with the Qt framework (http://qt-project.org).
 * More details on <http://www.ipseity-project.com>
 * Copyright (C) 2015, 2014, 2013 Ipseity Core Developers
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include "XMLScanner.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>


//#include "ApplicationLogFile.h"

//TODO: break parser into header/source
//TODO: handle <?xml tags
//TODO: handle <! -- comments
//TODO: better error messages (line, char, context) (could be done outside parser)


#define CHAR_TAGSTART(c) ((c)=='<')
#define CHAR_CLOSE(c) ((c)=='/')
#define CHAR_TAGEND(c) ((c)=='>')
#define CHAR_EQUALS(c) ((c)=='=')
#define CHAR_SPACE(c) (isspace(c))
#define CHAR_ID(c) (((c)=='-')||((c)==':')||((c)=='_')||isalnum(c))
#define CHAR_QUOTE(c) ((c)=='\"')


char XMLScanner::Text[1024];


XMLScanner::XMLScanner( const char* file, const char* base_path ) : VFile( file, base_path )
{
	init();
}


void
XMLScanner::init()
{
	buf_ = (char*) mem;
	mem = NULL;
	end_ = buf_+size;

	if( buf_ )
	{
		m_Loaded = true;
	}
	else
	{
		m_Loaded = false;
	}

  unsigned char const * tmp = reinterpret_cast< unsigned char const * >( buf_ );
  if( size > 2 && tmp[0] == 0xef && tmp[1] == 0xbb && tmp[2] == 0xbf ) {
    //  this is a UTF-8 file with the "Unicode" marker char up front
    buf_ += 3;
  }
  start_ = buf_;

  stackPtr_ = 0;
  stack_[0].kind = Token::Error;
  stack_[0].key = 0;
  stack_[0].keySize = 0;
  stack_[0].value = 0;
  stack_[0].valueSize = 0;
}


XMLScanner::~XMLScanner()
{
	if( m_Loaded )
	{
		delete start_;
	}
}


bool XMLScanner::isLoaded() const
{
	return m_Loaded;
}


#define SKIP_SPACE(x) { \
  while( buf_ < end_ && CHAR_SPACE( *buf_ ) ) { \
    ++buf_; \
  } \
  if( buf_ == end_ ) { \
    error = "unterminated " #x ; \
    goto return_error; \
  } \
}


bool XMLScanner::nextToken( XMLScanner::Token * outToken )
{
  if( buf_ == end_ ) {
    outToken->kind = Token::EndOfFile;
    outToken->key = 0;
    outToken->keySize = 0;
    outToken->value = 0;
    outToken->valueSize = 0;
    return false;
  }

  char const * error = "unhandled error";

  bool allowTagOrClose = false;
  bool allowCDATA = false;
  bool allowWhitespace = false;
  bool allowAttribute = false;

  switch( stack_[stackPtr_].kind ) {
  case Token::Whitespace:
    allowTagOrClose = true;
    allowCDATA = true;
    break;
  case Token::CDATA:
    allowTagOrClose = true;
    allowWhitespace = true;
    break;
  case Token::Tag:
  case Token::Attribute:
    allowAttribute = true;
    break;
  default:
  case Token::EndOfFile:
    assert( !"internal error in XMLScanner" );
  case Token::Error:
  case Token::Close:
    allowTagOrClose = true;
    allowCDATA = true;
    allowWhitespace = true;
    break;
  }

finished_a_tag:

  char const * beg = buf_;

  if( allowTagOrClose ) {
    if( *buf_ == '<' ) {
      //  Tag or Close
      ++buf_;
      bool isClose = false;
      if( CHAR_CLOSE( *buf_ ) ) {
        //  Close
        isClose = true;
        ++buf_;
      }
      beg = buf_;
      if( buf_ >= end_ || !CHAR_ID( *buf_ ) ) {
        error = "unterminated tag";
        goto return_error;
      }
      while( buf_ < end_ && CHAR_ID( *buf_ ) ) {
        ++buf_;
      }
      if( buf_ == beg ) {
        error = "unterminated tag";
        goto return_error;
      }
      if( isClose ) {
        if( stackPtr_ == 0 ) {
          error = "close tag with no open";
          goto return_error;
        }
        --stackPtr_;
        if( buf_-beg != stack_[stackPtr_].keySize || memcmp( beg, stack_[stackPtr_].key, buf_-beg ) ) {
          //  TODO: a lenient parser would auto-close some number of tags if necessary,
          //  and ignore closes that have no tags on the stack.
          error = "mis-matching close tag";
          goto return_error;
        }
        stack_[stackPtr_].kind = Token::Close;
        stack_[stackPtr_].value = beg;
        stack_[stackPtr_].valueSize = buf_-beg;
        //  retain the other values from the Tag, as it points to the right tag name (and that's useful)
        SKIP_SPACE(tag);
        if( !CHAR_TAGEND( *buf_ ) ) {
          error = "improper close tag format";
          goto return_error;
        }
        ++buf_;
      }
      else {
        if( stackPtr_ == StackDepth-1 ) {
          error = "too deep tag nesting";
          goto return_error;
        }
        stack_[stackPtr_].kind = Token::Tag;
        stack_[stackPtr_].key = beg;
        stack_[stackPtr_].keySize = buf_-beg;
        stack_[stackPtr_].value = 0;
        stack_[stackPtr_].valueSize = 0;
        ++stackPtr_;
        stack_[stackPtr_] = stack_[stackPtr_-1];
      }
      goto return_ok;
    }
  }
  assert( !(allowAttribute && allowWhitespace) );
  if( allowWhitespace ) {
    while( buf_ < end_ && CHAR_SPACE( *buf_ ) ) {
      ++buf_;
    }
    if( buf_ > beg ) {
      stack_[stackPtr_].kind = Token::Whitespace;
      stack_[stackPtr_].key = 0;
      stack_[stackPtr_].keySize = 0;
      stack_[stackPtr_].value = beg;
      stack_[stackPtr_].valueSize = buf_-beg;
      goto return_ok;
    }
  }
  assert( !(allowAttribute && allowCDATA) );
  if( allowCDATA ) {
    //  CDATA
    char * space = 0;
    while( buf_ < end_ ) {
      if( CHAR_SPACE( *buf_ ) && !space ) {
        space = buf_;
      }
      else if( CHAR_TAGSTART( *buf_ ) ) {
        if( space ) {
          buf_ = space;
        }
        break;
      }
      else {
        space = 0;
      }
      ++buf_;
    }
    stack_[stackPtr_].kind = Token::CDATA;
    stack_[stackPtr_].key = 0;
    stack_[stackPtr_].keySize = 0;
    stack_[stackPtr_].value = beg;
    stack_[stackPtr_].valueSize = buf_-beg;
    goto return_ok;
  }
  //  allowAttribute really means we're inside an open tag
  if( allowAttribute ) {
    SKIP_SPACE(attribute);
    beg = buf_;
    if( CHAR_CLOSE( *buf_ ) ) {
      if( ++buf_ == end_ ) {  //  skip close
        error = "unterminated tag";
        goto return_error;
      }
      if( !CHAR_TAGEND( *buf_ ) ) {
        error = "XML syntax error (bracket must follow closing slash)";
        goto return_error;
      }
      ++buf_; // skip tagend
      assert( stackPtr_ > 0 );
      if( stackPtr_ == 0 ) {
        error = "internal error (self-closing tag with no stack)";
        goto return_error;
      }
      --stackPtr_;
      stack_[stackPtr_].kind = Token::Close;
      stack_[stackPtr_].value = buf_-2;
      stack_[stackPtr_].valueSize = 2;
      goto return_ok;
    }
    else if( CHAR_TAGEND( *buf_ ) ) {
      ++buf_;
      allowAttribute = false;
      allowCDATA = true;
      allowWhitespace = true;
      allowTagOrClose = true;
      goto finished_a_tag;
    }
    else if( !CHAR_ID( *buf_ ) ) {
      error = "illegal attribute name";
      goto return_error;
    }
    stack_[stackPtr_].key = buf_;
    // gather attribute identifier
    while( buf_ < end_ && CHAR_ID( *buf_ ) ) {
      ++buf_;
    }
    stack_[stackPtr_].keySize = buf_ - stack_[stackPtr_].key;
    if( buf_ == end_ ) {
      error = "unterminated attribute";
      goto return_error;
    }
    SKIP_SPACE(attribute);
    if( !CHAR_EQUALS( *buf_ ) ) {
      error = "expected attribute equals sign";
      goto return_error;
    }
    ++buf_; // skip equals
    SKIP_SPACE(attribute);
    if( !CHAR_QUOTE( *buf_ ) ) {
      error = "expected attribute quote";
      goto return_error;
    }
    ++buf_; // skip quote
    stack_[stackPtr_].value = buf_;
    // gather attribute value
    while( buf_ < end_ && !CHAR_QUOTE( *buf_ ) ) {
      ++buf_;
    }
    if( buf_ == end_ ) {
      error = "unterminated attribute value";
      goto return_error;
    }
    stack_[stackPtr_].valueSize = buf_ - stack_[stackPtr_].value;
    stack_[stackPtr_].kind = Token::Attribute;
    ++buf_; // skip last quote
    goto return_ok;
  }

  error = "XML format error";

return_error:
  stack_[stackPtr_].kind = Token::Error;
  stack_[stackPtr_].key = beg;
  stack_[stackPtr_].keySize = buf_-beg;
  stack_[stackPtr_].value = error;
  stack_[stackPtr_].valueSize = strlen( error );
  stack_[stackPtr_].tagId = (*outToken).tagId;
  *outToken = stack_[stackPtr_];

  return false;

return_ok:
  stack_[stackPtr_].tagId = (*outToken).tagId;
  *outToken = stack_[stackPtr_];
  (*outToken).item = (*outToken).value;
  (*outToken).itemTotalSize = (*outToken).valueSize;

  return true;
}


size_t XMLScanner::depth() const
{
  return stackPtr_;
}


XMLScanner::Token const * XMLScanner::tokenAtDepth( size_t depth )
{
  assert( depth <= stackPtr_ );
  return &stack_[stackPtr_-depth];
}
