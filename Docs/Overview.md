# Sector Engine - Overview


## Coordinate Systems

### World Space

The world space in the sector engine is a right-handed, Z up coordinate system:
1. The +Z axis corresponds to up in the world. 
2. The +X axis corresponds to east, and direction of 0deg angle. 
3. The +Y axis corresponds to north, and direction 90 deg to the X axis. 
4. Angles increase in counter-clockwise direction, starting from the X axis.

This makes the XY plane the horizontal plane that sectors are defined on, and
the Z axis the extra height dimesion for objects in the engine. 

This coordinate system was chosen to make the cordinate systems of the game and 
level editor line up better. In the 2D level editor, it makes sense to have the two axis named X and Y(as opposed to say X and Z) since there are only two dimensions. Since sectors aredefined in a 2D editor, we follow that convesion and make the 3rd axis, Up/Down the Z axis. We could make the Y axis the up/down axis, but it makes eding moreconfusing since you are editing in the XZ plane. 

This requires converting this coordinate system to the native coordinate system
of the graphics framework, but this is a minor step that can be done by modifying
a matrix or swizzling some vectors. This also makes the coordinate system
independent of a particular framework, since OpenGL and DirectX follow different
conventions.