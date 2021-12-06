# Vroom Engine
## Forked from [solidajenjo/Engine3D](https://github.com/solidajenjo/Engine3D)
## Description
Next generation engine coded in cpp for videogames.
## Features
 - Automatically load 'Street Environment'.
 - There is an “Assets” window that shows all user assets.
 - The user can drop/import new files. Assets window reacts properly.
 - Upon start, all resources not managed inside Assets are generated in Library.
 - Various settings and performance metrics in the menus.
 - All resources use reference counting.
 -The editor has 2 windows one with the scene(editor view) and another captured from a gameobject with a camera component(main game camera), each one has its own framebuffer and the user can visualize both at the same time.
 - All meshes use a bounding volume (AABB) and can be discarded using Frustum Culling.
 - Hierarchy: delete, reparent, create empty and create children.
 - Transform: translate, rotate and scale Game Objects.
 - Mesh: select or drop any imported mesh.
 -  Camera is a component with settings that can be modified.
 -  GameObjects can be picked from the world using mouse.
 ## Controls

 - Camera moves with WASD + right click (mouse).
 - Pressing F focusses camera on selected object.
 - Holding SHIFT doubles movement speed.
 - LALT + right click (mouse) rotates around selected object.
 - Mouse wheel up/down zooms scene view.
 - Left click to select a GameObject in the scene.
 - Delete to delete a GameObject.

## Developers

 - Isaac Digón[https://github.com/isaac553876299]
 - Isaac Colomer[https://github.com/IsaaColomer]
 - Ignasi Pardo[https://github.com/KuronoaScarlet]

**Students of 3rd year at CITM degree on Videogames Design and Development**

## License
MIT License

Copyright (c) [2021] [Vroom Engine]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.