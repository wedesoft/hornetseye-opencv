/* HornetsEye - Computer Vision with Ruby
   Copyright (C) 2006, 2007, 2008, 2009 Jan Wedekind

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>. */
#include <algorithm>
#include <opencv/cxcore.h>
#include "cvmat.hh"
#include "error.hh"
#include "rubytools.hh"

using namespace std;

VALUE CvMatExt::cRubyClass = Qnil;

VALUE CvMatExt::registerRubyClass( VALUE module )
{
  cRubyClass = rb_define_class_under( module, "CvMat", rb_cObject );
  rb_define_method( cRubyClass, "to_multiarray",
                    RUBY_METHOD_FUNC( wrapToMultiArray ), 0 );
  return cRubyClass;
}

void CvMatExt::deleteRubyObject( void *ptr )
{
  CvMat *mat = (CvMat *)ptr;
  cvReleaseMat( &mat );
}

VALUE CvMatExt::wrapToMultiArray( VALUE rbSelf )
{
  VALUE rbRetVal = Qnil;
  try {
    VALUE mHornetseye = rb_define_module( "Hornetseye" );
    CvMat *cvMat;
    dataGetStruct( rbSelf, cRubyClass, CvMat, cvMat );
    int width = cvMat->width;
    int height = cvMat->height;
    int channels = CV_MAT_CN( cvMat->type );
    VALUE rbTypecode;
    switch ( CV_MAT_DEPTH( cvMat->type ) ) {
    case CV_8U:
      rbTypecode = rb_const_get( mHornetseye, rb_intern( "UBYTE" ) );
      break;
    case CV_8S:
      rbTypecode = rb_const_get( mHornetseye, rb_intern( "BYTE" ) );
      break;
    case CV_16U:
      rbTypecode = rb_const_get( mHornetseye, rb_intern( "USINT" ) );
      break;
    case CV_16S:
      rbTypecode = rb_const_get( mHornetseye, rb_intern( "SINT" ) );
      break;
    case CV_32S:
      rbTypecode = rb_const_get( mHornetseye, rb_intern( "INT" ) );
      break;
    case CV_32F:
      rbTypecode = rb_const_get( mHornetseye, rb_intern( "SFLOAT" ) );
      break;
    case CV_64F:
      rbTypecode = rb_const_get( mHornetseye, rb_intern( "DFLOAT" ) );
      break;
    default:
      ERRORMACRO( false, Error, ,
                  "Encountered unknown type number "
                  << CV_MAT_DEPTH( cvMat->type )
                  << " while trying to convert to MultiArray" );
    };
    VALUE rbTypeSize = rb_funcall( rbTypecode, rb_intern( "storage_size" ), 0 );
    int typesize = NUM2INT( rbTypeSize );
    int size = width * height * channels * typesize;
    VALUE cMalloc = rb_define_class_under( mHornetseye, "Malloc", rb_cObject );
    VALUE rbDest = rb_funcall( cMalloc, rb_intern( "new" ), 1, INT2NUM( size ) );
    char *dest; Data_Get_Struct( rbDest, char, dest );
    copy( cvMat->data.ptr, cvMat->data.ptr + size, dest );
    VALUE rbMultiArray = rb_funcall( mHornetseye, rb_intern( "MultiArray" ), 4,
                                     rbTypecode, INT2NUM( channels ),
                                     INT2NUM( width ), INT2NUM( height ) );
    VALUE rbImport = rb_funcall( rbMultiArray, rb_intern( "new" ), 1, rbDest );
    rbRetVal = rbImport;
  } catch( std::exception &e ) {
    rb_raise( rb_eRuntimeError, "%s", e.what() );
  };
  return rbRetVal;
}

