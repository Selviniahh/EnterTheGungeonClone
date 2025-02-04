# Project Structure

Almost all the game objects has this 3 functions 
- **Initialize:** Initializing member variables when object being constructed. Animation's texture loading handled in **initialize** function  
- Update: In every frame this function is called. Sprite-sheet animation playing, input management, moving game obhects handled in **Update** function  
- **Draw**: Draws the game objects 

# Folder structure
under `src` folder there are 
- `Animation`
- `Characters`
- `Managers`
- `UI`

folders. 

---

### **Main.cpp**
- Initializes the `GameManager` and calls its `Update` and `Draw` methods.
- Ensures the thread sleeps when the window is unfocused to prevent crashes.

### **GameManager**
- The `GameManager` class is the core of the project. All game objects’ `Initialize`, `Update`, and `Draw` methods are managed here.
- Enemies, guns, levels, and other game elements must be initialized through the **GameManager**.

### **Globals**
- Global variables like `FrameTick` and `ScreenSize`, as well as relevant operator overloads, are defined here for universal access.

### **Input Manager**
- Calculates movement direction using the **W/A/S/D** keyboard inputs.
- Handles camera zoom (via **E/Q**), unzoom, and movement (using **arrow keys**).