"""
Write a program that simulates the motion of objects in space. (stars/planets/satellites etc.)
Any two objects are attracted to each other by the force of gravitation which can be calculated using the following formula:
(The formula is written in TeX, you can use free online tools such as latexbase.com to view it)

$$\vec{F_{ij}} = \frac{G m_i m_j}{r_{ij}^2} \frac{\vec{r_{ij}}}{\|r_{ij}\|}.$$

The force that acts on an object $i$ can be calculated as the vector sum of all forces acting on the object.
In our case, we only need to account for the gravitational forces caused by other objects in the simulation.

$$\vec{F_i} = \sum_{j \neq i} \vec{F_{ij}}$$

You are required to write four functions:

- "calculate_force" will calculate the force acting on an object based on other objects' current state in the simulation.
- "update_space_object" will calculate new coordinates and speed for an object based on the return value of calculate_force, the timestep and the object's current state.
- "update_motion" will simulate the motion of all objects for a single timestep (the size of the timestep is passed in)
- "simulate_motion" is a generator function that yields a dictionary with object names as keys and tuples of (x,y) coordinates as values. Each iteration progresses the simulation by the defined timestep.

More precise descriptions of each function's required functionality can be found in the code below.
For examples of function calls, check the test script `test_space_motion.py`.

You also have to write a parametrized decorator "logging" that will measure how many times a function
has been called and how long it ran. The information should be printed to standard output in this form:
"function_name - number_of_calls - time units\n".
The decorator should have an optional "units" parameter for specifying the output format (default is 'ms').
The decorator should accept 'ns', 'us', 'ms', 's', 'min', 'h' and 'days' as values for the "units" parameter.
The time should be printed as a float number with exactly 3 decimal places (eg. 0.042).

A couple more things:

- Use scientific notation (SI units).
- Do not bother with optimisation; just write something that works! (That is a nice thing to hear for once, isn't it?)
- Any function that takes multiple space objects should take them as separately named tuples, not as a list of them.

    f(5, earth, moon, mars, sun) - GOOD
    f(5, (earth, moon, mars, sun)) - MUCH BAD

Good luck!
"""

import time  # measuring time
from collections import namedtuple


# Define universal gravitation constant
G = 6.67408e-11  # N-m2/kg2
SpaceObject = namedtuple('SpaceObject', 'name mass x y vx vy color')
Force = namedtuple('Force', 'fx fy')
AllovedUnits = set(['ns', 'us', 'ms', 's', 'min', 'h', 'days'])

def print_ns(time):
    res = "{:.3f}".format(time * 1000000000);
    str = res + " ns"
    return str

def print_us(time):
    res = "{:.3f}".format(time * 1000000);
    str = res + " us"
    return str

def print_ms(time):
    res = "{:.3f}".format(time * 1000);
    str = res + " ms"
    return str

def print_s(time):
    res = "{:.3f}".format(time);
    str = res + " s"
    return str

def print_min(time):
    res = "{:.3f}".format(time / 60);
    str = res + " min"
    return str

def print_h(time):
    res = "{:.3f}".format(time / 3600);
    str = res + " h"
    return str

def print_days(time):
    res = "{:.3f}".format(time / 86400);
    str = res + " days"
    return str


Options = {'ns': print_ns, 'us': print_us, 'ms' : print_ms, 's':print_s,
           'min': print_min, 'h': print_h, 'days': print_days}


def logging(unit='ms'):
    def decorator(f):
        def wrapper(*args, **kvargs):
            wrapper.calls += 1
            start = time.time()
            ret = f(*args, **kvargs)
            end = time.time()
            if (unit not in Options):
                return False;
            else:
                print(format(f.__name__), wrapper.calls, Options[unit](end - start), sep=" - ")
            return ret;
        wrapper.calls = 0;
        return wrapper;
    return decorator;

def F_ij(m1, m2, distance):
    return (G * m1 * m2) / (distance**2)

def F_(f_ij, c1, c2, distance): #suradnica 
    return f_ij * (c2 - c1)/distance

@logging(unit='ms')
def calculate_force(*args):
    # input: one of the space objects (indexed as i in below formulas), other space objects (indexed as j, may be any number of them)
    # returns named tuple (see above) that represents x and y components of the gravitational force
    # calculate force (vector) for each pair (space_object, other_space_object):
    # |F_ij| = G*m_i*m_j/distance^2
    # F_x = |F_ij| * (other_object.x-space_object.x)/distance
    # analogous for F_y
    # for each coordinate (x, y) it sums force from all other space objects
    obj1 = args[0]
    fx_ = fy_ = 0
    #print(obj1.x, obj1.y, sep="    ")
    for obj2 in args[1:]:
        distance =  ((obj2.x - obj1.x)**2 + (obj2.y - obj1.y)**2)**0.5
        fx_ += F_(F_ij(obj1.mass, obj2.mass, distance), obj1.x, obj2.x, distance)
        fy_ += F_(F_ij(obj1.mass, obj2.mass, distance), obj1.y, obj2.y, distance)

    force = Force(fx = fx_, fy = fy_)
    #print(force.fx, force.fy)
    return force


@logging(unit='s')
def update_space_object(object, force, timestep):
    # here we update coordinates and speed of the object based on the force that acts on it
    # input: space_object we want to update (evolve in time), force (from all other objects) that acts on it, size of timestep
    # returns: named tuple (see above) that contains updated coordinates and speed for given space_object
    # hint:
    # acceleration_x = force_x / mass
    # same for y
    # speed_change_x = acceleration_x * timestep
    # same for y
    # speed_new_x = speed_old_x + speed_change_x
    # same for y
    # x_final = x_old + speed_new_x * timestep
    
    new_x = object.x
    new_y = object.y
    new_speed_x = object.vx
    new_speed_y = object.vy
    
    acceleration_x = force.fx / object.mass
    acceleration_y = force.fy / object.mass
    speed_change_x = acceleration_x * timestep
    speed_change_y = acceleration_y * timestep
    new_speed_x += speed_change_x
    new_speed_y += speed_change_y
    new_x += new_speed_x * timestep
    new_y += new_speed_y * timestep
    space_object = SpaceObject(name=object.name, mass=object.mass, x=new_x, y=new_y,
                               vx = new_speed_x, vy= new_speed_y, color=object.color)
    return space_object


@logging(unit='ms')
def update_motion(*args):
    # input: timestep and space objects we want to simulate (as named tuples above)
    # returns: list or tuple with updated objects
    # hint:
    # iterate over space objects, for given space object calculate_force with function above, update
    i = 0;
    timestep = args[0]
    space_objects = list(args[1:])
    #print (space_objects, sep="/n")
    updated_space_objects = []
    for i in range(len(space_objects)):
        space_objects[0], space_objects[i] = space_objects[i], space_objects[0]
        updated_space_objects.insert(i, update_space_object(space_objects[0], calculate_force(*space_objects), timestep))
        space_objects[0], space_objects[i] = space_objects[i], space_objects[0]
        i += 1
    return updated_space_objects  # (named tuple with x and y)


@logging()
def simulate_motion(*args):
    # generator that in every iteration yields dictionary with the name of the objects as a key and tuple of coordinates (x first, y second) as values
    # input size of the timestep, number of timesteps (integer), space objects (any number of them)
    size_of_timestep = args[0]
    number_of_timesteps = args[1]
    space_objects = args[2:]
    i = 0
    while i < number_of_timesteps:
        space_objects = update_motion(size_of_timestep, *space_objects);
        res = {space_objects[i].name: [space_objects[i].x, space_objects[i].y] for i in range(len(space_objects))}
        yield res
        i +=1

