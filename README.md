## Project Description
An interactive 3D graphics application created in OpenGL. The scene consists of a large rock formation with a small pond in the middle, featuring a waterfall and a swimming fish. Furthermore, the scene includes flowers, stones, and crystals that change color when clicked. Lighting is calculated using the Phong illumination model, and the entire scene is surrounded by a night sky skybox.

<img width="300" height="250" alt="thumb1" src="https://github.com/user-attachments/assets/03084a51-9a50-4cb2-a63f-8b4b04517e3d" />
<img width="300" height="250" alt="thumb2" src="https://github.com/user-attachments/assets/9556ed9c-6a44-464c-9b5d-f95a71ef0184" />

## Controls
* **W/S** - Move camera forward/backward in its viewing direction.
* **A/D** - Move camera left/right.
* **SHIFT/CTRL** - Move camera up/down in world coordinates.
* **Passive mouse movement** - Rotate the camera.
* **Left mouse click** - Change the color of the crystal under the cursor.
* **F2** - Print status to the terminal.
* **C** - Switch camera mode (2x static position, free-look, dynamic view from the fish).
* **ESC** - Exit the program.

## Technical specification
* **Language:** C++
* **Graphics API:** OpenGL 3.1+
* **Libraries:** CVUT University's framework, which includes Assimp, FREEGLUT... https://cent.felk.cvut.cz/courses/PGR/framework/doc/index.html
