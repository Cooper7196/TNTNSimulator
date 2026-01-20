import math
import numpy as np
from scipy.signal import StateSpace

dt = 0.01

# Robot Parameters
mass = 10
inertia = 6
track_radius = 6 * 0.0254
gear_ratio = 1/1.5
wheel_radius = 2 * 0.0254
num_motors_per_side = 3

# Motor Parameters
nominal_voltage = 12.0
stall_torque = 2.1286320584285905 * num_motors_per_side
stall_current = 2.5029144189407573 * num_motors_per_side
free_current = 0.13 * num_motors_per_side
free_speed = 200

# convert rpm to rad/s
free_speed = free_speed / 60 * (2 * np.pi)

resistance = nominal_voltage / stall_current
torque_const = stall_torque / stall_current
angular_vel_const = free_speed / (nominal_voltage - resistance * free_current)

input_voltage_left = 10
input_voltage_right = 12



C1 = -(gear_ratio ** 2 * torque_const) / \
    (angular_vel_const * resistance * wheel_radius**2)
C2 = (gear_ratio * torque_const) / (resistance * wheel_radius)

A = np.array(
    [
        [(1/mass + track_radius**2 / inertia) * C1,
         (1/mass - track_radius**2 / inertia) * C1],
        [(1/mass - track_radius**2 / inertia) * C1,
         (1/mass + track_radius**2 / inertia) * C1]
    ]
)
B = np.array(
    [
        [(1/mass + track_radius**2 / inertia) * C2,
         (1/mass - track_radius**2 / inertia) * C2],
        [(1/mass - track_radius**2 / inertia) * C2,
         (1/mass + track_radius**2 / inertia) * C2]
    ]
)

C = np.eye(2)
D = np.zeros((2, 2))

sys = StateSpace(A, B, C, D)

sim = sys.to_discrete(dt)

theta = 0
x_pos = 0
y_pos = 0

x = np.zeros((2, 1))

speedData = []
for i in range(1000):
    
    x = sim.A @ x + \
        sim.B @ np.array([[input_voltage_left], [input_voltage_right]])

    leftSpeed = x[0][0]
    rightSpeed = x[1][0]

    vel = (leftSpeed + rightSpeed) / 2
    omega = (rightSpeed - leftSpeed) / (track_radius * 2)
    xSpeed = vel * math.cos(theta) * dt
    ySpeed = vel * math.sin(theta) * dt
    angularSpeed = omega * dt

    x_pos += xSpeed
    y_pos += ySpeed
    theta += angularSpeed
