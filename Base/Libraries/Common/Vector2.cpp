#include "Vector2.h"
#include <iostream>
using namespace std;

Vector2::Vector2( float a, float b )
{
	x = a;
	y = b;
}

Vector2::Vector2( const Vector2 &rhs )
{
	x = rhs.x;
	y = rhs.y;
}

void Vector2::Set( float a, float b )
{
	x = a;
	y = b;
}

void Vector2::SetZero()
{
	x = 0.f;
	y = 0.f;
}

bool Vector2::IsEqual(float a, float b) const
{
	return a - b <= Math::EPSILON && b - a <= Math::EPSILON;
}

Vector2 Vector2::operator+( const Vector2& rhs ) const
{
	return Vector2(this->x + rhs.x, this->y + rhs.y);
}

Vector2 Vector2::operator+=( const Vector2& rhs ) 
{
	return Vector2(this->x += rhs.x, this->y += rhs.y);
}

Vector2 Vector2::operator-( const Vector2& rhs ) const
{
	return Vector2(x - rhs.x, y - rhs.y);
}

Vector2 Vector2::operator-=( const Vector2& rhs ) 
{
	return Vector2(this->x -= rhs.x, this->y -= rhs.y);
}

Vector2 Vector2::operator-( void ) const
{
	return Vector2(-this->x, -this->y);
}

Vector2 Vector2::operator*( float scalar ) const
{
	return Vector2(this->x * scalar, this->y * scalar);
}

Vector2& Vector2::operator*=(float scalar) 
{
	x *= scalar;
	y *= scalar;
	return *this;
}

float Vector2::Length( void ) const
{
	return sqrt(x * x + y * y);
}

float Vector2::LengthSquared(void) const
{
	return x * x + y * y;
}

float Vector2::Dot( const Vector2& rhs ) const
{
	return x * rhs.x + y * rhs.y;
}

bool Vector2::isZero()
{
	return IsEqual(x, 0.f) && IsEqual(y, 0.f);
}

std::ostream& operator<< (std::ostream& os, Vector2& rhs)
{
	os << "[ " << rhs.x << ", " << rhs.y << " ]";
	return os;
}

/******************************************************************************/
/*!
\brief	Assignment operation (assign a vector to this vector)

\param	rhs
	Vector to assign
\return 
	A reference to this vector
*/
/******************************************************************************/
Vector2& Vector2::operator=(const Vector2& rhs)
{
	x = rhs.x;
	y = rhs.y;
	return *this;
}

bool Vector2::operator<=(const Vector2& rhs)
{
	return x <= rhs.x && y <= rhs.y;
}

bool Vector2::operator>(const Vector2& rhs)
{
	return x > rhs.x && y > rhs.y;
}

Vector2 Vector2::Normalized( void )
{
	float d = Length();
	if(d <= Math::EPSILON && -d <= Math::EPSILON)
	  throw DivideByZero();
	return Vector2(x / d, y / d);
}

void Vector2::Clip_EulerAngles()
{
	if (x < 0.f)x += 360.f;
	if (y < 0.f)y += 360.f;
	if (x > 360.f)x -= 360.f;
	if (y > 360.f)y -= 360.f;
}

bool Vector2::Same(Vector2& check)
{
	return abs(x - check.x) <= Math::EPSILON &&
		abs(y - check.y) <= Math::EPSILON;
}

float Vector2::getAngleFromDir(const Vector2& dir)
{
	return atan2(dir.y, dir.x) * (180 / Math::PI);
}