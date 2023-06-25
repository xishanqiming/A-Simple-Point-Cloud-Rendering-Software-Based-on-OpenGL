// Point3D.h: interface for the Point3D class.
// 
//////////////////////////////////////////////////////////////////////
/*
* 基于OpenGL的2D/3D点云图像读取软件
* by Weilin Chang 2022-12-20
*/
#if !defined(AFX_Point3D_H__31FDB339_F187_488D_B03A_3990BAFAC026__INCLUDED_)
#define AFX_Point3D_H__31FDB339_F187_488D_B03A_3990BAFAC026__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Point3D  
{
public:
	Point3D();
	virtual ~Point3D();
	Point3D operator + (const Point3D &point3D);
	Point3D operator - (const Point3D &point3D);
	Point3D operator / (const int &sum);
	Point3D operator * (const int &sum);
	Point3D operator += (const Point3D &point3D);
	Point3D operator -= (const Point3D &point3D);
	Point3D operator /= (const int &sum);	
	Point3D operator *= (const int &sum);
public:
	float x;
	float y;
	float z;
	BYTE   w;// w = 0    
	         //   = 1    

};

#endif // !defined(AFX_Point3D_H__31FDB339_F187_488D_B03A_3990BAFAC026__INCLUDED_)
