/*
-----------------------------------------------------------------------------
This source file is part of OGRE-Next
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org

Copyright (c) 2000-2014 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#include "Vao/OgreGL3PlusUavBufferPacked.h"

#include "Vao/OgreGL3PlusBufferInterface.h"
#include "Vao/OgreGL3PlusReadOnlyBufferPacked.h"
#include "Vao/OgreGL3PlusTexBufferPacked.h"

namespace Ogre
{
    GL3PlusUavBufferPacked::GL3PlusUavBufferPacked( size_t internalBufStartBytes, size_t numElements,
                                                    uint32 bytesPerElement, uint32 bindFlags,
                                                    void *initialData, bool keepAsShadow,
                                                    VaoManager *vaoManager,
                                                    GL3PlusBufferInterface *bufferInterface ) :
        UavBufferPacked( internalBufStartBytes, numElements, bytesPerElement, bindFlags, initialData,
                         keepAsShadow, vaoManager, bufferInterface )
    {
    }
    //-----------------------------------------------------------------------------------
    GL3PlusUavBufferPacked::~GL3PlusUavBufferPacked() {}
    //-----------------------------------------------------------------------------------
    TexBufferPacked *GL3PlusUavBufferPacked::getAsTexBufferImpl( PixelFormatGpu pixelFormat )
    {
        OGRE_ASSERT_HIGH( dynamic_cast<GL3PlusBufferInterface *>( mBufferInterface ) );

        GL3PlusBufferInterface *bufferInterface =
            static_cast<GL3PlusBufferInterface *>( mBufferInterface );

        TexBufferPacked *retVal = OGRE_NEW GL3PlusTexBufferPacked(
            mInternalBufferStart * mBytesPerElement, mNumElements, mBytesPerElement, 0, mBufferType,
            (void *)0, false, (VaoManager *)0, bufferInterface, pixelFormat );
        // We were overriden by the BufferPacked we just created. Restore this back!
        bufferInterface->_notifyBuffer( this );

        return retVal;
    }
    //-----------------------------------------------------------------------------------
    ReadOnlyBufferPacked *GL3PlusUavBufferPacked::getAsReadOnlyBufferImpl()
    {
        OGRE_ASSERT_HIGH( dynamic_cast<GL3PlusBufferInterface *>( mBufferInterface ) );

        GL3PlusBufferInterface *bufferInterface =
            static_cast<GL3PlusBufferInterface *>( mBufferInterface );

        ReadOnlyBufferPacked *retVal = OGRE_NEW GL3PlusReadOnlyUavBufferPacked(
            mInternalBufferStart * mBytesPerElement, mNumElements, mBytesPerElement, 0, mBufferType,
            (void *)0, false, (VaoManager *)0, bufferInterface, PFG_NULL );
        // We were overriden by the BufferPacked we just created. Restore this back!
        bufferInterface->_notifyBuffer( this );

        return retVal;
    }
    //-----------------------------------------------------------------------------------
    inline void GL3PlusUavBufferPacked::bindBuffer( uint16 slot, size_t offset, size_t sizeBytes )
    {
        OGRE_ASSERT_HIGH( dynamic_cast<GL3PlusBufferInterface *>( mBufferInterface ) );
        OGRE_ASSERT_LOW( offset <= getTotalSizeBytes() );
        OGRE_ASSERT_LOW( sizeBytes <= getTotalSizeBytes() );
        OGRE_ASSERT_LOW( ( offset + sizeBytes ) <= getTotalSizeBytes() );

        sizeBytes = !sizeBytes ? ( mNumElements * mBytesPerElement - offset ) : sizeBytes;

        GL3PlusBufferInterface *bufferInterface =
            static_cast<GL3PlusBufferInterface *>( mBufferInterface );

        OCGE( glBindBufferRange( GL_SHADER_STORAGE_BUFFER, slot, bufferInterface->getVboName(),
                                 static_cast<GLintptr>( mFinalBufferStart * mBytesPerElement + offset ),
                                 static_cast<GLsizeiptr>( sizeBytes ) ) );
    }
    //-----------------------------------------------------------------------------------
    //    void GL3PlusUavBufferPacked::bindBufferVS( uint16 slot, size_t offset, size_t sizeBytes )
    //    {
    //        bindBuffer( slot, offset, sizeBytes );
    //    }
    //    //-----------------------------------------------------------------------------------
    //    void GL3PlusUavBufferPacked::bindBufferPS( uint16 slot, size_t offset, size_t sizeBytes )
    //    {
    //        bindBuffer( slot, offset, sizeBytes );
    //    }
    //    //-----------------------------------------------------------------------------------
    //    void GL3PlusUavBufferPacked::bindBufferGS( uint16 slot, size_t offset, size_t sizeBytes )
    //    {
    //        bindBuffer( slot, offset, sizeBytes );
    //    }
    //    //-----------------------------------------------------------------------------------
    //    void GL3PlusUavBufferPacked::bindBufferHS( uint16 slot, size_t offset, size_t sizeBytes )
    //    {
    //        bindBuffer( slot, offset, sizeBytes );
    //    }
    //    //-----------------------------------------------------------------------------------
    //    void GL3PlusUavBufferPacked::bindBufferDS( uint16 slot, size_t offset, size_t sizeBytes )
    //    {
    //        bindBuffer( slot, offset, sizeBytes );
    //    }
    //-----------------------------------------------------------------------------------
    void GL3PlusUavBufferPacked::bindBufferCS( uint16 slot, size_t offset, size_t sizeBytes )
    {
        bindBuffer( slot, offset, sizeBytes );
    }
    //-----------------------------------------------------------------------------------
}  // namespace Ogre
