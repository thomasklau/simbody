/* -------------------------------------------------------------------------- *
 *                         Simbody(tm)  ExampleGeodesic                       *
 * -------------------------------------------------------------------------- *
 * This is part of the SimTK biosimulation toolkit originating from           *
 * Simbios, the NIH National Center for Physics-Based Simulation of           *
 * Biological Structures at Stanford, funded under the NIH Roadmap for        *
 * Medical Research, grant U54 GM072970. See https://simtk.org/home/simbody.  *
 *                                                                            *
 * Portions copyright (c) 2005-12 Stanford University and the Authors.        *
 * Authors: Ian Stavness, Michael Sherman                                     *
 * Contributors:                                                              *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may    *
 * not use this file except in compliance with the License. You may obtain a  *
 * copy of the License at http://www.apache.org/licenses/LICENSE-2.0.         *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 * -------------------------------------------------------------------------- */

/**
 * This example demonstrates finding the geodesic between two points on
 * a ContactGeometry object.
 **/


#include "Simbody.h"
#include "simmath/internal/Geodesic.h"
#include "simmath/internal/GeodesicGeometry.h"
#include "simmath/internal/ContactGeometry.h"

using namespace SimTK;
using std::cos;
using std::sin;
using std::cout;
using std::endl;


int main() {
  try {

    // setup test problem
    double r = 1;
    double uP = -Pi / 2;
    double vP = Pi / 3;
    double uQ = 0;
    double vQ = 2;
    Vec3 O(-r, -r, 0.2);
    Vec3 I(r, r, -r);
    Vec3 P(r * cos(uP) * sin(vP), r * sin(uP) * sin(vP), r * cos(vP));
    Vec3 Q(r * cos(uQ) * sin(vQ), r * sin(uQ) * sin(vQ), r * cos(vQ));

    Vec3 r_OP = P - O;
    Vec3 r_IQ = Q - I;
    Vec3 tP = r_OP.normalize();
    Vec3 tQ = r_IQ.normalize();

    int n = 2; // problem size
    Vector x(n), dx(n), Fx(n), xold(n);
    Matrix J(n, n);

    ContactGeometry::Sphere sphere(r);
    GeodesicGeometry geodgeom(sphere);
    Geodesic geod;

    // Create a dummy mb system for visualization
    MultibodySystem dummySystem;
    SimbodyMatterSubsystem matter(dummySystem);
    matter.updGround().addBodyDecoration(Transform(), DecorativeSphere(r)
            .setColor(Gray)
            .setOpacity(0.5)
            .setResolution(5));

    // Visualize with default options; ask for a report every 1/30 of a second
    // to match the Visualizer's default 30 frames per second rate.
    Visualizer viz(dummySystem);
    viz.setBackgroundType(Visualizer::SolidColor);
    dummySystem.addEventReporter(new Visualizer::Reporter(viz, 1./30));

    // add vizualization callbacks for geodesics, contact points, etc.
    Vector tmp(6); // tmp = ~[P Q]
    tmp[0]=P[0]; tmp[1]=P[1]; tmp[2]=P[2]; tmp[3]=Q[0]; tmp[4]=Q[1]; tmp[5]=Q[2];
    viz.addDecorationGenerator(new PathDecorator(tmp, P, Q, Green));
    viz.addDecorationGenerator(new PlaneDecorator(geodgeom.getPlane(), Gray));
    viz.addDecorationGenerator(new GeodesicDecorator(geodgeom.getGeodP(), Red));
    viz.addDecorationGenerator(new GeodesicDecorator(geodgeom.getGeodQ(), Blue));
//    viz.addDecorationGenerator(new GeodesicDecorator(geod, Orange));
    dummySystem.realizeTopology();
    State dummyState = dummySystem.getDefaultState();

    // calculate the geodesic
    geodgeom.calcGeodesic(P, Q, tP, tQ, geod);
    viz.report(dummyState);

    cout << "num geod pts = " << geod.getPoints().size() << endl;

  } catch (const std::exception& e) {
    std::printf("EXCEPTION THROWN: %s\n", e.what());
    exit(1);

  } catch (...) {
    std::printf("UNKNOWN EXCEPTION THROWN\n");
    exit(1);
  }

    return 0;
}
