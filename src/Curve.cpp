#include "Curve.h"
#include <glm/gtx/string_cast.hpp>
#include <iostream>

Curve::Curve(const std::string& name, unsigned steps, const std::string& curveType)
    : name(name), steps(steps), curveType(curveType) {}

// Get curve name
const std::string& Curve::getName() const {
    return name;
}

// Set control points
void Curve::setControlPoints(const std::vector<glm::vec3>& cps) {
    controlPoints = cps;
}

// Get control points
const std::vector<glm::vec3>& Curve::getControlPoints() const {
    return controlPoints;
}

// Getter for curvePoints
const CurvePoints& Curve::getCurvePoints() const {
    return curvePoints;
}

// Setter for curvePoints
void Curve::setCurvePoints(const CurvePoints& points) {
    curvePoints = points;
}


// Set steps
void Curve::setSteps(unsigned steps) {
    this->steps = steps;
}

// Get steps
unsigned Curve::getSteps() const {
    return steps;
}

// Set curve type
void Curve::setCurveType(const std::string& type) {
    curveType = type;
}

// Get curve type
const std::string& Curve::getCurveType() const {
    return curveType;
}


// Function to evaluate Bezier curve points
CurvePoints Curve::evalBezier(const std::vector<glm::vec3>& P, unsigned steps) {


    CurvePoints curve;

    // Check if number of points are required for Bezier
    if (P.size() < 4 || P.size() % 3 != 1) {
        std::cerr << "evalBezier must be called with 3n+1 control points." << std::endl;
        exit(0);
    }

    // 2.3.1 - Create the functionality to process Bezier control points

    // TODO:
    // You should implement this function so that it returns a Curve
    // (e.g., a vector< CurvePoint >).  The variable "steps" tells you
    // the number of points to generate on each piece of the spline.
    // At least, that's how the sample solution is implemented and how
    // the SWP files are written.  But you are free to interpret this
    // variable however you want, so long as you can control the
    // "resolution" of the discretized spline curve with it.

    // Make sure that this function computes all the appropriate
    // Vector3fs for each CurvePoint: V,T,N,B.
    // [NBT] should be unit and orthogonal.

    // Also note that you may assume that all Bezier curves that you
    // receive have G1 continuity.  Otherwise, the TNB will not be
    // be defined at points where this does not hold.


    std::cerr << "\t>>> evalBezier has been called with the following input:" << std::endl;

    std::cerr << "\t>>> Control points (type vector< glm::vec3 >): "<< std::endl;
    for( unsigned i = 0; i < P.size(); ++i )
    {
        std::cerr << "\t>>> (" << P[i].x << ", " << P[i].y << ", " << P[i].z << ")" << std::endl;
    }

    std::cerr << "\t>>> Steps (type steps): " << steps << std::endl;
    std::cerr << "\t>>> Returning empty curve." << std::endl;

    glm::vec3 prevB;

    glm::mat4 mBez = glm::mat4(

        glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
        glm::vec4(-3.0f, 3.0f, 0.0f, 0.0f),
        glm::vec4(3.0f, -6.0f, 3.0f, 0.0f),
        glm::vec4(-1.0f, 3.0f, -3.0f, 1.0f)
    );

    //derivative of BezierCurve - to calculate tangents
    glm::mat4 dBez = glm::mat4(

        glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
        glm::vec4(-3.0f, 3.0f, 0.0f, 0.0f),
        glm::vec4(6.0f, -12.0f, 6.0f, 0.0f),
        glm::vec4(-3.0f, 9.0f, -9.0f, 3.0f)
    );
    
    for (size_t i = 0; i < P.size() -3; i+=3) {

        glm::mat4 G = glm::mat4(
            glm::vec4( P[i][0], P[i][1], P[i][2], 0.0f),
            glm::vec4( P[i+1][0], P[i+1][1], P[i+1][2], 0.0f),
            glm::vec4( P[i+2][0], P[i+2][1], P[i+2][2], 0.0f),
            glm::vec4( P[i+3][0], P[i+3][1], P[i+3][2], 0.0f)
        );

        glm::vec3 B = (i==0) ? glm::vec3(0.0f, 0.0f, 1.0f) : prevB;
        
        for (unsigned j = 0; j <= steps; ++j) {

            float t =  static_cast<float>(j) / static_cast<float> (steps);

            glm::vec4 M(1, t, t*t, t*t*t);

            CurvePoint point;

            point.V = glm::vec3 ( (G * (mBez * M))[0],
                                  (G * (mBez * M))[1],
                                  (G * (mBez * M))[2]);

            if (j == 0)
            {
                point.T = glm::normalize(glm::vec3(G[1])) - glm::vec3(G[0]);
            } 
            
            else {
            point.T = glm::normalize(glm::vec3 ( (G * (dBez * M))[0],
                                                (G * (dBez * M))[1],
                                                (G * (dBez * M))[2]));
            }

            point.N = glm::normalize( glm::cross(B, point.T) );

            point.B = glm::normalize( glm::cross(point.T, point.N) );

            prevB = point.B;

            curve.push_back(point);
            }

        }

    // Right now this will just return this empty curve.

    return curve;
}



