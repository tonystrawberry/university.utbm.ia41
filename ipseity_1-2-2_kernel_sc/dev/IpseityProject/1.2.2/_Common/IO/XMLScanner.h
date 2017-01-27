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


#ifndef XMLScanner_h
#define XMLScanner_h


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "StringFunctions.h"
#include "VFiles.h"


#include <iostream>

using namespace std;


class XMLScanner : public VFile
{
    public:

    struct Token {
      enum Kind {
        Error,
        Whitespace,
        CDATA,
        Tag,
        Attribute,
        Close,
        EndOfFile
      };
      Kind kind;

      char const * key;     //  NOT zero terminated!
                            //  tag, attribute, close, error (indicator)
      size_t keySize;

      char const * value;   //  NOT zero terminated!
                            //  attribute, whitespace, cdata, error (error msg) (also points at close tag on Close)
      size_t valueSize;

      char const * item;
      size_t itemSize, itemTotalSize;

      bool inside;

      uint32 tagId;


      inline bool keyIs( char const * str ) const {
        size_t l = strlen( str );
        return key && keySize == l && !strncmp( str, key, l );
      }

      inline bool valueIs( char const * str ) const {
        size_t l = strlen( str );
        return value && valueSize == l && !strncmp( str, value, l );
      }


      inline bool booleanValue() const
      {
          if( !strncmp( value, "yes", valueSize ) )
          {
              return true;
          }
          else
          {
              return false;
          }
      }


      inline int intValue() const
      {
          int v;

          strncpy( Text, value, valueSize );
          Text[valueSize] = '\0';

          v = atoi( Text );

          return v;
      }

      inline int intValue( int nbr ) const
      {
          int v;
          char* ptr;
          int nr = 0;

          strncpy( Text, value, valueSize );
          Text[valueSize] = '\0';

          ptr = Text;
          while( nr < nbr )
          {
              ptr = strstr( ptr, "," )+1;
              nr++;
          }
          v = atoi( ptr );

          return v;
      }

      inline float floatValue() const
      {
          float v;

          strncpy( Text, value, valueSize );
          Text[valueSize] = '\0';

          v = (float) atof( Text );

          return v;
      }

      inline float floatValue( int nbr ) const
      {
          float v;
          char* ptr;
          int nr = 0;

          strncpy( Text, value, valueSize );
          Text[valueSize] = '\0';

          ptr = Text;
          while( nr < nbr )
          {
              ptr = strstr( ptr, "," )+1;
              nr++;
          }
          v = (float) atof( ptr );

          return v;
      }

      inline char* stringValue()
      {
        strncpy( Text, value, valueSize );
        Text[valueSize] = '\0';

        return Text;
      }

      inline int nbrKeyParameters()
      {
          int nbr_parameters;

          nbr_parameters = 0;
          for( unsigned int i = 0; i < valueSize; ++i )
          {
              if (value[i] == ',')
              {
                  ++nbr_parameters;
              }
          }

          if (valueSize)
          {
              ++nbr_parameters;
          }

          return nbr_parameters;
      }

      inline char const* nextItem( const char* sep )
      {
          while( itemTotalSize && (*item == '\"' || *item == 0x0A || *item == 0x0D || (*item == ' ' && !inside)) )
          {
              if (*item == '\"')
              {
                  inside = true;
              }

              ++item;
              --itemTotalSize;
          }

          if (itemTotalSize == 0)
          {
              itemSize = 0;

              value = NULL;
              valueSize = 0;

              return NULL;
          }

          char const * ptr = item;
          char const * occ, * occ1, * occ2;

          occ1 = strstr( item, "\"" );
          occ2 = strstr( item, sep );

          if (occ2 == NULL || occ1 < occ2)
          {
              inside = false;
              occ = occ1;
          }
          else
          {
              occ = occ2;
          }

          if (occ)
          {
              itemSize = occ - item;

              if (itemSize > 0)
              {
                  itemTotalSize -= itemSize+1;
              }
          }

          item += itemSize+1;

          value = ptr;
          valueSize = itemSize;

          return ptr;
      }

      bool queryTagId( const char** strtab )
      {
          return StringFunctions::queryId( strtab, key, keySize, tagId );
      }
    };

    //  TODO: perhaps there should be a way to put the scanner in lenient mode?
    XMLScanner( const char* file, const char* base_path );
    ~XMLScanner();

    bool isLoaded() const;

    // Tokens returned by nextToken() point into the data
    // stream that you supplied. nextToken() returns false
    // on error or end of file.
    bool nextToken( XMLScanner::Token * outToken );

    // tokenAtDepth(0) is the last returned token, and may
    // be of any kind. tokenAtDepth(1..N) are always Tag
    // tokens. Depth must be between [0,depth()].
    size_t depth() const;
    XMLScanner::Token const * tokenAtDepth( size_t depth );

    protected:
        void init();

  private:
    enum {
      StackDepth = 128
    };
    size_t stackPtr_;
    Token stack_[StackDepth];
    char * start_;
    char * buf_;
    char * end_;

    bool m_Loaded;

    static char Text[1024];
};


#endif
