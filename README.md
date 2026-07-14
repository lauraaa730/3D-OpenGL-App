# PGR-semestral



# 3D Interactive Night Oasis (OpenGL)

A real-time 3D interactive application built in C++ and OpenGL. The scene features a large rock formation surrounding a small pond with an animated waterfall and a swimming fish. The environment is further detailed with flora, stones, and interactive crystals, all enclosed within a night sky skybox.

<!-- PRO-TIP: Drag and drop a screenshot or GIF of your scene right here! -->
![Application Preview](Drag_and_drop_your_gif_or_image_here)

---

##Key Technical Features
* **Lighting & Shading:** Implemented the Phong illumination model to dynamically light the scene and objects.
* **Object Interaction (3D Picking):** Raycasting/Picking system allowing users to click on 3D crystals to dynamically change their color.
* **Advanced Camera System:** Complex camera management featuring multiple modes: free-look, static viewpoints, and a dynamic tracking view attached to the swimming fish.
* **Environment Rendering:** Implemented a cubemap skybox for the night sky backdrop.
* **Animations:** Dynamic movement handling for the swimming fish and waterfall elements.

---

##Controls
* **`W` / `S`**: Move camera forward / backward (along the viewing vector)
* **`A` / `D`**: Move camera left / right
* **`SHIFT` / `CTRL`**: Move camera up / down (along global world coordinates)
* **`Mouse Movement`**: Rotate camera (Free-look mode)
* **`C`**: Switch camera modes (Cycles through: 2x static positions, free-look, and dynamic fish view)
* **`Left Mouse Click`**: Change the color of the crystal currently under the cursor
* **`F2`**: Print application status to the terminal
* **`ESC`**: Exit the application

---

## Tech Stack
* **Language:** C++
* **Graphics API:** OpenGL 3.3+ (Core Profile)
* **Libraries:** [Add any you used here, e.g., GLFW, GLM, Assimp]
