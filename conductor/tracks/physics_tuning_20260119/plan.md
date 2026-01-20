# Implementation Plan: Refine and Tune Physics Engine

## Phase 1: Analysis and Debug Visualization

- [x] Task: Add on-screen debug text rendering to the `Renderer` class to show velocity and position.
- [x] Task: Verify the implementation of the state-space discretization and update logic.
- [x] Task: Conductor - User Manual Verification 'Phase 1' (Protocol in workflow.md)

## Phase 2: Friction Implementation

- [~] Task: Add linear and angular friction coefficients to the `Robot` class.
- [~] Task: Update `Robot::update` to apply friction forces/torques *before* or *integrated into* the state-space update.
    - *Note:* State-space models usually handle "damping" via the `A` matrix (the back-EMF part acts as damping). If it's drifting too much, the back-EMF term (`C1`) might be incorrect or we need extra external friction (scrubbing).
- [ ] Task: Conductor - User Manual Verification 'Phase 2' (Protocol in workflow.md)

## Phase 3: Tuning and Refinement

- [x] Task: Tune the `Inertia` parameter to make turning less "floaty".
- [x] Task: Tune the `Mass` parameter.
- [x] Task: Adjust friction coefficients to achieve realistic stopping.
- [x] Task: Conductor - User Manual Verification 'Phase 3' (Protocol in workflow.md)
