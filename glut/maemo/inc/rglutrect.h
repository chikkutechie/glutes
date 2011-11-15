/*
 * Copyright (c) 2011, Ranjith TV
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer
 *     in the documentation and/or other materials provided with the distribution.
 *   * Neither the name the Ranjith nor the names of its contributors may be used
       to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _RGLUTRECT_H_
#define _RGLUTRECT_H_

/**
 * @class RGlutRect
 * @brief Rectangle class implementation.
 */
template <typename T>
class RGlutRect
{
public:
    RGlutRect()
    {}

    RGlutRect(T x1, T y1, T x2, T y2)
        : mX1(x1),
          mY1(y1),
          mX2(x2),
          mY2(y2)
    {}

    bool isEmpty() const {
        return ((width() == 0) | (height() == 0));
    }

    /*!
     * Retrieves x1 value
     */
    T x1() const {
        return mX1;
    }
    /*!
     * Retrieves y1 value
     */
    T y1() const {
        return mY1;
    }
    /*!
     * Retrieves x2 value
     */
    T x2() const {
        return mX2;
    }
    /*!
     * Retrieves y2 value
     */
    T y2() const {
        return mY2;
    }

    /*!
     * Sets X1 value
     */
    void setX1(T v) {
        mX1 = v;
    }
    /*!
     * Sets Y1 value
     */
    void setY1(T v) {
        mY1 = v;
    }
    /*!
     * Sets X2 value
     */
    void setX2(T v) {
        mX2 = v;
    }
    /*!
     * Sets Y2 value
     */
    void setY2(T v) {
        mY2 = v;
    }

    /*!
     * Sets height of the rectangle.
     */
    void setHeight(T v) {
        setY2(y1() + v);
    }
    
    /*!
     * Retrieves height of the rectangle.
     */
    T height() const {
        return mY2 - mY1;
    }

    /*!
     * Sets width of the rectangle.
     */
    void setWidth(T v) {
        setX2(x1() + v);
    }
    
    /*!
     * Retrieves height of the rectangle.
     */
    T width() const {
        return mX2 - mX1;
    }

private:
    T mX1;
    T mY1;
    T mX2;
    T mY2;
};

typedef RGlutRect<int> RGlutRectI;
typedef RGlutRect<unsigned int> RGlutRectUI;
typedef RGlutRect<float> RGlutRectF;
typedef RGlutRect<double> RGlutRectD;

#endif

