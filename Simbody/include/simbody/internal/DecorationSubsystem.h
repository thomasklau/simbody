#ifndef SimTK_SIMBODY_DECORATION_SUBSYSTEM_H_
#define SimTK_SIMBODY_DECORATION_SUBSYSTEM_H_

/* -------------------------------------------------------------------------- *
 *                               Simbody(tm)                                  *
 * -------------------------------------------------------------------------- *
 * This is part of the SimTK biosimulation toolkit originating from           *
 * Simbios, the NIH National Center for Physics-Based Simulation of           *
 * Biological Structures at Stanford, funded under the NIH Roadmap for        *
 * Medical Research, grant U54 GM072970. See https://simtk.org/home/simbody.  *
 *                                                                            *
 * Portions copyright (c) 2007-12 Stanford University and the Authors.        *
 * Authors: Michael Sherman                                                   *
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

/** @file
 * Define the public interface to DecorationSubsystem, a "do nothing"
 * subsystem providing a place for a MultibodySystem Modeler to toss in some
 * visuals which may optionally be displayed by an application which uses
 * that System.
 */

#include "SimTKcommon.h"
#include "simbody/internal/common.h"

#include <cassert>

namespace SimTK {

class DecorationGenerator;
class DecorativeGeometry;
class DecorativeLine;
class MultibodySystem;

/**
 * This is the client-side handle class encapsulating the hidden implementation
 * of the DecorationSubsystem. Any Subsystem can generate decorative
 * geometry, so the methods for extracting the geometry are at the Subsystem
 * level and thus inherited here. So the only significant methods here are
 * those for adding decorative geometry to the System beyond that which is
 * generated by the other subsystems.
 */
class SimTK_SIMBODY_EXPORT DecorationSubsystem : public Subsystem {
public:
    DecorationSubsystem();
    explicit DecorationSubsystem(MultibodySystem&);

    void addBodyFixedDecoration(MobilizedBodyIndex bodyNum,
                                const Transform& X_GD,
                                const DecorativeGeometry&);

    void addRubberBandLine(MobilizedBodyIndex b1, const Vec3& station1,
                           MobilizedBodyIndex b2, const Vec3& station2,
                           const DecorativeLine&);
    /**
     * Add a DecorationGenerator that will be invoked to add dynamically generated geometry
     * to the scene.  The DecorationSubsystem assumes ownership of the object passed to this method,
     * and will delete it when the subsystem is deleted.
     *
     * @param stage     the Stage the generator should be invoked at
     * @param generator the DecorationGenerator to add
     */
    void addDecorationGenerator(Stage stage, DecorationGenerator* generator);

    SimTK_PIMPL_DOWNCAST(DecorationSubsystem, Subsystem);
    class DecorationSubsystemGuts& updGuts();
    const DecorationSubsystemGuts& getGuts() const;
};

} // namespace SimTK

#endif // SimTK_SIMBODY_DECORATION_SUBSYSTEM_H_