CurvePoints Curve::evalBspline(const std::vector<glm::vec3>& P, unsigned steps) {

    CurvePoints curve;

    // Check if number of points are required for Bspline
    if (P.size() < 4) {
        std::cerr << "evalBspline must be called with 4 or more control points." << std::endl;
        exit(0);
    }

    // 2.3.2 - Create the functionality to process BSpline control points

    // TODO:
    // It is suggested that you implement this function by changing
    // basis from B-spline to Bezier.  That way, you can just call
    // your evalBezier function.
    
    std::cerr << "\t>>> evalBSpline has been called with the following input:" << std::endl;

    std::cerr << "\t>>> Control points (type vector< glm::vec3 >): "<< std::endl;
    for( unsigned i = 0; i < P.size(); ++i )
    {
        std::cerr << "\t>>> (" << P[i].x << ", " << P[i].y << ", " << P[i].z << ")" << std::endl;
    }

    std::cerr << "\t>>> Steps (type steps): " << steps << std::endl;
    std::cerr << "\t>>> Returning empty curve." << std::endl;

    glm::mat4 mBsp = glm::mat4(
        glm::vec4(1.0f/6.0f, 4.0f/6.0f, 1.0f/6.0f, 0.0f),
        glm::vec4(-3.0f/6.0f, 0.0f, 3.0f/6.0f, 0.0f),
        glm::vec4(3.0f/6.0f, -6.0f/6.0f, 3.0f/6.0f, 0.0f),
        glm::vec4(-1.0f/6.0f, 3.0f/6.0f, -3.0f/6.0f, 1.0f/6.0f)
    );

    glm::mat4 mBez = glm::mat4(
        glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
        glm::vec4(-3.0f, 3.0f, 0.0f, 0.0f),
        glm::vec4(3.0f, -6.0f, 3.0f, 0.0f),
        glm::vec4(-1.0f, 3.0f, -3.0f, 1.0f)
    );

    glm::mat4 BezInv = glm::inverse(mBez);

    for (size_t i = 0; i < P.size() -3; ++i) {

    glm::mat4 G = glm::mat4(
        glm::vec4( P[i][0], P[i][1], P[i][2], 0.0f),
        glm::vec4( P[i+1][0], P[i+1][1], P[i+1][2], 0.0f),
        glm::vec4( P[i+2][0], P[i+2][1], P[i+2][2], 0.0f),
        glm::vec4( P[i+3][0], P[i+3][1], P[i+3][2], 0.0f)
    );

    glm::mat4 bezG = G * mBsp * BezInv;

    std::vector<glm::vec3> BspCP;

    BspCP.push_back(glm::vec3(bezG[0]));
    BspCP.push_back(glm::vec3(bezG[1]));
    BspCP.push_back(glm::vec3(bezG[2]));
    BspCP.push_back(glm::vec3(bezG[3]));


    CurvePoints tempCurve = evalBezier(BspCP, steps);

    curve.insert(curve.end(), tempCurve.begin(), tempCurve.end());
    }
    // Return an empty curve right now.

    return curve;
}



// Function to evaluate circle points
CurvePoints Curve::evalCircle(float radius, unsigned steps) {

    CurvePoints circlePoints;
    
    // 2.3.3 - Create the functionality to process Circles based on radius and steps

    // Based on how you evaluated Bezier and B-spline curves, you will calcuate and return a curve
    // that will create a circle. The curve will contain the same CurvePoint structure: V,T,N,B.

    std::cerr << "\t>>> evalCircle has been called with the following input:" << std::endl;

    std::cerr << "\t>>> Radius: "<< radius << std::endl;
    std::cerr << "\t>>> Steps (type steps): " << steps << std::endl;
    std::cerr << "\t>>> Returning empty curve." << std::endl;

    float angleStep = 2.0f * glm::pi<float>() / steps;

    for ( unsigned i = 0; i <= steps; ++i ) {

        float angle = angleStep * i;

        // Calculate the point on the circle

        glm::vec3 V(radius * cos(angle), radius * sin(angle), 0);
        glm::vec3 T(-sin(angle), cos(angle), 0);
        glm::vec3 N(-cos(angle), -sin(angle), 0);
        glm::vec3 B(glm::vec3(0.0f, 0.0f, 1.0f));

        circlePoints.push_back({V, T, N, B});

    }
    
    return circlePoints;
}
