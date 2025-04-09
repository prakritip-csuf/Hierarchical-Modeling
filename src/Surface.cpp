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










    return surface;

}


