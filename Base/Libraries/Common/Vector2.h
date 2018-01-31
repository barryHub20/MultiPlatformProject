#ifndef VECTOR2_H
#define VECTOR2_H
#include "MyMath.h"
#include <iostream>

struct Vector2
{
	float x, y;
	Vector2( float a = 0, float b = 0 ); //default constructor
	Vector2( const Vector2 &rhs ); //copy constructor
	void Set( float a, float b ); //Set all data
	void SetZero();
	Vector2 operator+( const Vector2& rhs ) const; //Vector addition
	Vector2 operator+=( const Vector2& rhs );
	Vector2 operator-( const Vector2& rhs ) const; //Vector subtraction
	Vector2 operator-=( const Vector2& rhs );
	Vector2 operator-( void ) const; //Unary negation
	Vector2 operator*( float scalar ) const; //Scalar multiplication
	Vector2& operator*=(float scalar); //Scalar multiplication
	Vector2& operator=(const Vector2& rhs); //Assignment operator
	bool operator<=(const Vector2& rhs);
	bool operator>(const Vector2& rhs);
	float Length( void ) const; //Get magnitude
	float LengthSquared(void) const; //Get magnitude
	float Dot( const Vector2& rhs ) const; //Dot product
	bool IsEqual(float a, float b) const;
	bool isZero();	//is it zero?
	friend std::ostream& operator<<( std::ostream& os, Vector2& rhs); //print to ostream
	Vector2 Normalized( void ); //Return a copy of this vector, normalized
	void Clip_EulerAngles();
	bool Same(Vector2& check);
	static float getAngleFromDir(const Vector2& dir);
};
#endif
