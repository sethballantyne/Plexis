// Copyright(c) 2018 Seth Ballantyne <seth.ballantyne@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files(the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and / or sell copies of the Software, and to permit persons
// to whom the Software is furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
#pragma once

/// <summary>
/// Class for expressing a 2-dimensional vector.
/// </summary>
public value class Vector2 : System::IEquatable<Vector2>
{
private:
	// the vectors x component
	float x = 0.0f;

	// the vectors y component
	float y = 0.0f;

public:
	/// <summary>
	/// Initialises a new instance of the <see cref="Vector2"/> class.
	/// </summary>
	/// <param name="x">initial value of the vectors X component.</param>
	/// <param name="y">initial value of the vectors Y component.</param>
	Vector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	/// <summary>
	/// Initialises a new instance of the <see cref="Vector2" /> class.
	/// </summary>
	/// <param name="value">the value to be assigned to the X and Y components.</param>
	Vector2(float value)
	{
		this->x = this->y = value;
	}

	/// <summary>
	/// Calculates the dot product of this vector and <i>other</i>
	/// </summary>
	/// <param name="other">the second source vector.</param>
	/// <returns>the dot product of the two vectors.</returns>
	float Dot(Vector2 other)
	{
		return (this->x * other.X) + (this->Y * other.Y);
	}

    /// <summary>
    /// Returns a value indicating whether the current Vector2 instance is equal to the specified object.
    /// </summary>
    /// <param name="obj">The object to compare against this instance of <see cref="Vector2"/>.</param>
    /// <returns><c>true</c> if the objects are the same, otherwise <c>false</c>.</returns>
    bool Equals(Object ^obj) override
    {
        if(nullptr == obj || GetType() != obj->GetType())
        {
            return false;
        }

        return Equals(safe_cast<Vector2>(obj));
    }

    /// <summary>
    /// Returns a value indicating whether the current <see cref="Vector2"/> instance is equal to the specified object.
    /// </summary>
    /// <param name="vector">The <see cref="Vector2"/> instance to compare against.</param>
    /// <returns><c>true</c> if the instances match, otherwise <c>false</c>.</returns>
    bool virtual Equals(Vector2 vector)
    {
        return (this->x == vector.x && this->y == vector.y);
    }

    /// <summary>
    /// Gets the hash code for this instance.
    /// </summary>
    /// <returns>the instances hashcode as a 32-bit signed int.</returns>
    int GetHashCode() override
    {
        return x.GetHashCode() + y.GetHashCode();
    }

    /// <summary>
    /// Calculates and returns the length of the vector.
    /// </summary>
    /// <returns>The length of the vector.</returns>
    float Length()
    {
        return System::Math::Sqrt((this->x * this->x) + (this->y * this->y));
    }

    /// <summary>
    /// Converts the vector into a unit vector.
    /// </summary>
    /// <returns>The current vector as a unit vector.</returns>
    Vector2 Normalise()
    {
        float length = this->Length();
        if(0 == length)
        {
            return *this;
        }

        float temp = 1/ length;
        return Vector2(this->x * temp, this->y * temp);
    }

    /// <summary>
    /// Gets or sets the vectors X component.
    /// </summary>
    property float X
    {
        float get()
        {
            return this->x;
        }

        void set(float value)
        {
            this->x = value;
        }
    }
    
    /// <summary>
    /// Gets or sets the vectors Y component.
    /// </summary>
    property float Y
    {
        float get()
        {
            return this->y;
        }

        void set(float value)
        {
            this->y = value;
        }
    }

    /// <summary>
    /// Gets a <see cref="Vector2" /> Vector2 instance with both components set to 0.
    /// </summary>
    static property Vector2 Zero
    {
        Vector2 get()
        {
            return Vector2(0, 0);
        }
    }

    /// <summary>
    /// Reverses the direction of the specified vector.
    /// </summary>
    /// <returns>A vector facing in the opposite direction.</returns>
    Vector2 operator -()
    {
        return Vector2(-this->x, -this->y);
    }

    /// <summary>
    /// Adds two vectors.
    /// </summary>
    /// <param name="right">the vector to add to the current instance.</param>
    /// <returns>the sum of both vectors.</returns>
    Vector2 operator + (Vector2 right)
    {
        return Vector2(this->x + right.x, this->y + right.y);
    }

    /// <summary>
    /// Subtracts a vector from another.
    /// </summary>
    /// <param name="right">the vector to substract from the current instance.</param>
    /// <returns>the sum of both vectors.</returns>
    Vector2 operator - (Vector2 right)
    {
        return Vector2(this->x - right.x, this->y - right.y);
    }

    /// <summary>
    /// scales the vector by the specified value.
    /// </summary>
    /// <param name="scale">the value to scale the vector by.</param>
    /// <returns>The scaled vector.</returns>
    Vector2 operator * (float scale)
    {
        return Vector2(this->x * scale, this->y * scale);
    }

    /// <summary>
    /// scales the vector by the specified value.
    /// </summary>
    /// <param name="scale">the value to scale the vector by.</param>
    /// <returns>the scaled vector.</returns>
    Vector2 operator / (float scale)
    {
        return Vector2(this->x / scale, this->y / scale);
    }

    /// <summary>
    /// compares two vectors.
    /// </summary>
    /// <param name="right">the vector to compare against.</param>
    /// <returns><c>true</c> if the vectors are equal, otherwise <c>false</c>.</returns>
    bool operator == (Vector2 right)
    {
        return Equals(right);
    }

    /// <summary>
    /// compares two vectors to see if they're not equal.
    /// </summary>
    /// <param name="right">the vector to compare against.</param>
    /// <returns><c>true</c> if the vectors don't match, otherwise <c>false</c>.</returns>
    bool operator != (Vector2 right)
    {
        return !Equals(right);
    }
};
