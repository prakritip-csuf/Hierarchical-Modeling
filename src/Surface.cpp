#include "Surface.h"

namespace {
// Check if the profile curve is flat on the xy-plane
bool checkFlat(const Curve &profile) {
    for (const auto& p : profile.getControlPoints()) {
        if (p.z != 0.0f) return false;
    }
    return true;
}
}

Surface Surface::makeSurfRev(const Curve& profile, unsigned steps) {

    Surface surface;

    if (!checkFlat(profile)) {
        std::cerr << "surfRev profile curve must be flat on the xy-plane." << std::endl;
        exit(0);
    }

    // 2.4.1 - Create the functionality to process surface revolutions

    // Here you should build the surface.  See Surface.h for details.

    std::cerr << "\t>>> makeSurfRev called (but not implemented).\n\t>>> Returning empty surface." << std::endl;

    const CurvePoints& curvePoints = profile.getCurvePoints();

    for (unsigned i = 0; i < curvePoints.size(); ++i) {

        for (unsigned j = 0; j < steps; ++j){

            float angle = 2.0f * glm::pi<float>() * static_cast<float>(j) / static_cast<float>(steps);

            glm::mat4 rotateM = glm::rotate( glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));

            glm::vec4 profileVV( curvePoints[i].V, 1.0f);

            glm::vec3 surfaceVV = glm::vec3( rotateM * profileVV);

            glm::vec3 surfaceVN = glm::normalize(glm::mat3(rotateM) * curvePoints[i].N);

            surface.VV.push_back( surfaceVV );
            surface.VN.push_back( -surfaceVN );

        }

    }

    for (unsigned i = 0; i < curvePoints.size() - 1; ++i) {

        for (unsigned j = 0; j < steps; ++j){

            unsigned next = (j + 1) % steps;

            // Triangle 1
            surface.VF.push_back( Tup3u( i * steps + j , (i+1) * steps + j , i * steps + next ) );


            // Triangle 2
            surface.VF.push_back( Tup3u( (i+1) * steps + j , (i+1) * steps + next ,  i * steps + next) );

        }
    }

    return surface;
}


Surface Surface::makeGenCyl(const Curve& profile, const Curve& sweep) {

    Surface surface;

    if (!checkFlat(profile)) {
        std::cerr << "genCyl profile curve must be flat on the xy-plane." << std::endl;
        exit(0);
    }

    // 2.4.2 - Create the functionality to process surface sweeps

    // Here you should build the surface.  See surf.h for details.

    std::cerr << "\t>>> makeGenCyl called (but not implemented).\n\t>>> Returning empty surface." << std::endl;

     // Process surface sweeps
     const CurvePoints& profilePoints = profile.getCurvePoints();
     const CurvePoints& sweepPoints = sweep.getCurvePoints();
 
     // Ensure that both curves have valid data.
     if (profilePoints.empty() || sweepPoints.empty()) {
         std::cerr << "Error: Profile or sweep curve has no points." << std::endl;
         return surface; // Return an empty surface
     }
 
     // Build the surface vertices and normals
     for (unsigned i = 0; i < sweepPoints.size(); ++i) {
         glm::mat4 transform = glm::translate(glm::mat4(1.0f), sweepPoints[i].V);
 
         glm::mat3 rotation(sweepPoints[i].N, sweepPoints[i].B, sweepPoints[i].T);
         transform = transform * glm::mat4(rotation);
 
         for (unsigned j = 0; j < profilePoints.size(); ++j) {
             // Transform profile vertex
             glm::vec4 profileVertex(profilePoints[j].V, 1.0f); 
             glm::vec3 surfaceVertex = transform * profileVertex;
 
            
             glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(rotation))); 
             glm::vec3 surfaceNormal = -glm::normalize(normalMatrix * profilePoints[j].N);
             
             surface.VV.push_back(surfaceVertex);
             surface.VN.push_back(surfaceNormal);
         }
     }
 
     // Build the faces
     for (unsigned i = 0; i < sweepPoints.size() - 1; ++i) {
         for (unsigned j = 0; j < profilePoints.size() - 1; ++j) {
             unsigned nextSweep = i + 1;
             unsigned nextProfile = j + 1;
 
             // Triangle 1
             surface.VF.push_back(Tup3u(i * profilePoints.size() + j,
                                         i * profilePoints.size() + nextProfile,
                                         nextSweep * profilePoints.size() + j));
 
             // Triangle 2
             surface.VF.push_back(Tup3u(nextSweep * profilePoints.size() + j,
                                         i * profilePoints.size() + nextProfile,
                                         nextSweep * profilePoints.size() + nextProfile));
         }
     }


    return surface;

}


