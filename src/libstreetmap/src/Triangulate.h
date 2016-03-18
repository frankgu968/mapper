// DERIVED FROM ONLINE CODE SNIPPET SUBMITTED BY JOHN W. RATCLIFF, AVAILABLE HERE:
// http://www.flipcode.com/archives/Efficient_Polygon_Triangulation.shtml

#ifndef TRIANGULATE_H

#define TRIANGULATE_H

/*****************************************************************/
/** Static class to triangulate any contour/polygon efficiently **/
/** You should replace Vector2d with whatever your own Vector   **/
/** class might be.  Does not support polygons with holes.      **/
/** Uses STL vectors to represent a dynamic array of vertices.  **/
/** This code snippet was submitted to FlipCode.com by          **/
/** John W. Ratcliff (jratcliff@verant.com) on July 22, 2000    **/
/** I did not write the original code/algorithm for this        **/
/** this triangulator, in fact, I can't even remember where I   **/
/** found it in the first place.  However, I did rework it into **/
/** the following black-box static class so you can make easy   **/
/** use of it in your own code.  Simply replace Vector2d with   **/
/** whatever your own Vector implementation might be.           **/
/*****************************************************************/


#include <vector>  // Include STL vector class.

class TriangleXY {
public:
	TriangleXY(const std::pair<double, double> &_A, const std::pair<double, double> &_B, const std::pair<double, double> &_C) {
		A = _A;
		B = _B;
		C = _C;
	}
	std::pair<double, double> A, B, C;
};


class Triangulate
{
public:

  // triangulate a contour/polygon, places results in STL vector
  // as series of triangles.
  static bool Process(const std::vector<std::pair<double, double>> &contour,
		  	  	  	  	  	  std::vector<TriangleXY> &result);

  // compute area of a contour/polygon
  static float Area(const std::vector<std::pair<double, double>> &contour);

  // decide if point Px/Py is inside triangle defined by
  // (Ax,Ay) (Bx,By) (Cx,Cy)
  static bool InsideTriangle(float Ax, float Ay,
                      float Bx, float By,
                      float Cx, float Cy,
                      float Px, float Py);


private:
  static bool Snip(const std::vector<std::pair<double, double>> &contour,int u,int v,int w,int n,int *V);

};


#endif
