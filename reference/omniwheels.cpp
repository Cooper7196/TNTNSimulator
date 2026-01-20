// lemlib/v5-sim-engine/v5-sim-engine-physics/src/bot.cpp

// ... (Keep the motor voltage calculation loops unchanged) ...

        // [EXISTING] Motor State Space Updates
        algebra::Matrix<double, 2, 1> e({lV.convert(volt), rV.convert(volt)});
        auto y_l = (A_l * X_l) + (B_l * e);
        auto y_r = (A_r * X_r) + (B_r * e);
        X_l = y_l;
        X_r = y_r;

        // [EXISTING] Calculate Forward Kinematics from Motors
        LinearVelocity leftSpeed = y_l(0, 0) * mps;
        LinearVelocity rightSpeed = y_r(1, 0) * mps;
        
        // This is the "forcing function" for forward velocity (High Traction assumption)
        LinearVelocity v_fwd_motor = (leftSpeed + rightSpeed) / 2;
        
        // Calculate Angular Velocity
        AngularVelocity omega = ((rightSpeed - leftSpeed) * rad) / (track_radius * 2);

        // --- NEW LATERAL DYNAMICS ---

        // 1. Momentum Rotation (Coriolis/Centrifugal effect approximation)
        // We must transfer momentum between Forward and Lateral states based on rotation.
        // If we turned by 'dTheta' this frame, our velocity vector rotates by '-dTheta' in the local frame.
        double dTheta = omega.convert(radps) * dt.convert(sec);
        
        // Get previous step's total local velocity
        // We use the *previous* motor speed for the momentum transfer, not the new one
        double v_fwd_prev = linVel.convert(mps); // You might need to store this as a class member 'prev_linVel'
        double v_lat_prev = X_lat;

        // Rotate the velocity vector
        // New Lateral Component = Old_Lat * cos(dTheta) - Old_Fwd * sin(dTheta)
        double v_lat_rotated = v_lat_prev * std::cos(dTheta) - v_fwd_prev * std::sin(dTheta);

        // 2. Apply Friction to Lateral State (State Space Input u)
        // Model: x[k+1] = x[k] + B * u
        // Where u is the friction force (-mu * g * sign(v))
        
        double friction_accel = mu_lat * gravity; // a = F/m = mu*g
        double max_friction_delta = friction_accel * dt.convert(sec);

        // Apply Coulomb friction opposing motion
        if (std::abs(v_lat_rotated) <= max_friction_delta) {
            X_lat = 0.0; // Stiction (Stopped)
        } else {
            if (v_lat_rotated > 0) {
                X_lat = v_lat_rotated - max_friction_delta;
            } else {
                X_lat = v_lat_rotated + max_friction_delta;
            }
        }

        // --- GLOBAL POSITION UPDATE ---

        // Now we have the final Local Velocities for this frame:
        // Forward: Controlled by Motors (High authority)
        // Lateral: Controlled by Momentum & Friction (Low authority)
        
        double final_v_fwd = v_fwd_motor.convert(mps);
        double final_v_lat = X_lat;

        // Project Local -> Global
        double heading = theta.convert(rad); // Current global heading
        double c = std::cos(heading);
        double s = std::sin(heading);

        // Global VX = V_fwd * cos(theta) - V_lat * sin(theta)
        // Global VY = V_fwd * sin(theta) + V_lat * cos(theta)
        // (Note: The sign on V_lat depends on your coordinate system. Assuming Right-Hand Rule:
        //  X is Forward, Y is Left -> Lateral velocity is along Y axis)
        //  Wait, usually X is Forward, Y is Lateral Left.
        //  Global X/Y is usually standard Cartesian.
        
        // Let's use standard rotation matrix:
        // [vx]   [cos -sin] [v_fwd]
        // [vy] = [sin  cos] [v_lat]
        // Note: Check your specific coordinate system (is Lateral Left or Right positive?)
        // Assuming Standard Math: Left is positive lateral.
        
        LinearVelocity vx = (final_v_fwd * c - final_v_lat * s) * mps;
        LinearVelocity vy = (final_v_fwd * s + final_v_lat * c) * mps;
        
        // Store linVel for next frame's rotation calculation
        linVel = v_fwd_motor; 

        // Update Position
        V2Position delta(vx * dt, vy * dt);
        vel = V2Velocity(vx, vy);

        pos += delta;
        theta += omega * dt;

        // ... (Rest of sensor updates) ...