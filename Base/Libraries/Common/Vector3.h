/******************************************************************************/
/*!
\file	Vector3.h
\author Wen Sheng Tang
\par	email: tang_wen_sheng\@nyp.edu.sg
\brief
Struct to define a 3D vector
*/
/******************************************************************************/

#ifndef VECTOR3_H
#define VECTOR3_H

#include "MyMath.h"
#include <iostream>

#pragma warning( disable: 4290 ) //for throw(DivideByZero)

/******************************************************************************/
/*!
		Class Vector3:
\brief	Defines a 3D vector and its methods
*/
/******************************************************************************/
struct Vector3
{
	float x, y, z;
	bool IsEqual(float a, float b) const;

	Vector3(float a = 0.0, float b = 0.0, float c = 0.0);
	Vector3(const Vector3 &rhs);
	~Vector3();
	
	void Set( float a = 0, float b = 0, float c = 0 ); //Set all data
	void SetXY(float a, float b);
	void SetZero( void ); //Set all data to zero
	bool IsZero( void ) const; //Check if data is zero

	Vector3 operator+( const Vector3& rhs ) const; //Vector addition
	Vector3& operator+=( const Vector3& rhs ); 
	
	Vector3 operator-( const Vector3& rhs ) const; //Vector subtraction
	Vector3& operator-=( const Vector3& rhs );
	
	Vector3 operator-( void ) const; //Unary negation
	
	Vector3 operator*( float scalar ) const; //Scalar multiplication
	Vector3& operator*=( float scalar );

	bool operator==( const Vector3& rhs ) const; //Equality check
	bool operator!= ( const Vector3& rhs ) const; //Inequality check

	//never check Z
	bool operator>=(const Vector3& rhs) const; //Vector addition
	bool operator<=(const Vector3& rhs) const;

	Vector3& operator=(const Vector3& rhs); //Assignment operator

	float Length( void ) const; //Get magnitude
	float LengthSquared (void ) const; //Get square of magnitude
	float GetDistance(Vector3& check);
	float GetDistanceSquared(Vector3& check);
	Vector3 GetDirection(Vector3& check);
	bool Same(Vector3& check);
	
	float Dot( const Vector3& rhs ) const; //Dot product
	Vector3 Cross( const Vector3& rhs ) const; //Cross product
	
	//Return a copy of this vector, normalized
	//Throw a divide by zero exception if normalizing a zero vector
	Vector3 Normalized( void ) const throw( DivideByZero );
	float Normalize_andLength() throw(DivideByZero);
	
	//Normalize this vector and return a reference to it
	//Throw a divide by zero exception if normalizing a zero vector
	Vector3& Normalize( void ) throw( DivideByZero );
	
	friend std::ostream& operator<<( std::ostream& os, Vector3& rhs); //print to ostream

	friend Vector3 operator*( float scalar, const Vector3& rhs ); //what is this for?

	static float getAngleFromDir(float dirX, float dirY) throw( DivideByZero );	//get angle between both vectors using this as pivot

	Vector3& multiplyWithVector(Vector3& rhs);
	Vector3& divideWithVector(Vector3& rhs);

	void Clip_EulerAngles();	//if > 360, -= 360
};



#endif //VECTOR3_H
